#include <iostream>
#include <boost/log/trivial.hpp>
#include <Windows.h>

using namespace std;


int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 0.0f;
float fPlayerY = 0.0f;
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

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";




    //Game Loop
    while (1) {


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
                       // 13:20
                    }

                }

            }



        }
        //Write to console buffer
        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    




    return 0;

}


