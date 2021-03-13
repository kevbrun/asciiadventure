// AsciiAdventureEngine.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "AsciiAdventureEngine.h"


// This is an example of an exported variable
ASCIIADVENTUREENGINE_API int nAsciiAdventureEngine=0;

// This is an example of an exported function.
ASCIIADVENTUREENGINE_API int fnAsciiAdventureEngine(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CAsciiAdventureEngine::CAsciiAdventureEngine()
{
    return;
}
