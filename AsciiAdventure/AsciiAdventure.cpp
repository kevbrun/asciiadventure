#include <iostream>
#include <boost/log/trivial.hpp>
#include <Windows.h>
#include <chrono>

using namespace std;


int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;


int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4;
float fDepth = 16.0;

int main()
{
   
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;


    //Build tilemap

    wstring map;

    map += L"###############";
    map += L"#.............#";
    map += L"#.............#";
    map += L"#...#.........#";
    map += L"#.............#";
    map += L"#.......#.....#";
    map += L"#.............#";
    map += L"#.............#";
    map += L"#.............#";
    map += L"#.............#";
    map += L"#.......#######";
    map += L"#.............#";
    map += L"#.............#";
    map += L"#.............#";
    map += L"###############";

    

// Game Loop Delta time
    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();



    //Game Loop
    while (1) {



        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();



        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
            fPlayerA -= (0.8f) * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            fPlayerA += (0.8f) * fElapsedTime;
        }


        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            //rotate camera by univector
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
        }

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            //rotate camera by univector
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
        }


        for (int x = 0; x < nScreenWidth; x++) {
             
            // For each column calculate the project ray angle in world space 
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;


            float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
            float fEyeY = cosf(fRayAngle); // Unit vector for ray in player space

            while(!bHitWall && fDistanceToWall < fDepth) {
                fDistanceToWall += 0.1f;


                //Send raycast into the scene and check if at coordinate nTest a wall is hit
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                //Test if ray is out of bound
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapWidth) {
                    bHitWall = true;
                    fDistanceToWall =  fDepth; // Set distance to maximum depth
                }
                else {


                    //If Ray is within map boundaries check if at position is a wall

                    if (map[nTestY * nMapWidth + nTestX] == '#') {
                        bHitWall = true;
                  
                    }

                }

            }

            //Calculate distance to ceiling and floor
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            short nShade = ' ';

            //Get unicdode character for painting the wall based on Distance to wall
            if (fDistanceToWall < fDepth / 4.0f )   nShade = 0x2588;
            else if (fDistanceToWall < fDepth / 3.0f )  nShade = 0x2593;
            else if (fDistanceToWall < fDepth / 2.0f )  nShade = 0x2592;
            else if (fDistanceToWall < fDepth )    nShade = 0x2591;
            else  nShade = ' '; // Very far away wall

            for (int y = 0; y < nScreenHeight; y++) {
                if (y < nCeiling) {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if(y > nCeiling && y <= nFloor) {
                    screen[y * nScreenWidth + x] = nShade;
                }
                else {

                    //Shade floor based on distance

                    float distance = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2));
                    if (distance < 0.25)   nShade = '#';
                    else if (distance < 0.5 )  nShade = 'x';
                    else if (distance < 0.75)  nShade = '.';
                    else if (distance < 0.9)    nShade = '-';
                    else  nShade = ' '; // Very far away wall


                    screen[y * nScreenWidth + x] = nShade;
                    //23.54
                }

            }

        }
        //Write to console buffer
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    




    return 0;

}


