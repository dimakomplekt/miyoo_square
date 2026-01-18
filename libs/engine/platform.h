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


// =========================================================================================== SDL2 INCLUDE

#define SDL_MAIN_HANDLED

#ifdef PLATFORM_WINDOWS
    // Windows: use SDL2 for Windows
    #include "../sdl/win_SDL2-2.32.10/include/SDL.h"
    // #include <SDL.h>
#else
    // Linux / Miyoo Mini+: use SDL2 from Onion OS templates
    #include <SDL2/SDL.h>
#endif

// =========================================================================================== SDL2 INCLUDE


// =========================================================================================== COMMON DEFINES

enum Button {

    START_BTN,
    SELECT_BTN,
    LEFT_BTN,
    UP_BTN,
    RIGHT_BTN,
    DOWN_BTN,
    Y_BTN,
    X_BTN,
    A_BTN,
    B_BTN

};

// =========================================================================================== COMMON DEFINES
