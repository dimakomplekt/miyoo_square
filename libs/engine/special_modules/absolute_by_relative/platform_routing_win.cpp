// =========================================================================================== PLATFORM DEFINITION

#include "platform_routing.h"

#include "../../base_modules/platform/platform.h"

// =========================================================================================== PLATFORM DEFINITION


// =========================================================================================== WINDOWS LINK TO .EXE

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN

#include <windows.h>


std::filesystem::path get_exe_dir()
{
    char buffer[MAX_PATH];

    DWORD length = GetModuleFileNameA(
        nullptr,
        buffer,
        MAX_PATH
    );

    if (length == 0)
        return std::filesystem::current_path();

    return std::filesystem::path(
        buffer,
        buffer + length
    ).parent_path();
}

#endif

// =========================================================================================== WINDOWS LINK TO .EXE