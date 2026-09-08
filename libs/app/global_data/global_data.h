// global_data.h

#pragma once

// =========================================================================================== IMPORT

#include "program_dictionary/program_dictionary.h"

#include <SDL2/SDL.h>

#include <string>

#include "app_timer/app_timer.h"

// =========================================================================================== IMPORT


// =========================================================================================== APP SETTINGS

/**
 * @brief Default application name.
 *
 * Used as the main window title and as the base identifier
 * for the running program instance.
 * 
 */
const std::string THIS_APP_NAME = "MIYOO_SQUARE_1.0"; 


// =========================================================================================== APP SETTINGS


// =========================================================================================== GLOBAL WINDOW SETTINGS

// =========================================================================================== TEST FLAGS

#define MIYOO_TEST_CASENAME   0
#define MIYOO_TEST_APP_LOG    0
#define MIYOO_TEST_START_LOG  0



#if MIYOO_TEST_APP_LOG
    #define APP_LOG(x) std::cout << "[APP] " << x << std::endl
#else
    #define APP_LOG(x) do {} while (0)
#endif

// =========================================================================================== TEST FLAGS

// Main window horizontal size
#define MAIN_WINDOW_H_SIZE 640 

// Main window vertical size
#define MAIN_WINDOW_V_SIZE 480


// ===== SDL3 AND SDL2 CONFLICT =====

// const SDL_WindowFlags THIS_APP_WINDOW_FLAG = 0; // | SDL_WINDOW_ALLOW_HIGHDPI

#ifdef PLATFORM_WINDOWS

    const Uint32 THIS_APP_WINDOW_FLAG = SDL_WINDOW_ALLOW_HIGHDPI; 

#elif defined(PLATFORM_MIYOO)

    const Uint32 THIS_APP_WINDOW_FLAG = SDL_WINDOW_SHOWN; 

#endif

// ===== SDL3 AND SDL2 CONFLICT =====



// =========================================================================================== GLOBAL WINDOW SETTINGS
