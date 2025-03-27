// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        initBase();
        break;
    case DLL_THREAD_ATTACH:
        initLibrary("");
        break;
    case DLL_THREAD_DETACH:
        finalizeLibrary();
        break;
    case DLL_PROCESS_DETACH:
        finalizeBase();
        break;
    }
    return TRUE;
}

