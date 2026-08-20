// engine.h


#pragma once


// =========================================================================================== IMPORT

// ===== PLATFORM =====

// Current platform by preprocessor directives
#include "base_modules/platform/platform.h"

// ===== PLATFORM =====



// ===== SDL =====

// Includes framework (library contains links to SDL2
// TODO: IMPORT? LINUX?

#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL_image.h>


// ===== SDL =====



// ===== INPUTS =====

// Global inputs control 
#include "logic_modules/program_logic/global_inputs/global_inputs.h" 

// ===== INPUTS =====



// ===== LANGUAGE =====

// Language state management (library contains links to language choose methods
// for program dictionary workflow)
#include "logic_modules/program_logic/lang_state/lang_state.h"
#include "logic_modules/program_logic/lang_state/lang_utils.h"

// ===== LANGUAGE =====



// ===== STATE MACHINE =====

// State machine include
#include "logic_modules/program_logic/state_machine/state_machine.h"

// ===== STATE MACHINE =====



// ===== COMPILER HELPERS =====

// Absolute path by relative path from .exe folder
#include "special_modules/absolute_by_relative/absolute_by_relative.h"


// ===== COMPILER HELPERS =====



// =========================================================================================== IMPORT


// =========================================================================================== DEFINES

// ===== TESTS =====

#define TEST_MODE false                 // Basic testing


// ===== TESTS =====


// =========================================================================================== DEFINES