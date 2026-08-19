// platform.h

#pragma once

// =========================================================================================== PLATFORM DEFINITION

#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS

#elif defined(__linux__)
    #define PLATFORM_LINUX

#else   
    #error "Unsupported platform"
#endif

// =========================================================================================== PLATFORM DEFINITION


// =========================================================================================== PLATFORM-SPECIFIC DEFINES


enum class Platform {

    Windows,
    Linux

};


// For logic switch use expressions like: "if (current_platform == Platform::Linux)..."
constexpr Platform current_platform =

#ifdef PLATFORM_WINDOWS

    Platform::Windows;

#elif defined(PLATFORM_LINUX)

    Platform::Linux;

#else

    #error "Unknown platform mapping"

#endif

/*

if (current_platform == Platform::Windows)
{
    // Windows-specified defines
}
else if (current_platform == Platform::Linux)
{
    // Linux-specified defines
}
else
{
    #error "Unknown platform handling
}

*/

// =========================================================================================== PLATFORM-SPECIFIC DEFINES

// =========================================================================================== COMMON DEFINES

#define SDL_MAIN_HANDLED

// =========================================================================================== COMMON DEFINES
