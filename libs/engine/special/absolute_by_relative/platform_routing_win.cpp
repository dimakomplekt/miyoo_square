// platform_routing_win.cpp

// =========================================================================================== PLATFORM DEFINITION

#include "platform_routing.h"
#include "../../platform/platform.h"


// =========================================================================================== PLATFORM DEFINITION


// =========================================================================================== WINDOWS LINK TO .EXE

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN

// 1 time include in cpp file, not in header
#include <windows.h>

std::filesystem::path get_exe_dir()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
}

#endif

// =========================================================================================== WINDOWS LINK TO .EXE
