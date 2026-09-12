// 2.0_GAME.h


#pragma once

// =========================================================================================== IMPORT

#include "../../../../engine/engine.h"

#include "../../../../engine/logic_modules/program_gui/basic_elements/global_palette/global_palette.h"
#include "../../../../engine/logic_modules/program_gui/basic_elements/global_fonts/global_fonts.h"

#include "../../../global_data/global_data.h"


#include "../../../game_objects/game_objects_include.h"

// =========================================================================================== IMPORT


// =========================================================================================== MAIN STATE API

/**
 * Functions called when entering or exiting a specific state.
 *
 * These are assigned to the `on_enter` and `on_exit` callbacks of the
 * corresponding State objects in the state machine.
 *
 * Example:
 * 
 * app_state_machine.add_state(std::make_unique<State>(START_ID, "START"));
 * state->on_enter = start_enter;
 * 
 */

void game_enter();
void game_exit();

void game_update();
void game_render(SDL_Renderer* renderer);

// =========================================================================================== MAIN STATE API
