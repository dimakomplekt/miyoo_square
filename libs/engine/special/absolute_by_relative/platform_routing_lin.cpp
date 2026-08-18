// platform_rouing_win.cpp


// =========================================================================================== PLATFORM DEFINITION

#include "platform_routing.h"
#include "../../platform/platform.h"

// =========================================================================================== PLATFORM DEFINITION


// =========================================================================================== LINUX LINK TO .EXE

#ifdef PLATFORM_LINUX

#include <unistd.h>
#include <limits.h>

std::filesystem::path get_exe_dir()
{
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer));
    return std::filesystem::path(std::string(buffer, len)).parent_path();
}

#endif

// =========================================================================================== LINUX LINK TO .EXE
