// engine.h


#pragma once


// =========================================================================================== IMPORT

// Language state management (library contains links to language choose methods
// for program dictionary workflow)

#include "lang_state/lang_state.h"
#include "lang_state/lang_utils.h"


// Current platform by preprocessor directives

#include "platform/platform.h"


// State machine include

#include "state_machine/state_machine.h"


// Includes both the frameworks and translation methods (library contains links 
// to both SDL3 and OpenCV2)

#include "special/opencv_to_sdl/opencv_to_sdl.h"

// SDL ttf include

#include "SDL3_ttf-3.2.2/include/SDL3_ttf/SDL_ttf.h"

// SDL image include
#include "SDL3_image-3.4.4/include/SDL3_image/SDL_image.h"

// Mouse and keyboard read

#include "global_inputs/global_inputs.h" 


// Absolute path by relative path from .exe folder

#include "special/absolute_by_relative/absolute_by_relative.h"

// =========================================================================================== IMPORT


// =========================================================================================== DEFINE


#define TEST_MODE false

// =========================================================================================== DEFINE