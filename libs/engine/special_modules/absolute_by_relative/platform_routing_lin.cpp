// =========================================================================================== PLATFORM DEFINITION

#include "platform_routing.h"

#include "../../base_modules/platform/platform.h"

// =========================================================================================== PLATFORM DEFINITION


// =========================================================================================== LINUX / MIYOO LINK TO .EXE

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MIYOO)

#include <unistd.h>
#include <limits.h>


std::filesystem::path get_exe_dir()
{
    char buffer[PATH_MAX];

    ssize_t len = readlink(
        "/proc/self/exe",
        buffer,
        sizeof(buffer) - 1
    );

    if (len <= 0)
        return std::filesystem::current_path();

    buffer[len] = '\0';

    return std::filesystem::path(buffer).parent_path();
}

#endif

// =========================================================================================== LINUX / MIYOO LINK TO .EXE