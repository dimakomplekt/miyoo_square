#pragma once

// =========================================================================================== IMPORT

#include "state_machine.h"
#include <vector>
#include <string>
#include <utility> // for std::pair

// =========================================================================================== IMPORT


// =========================================================================================== STATE IDS

/**
 * @brief Unique hierarchical identifiers for all game states.
 *
 * Each ID defines the position of a state in the state machine hierarchy.
 * This makes it easy to determine parent-child relationships and manage
 * nested states.
 */
const State_ID START_ID            = {0};           // Initial boot/start state
const State_ID MAIN_MENU_ID        = {1};           // Main menu state
const State_ID GAME_ID             = {1, 1};        // Top-level game state
const State_ID LEVEL_GAMEPLAY_ID   = {1, 1, 1};     // Gameplay level state
const State_ID SMALL_MENU_ID       = {1, 1, 2};     // In-game small menu
const State_ID EXIT_PROGRAM_ID     = {2};           // Program exit state


/**
 * @brief Mapping of State_IDs to human-readable names.
 *
 * Useful for debugging, logging, or automatic state registration.
 */
const std::vector<std::pair<State_ID, std::string>> state_defs = {
    {{0},       "START"},
    {{1},       "MAIN_MENU"},
    {{1,1},     "GAME"},
    {{1,1,1},   "LEVEL_GAMEPLAY"},
    {{1,1,2},   "SMALL_MENU"},
    {{2},       "EXIT_PROGRAM"}
};

// =========================================================================================== STATE IDS


// =========================================================================================== CALLBACKS

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

void start_enter();
void start_exit();
void start_render(SDL_Renderer* renderer);

void main_menu_enter();
void main_menu_exit();

void game_enter();
void game_exit();

void level_gameplay_enter();
void level_gameplay_exit();

void small_menu_enter();
void small_menu_exit();

void exit_program_enter();
void exit_program_exit();

// =========================================================================================== CALLBACKS


// =========================================================================================== INITIALIZATION

/**
 * @brief Initializes all game states and adds them to the state machine.
 *
 * - Creates State objects for each defined ID and name.
 * - Assigns the corresponding enter and exit callbacks.
 * - Automatically links parent and child states based on hierarchical IDs.
 *
 * @param app_state_machine Reference to the State_machine to populate.
 */
void init_game_states(State_machine& app_state_machine);

// =========================================================================================== INITIALIZATION
