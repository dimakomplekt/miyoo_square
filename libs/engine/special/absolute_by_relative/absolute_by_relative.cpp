// absolute_by_relative.cpp


// =========================================================================================== IMPORT

#include "absolute_by_relative.h"
#include "platform_routing.h"

// =========================================================================================== IMPORT


// =========================================================================================== IMPORT

// Absolute path by relative path from exe

std::string absolute_by_relative_from_exe(const std::string& relative)
{
    return (get_exe_dir() / relative).string();
}

// =========================================================================================== IMPORT