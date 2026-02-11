// game_states.cpp


// =========================================================================================== IMPORT

#include "game_states.h"
#include <iostream> // for std::cout, std::cerr

// =========================================================================================== IMPORT


// =========================================================================================== CALLBACKS

// These are the callbacks executed when entering or exiting a state.
// Currently, they are simple stubs printing to the console.
// You can replace the body with more complex logic or calls to other modules.


void start_enter()         { std::cout << "Entering START\n"; }
void start_exit()          { std::cout << "Exiting START\n"; }

void start_render(SDL_Renderer* renderer)
{

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Рисуем красный круг для чека работоспособности
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int cx = 400, cy = 300, r = 50;

    for (int w = 0; w < r * 2; w++)
    {
        for (int h = 0; h < r * 2; h++)
        {
            int dx = r - w;
            int dy = r - h;

            if ((dx*dx + dy*dy) <= (r*r)) SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
        }
    }

    SDL_RenderPresent(renderer);
}


void main_menu_enter()     { std::cout << "Entering MAIN_MENU\n"; }
void main_menu_exit()      { std::cout << "Exiting MAIN_MENU\n"; }

void game_enter()          { std::cout << "Entering GAME\n"; }
void game_exit()           { std::cout << "Exiting GAME\n"; }

void level_gameplay_enter(){ std::cout << "Entering LEVEL_GAMEPLAY\n"; }
void level_gameplay_exit() { std::cout << "Exiting LEVEL_GAMEPLAY\n"; }

void small_menu_enter()    { std::cout << "Entering SMALL_MENU\n"; }
void small_menu_exit()     { std::cout << "Exiting SMALL_MENU\n"; }

void exit_program_enter()  { std::cout << "Entering EXIT_PROGRAM\n"; }
void exit_program_exit()   { std::cout << "Exiting EXIT_PROGRAM\n"; }

// =========================================================================================== CALLBACKS


// =========================================================================================== INITIALIZATION

void init_game_states(State_machine& app_state_machine)
{
    // Each block below creates a State object, assigns its enter/exit callbacks,
    // and adds it to the state machine. The hierarchy linking is handled
    // automatically by State_machine::add_state.

    // === START ===

    // Smart initialization by the state machine
    app_state_machine.initiate_state(START_ID, "START");

    // Callbacs setting by the state machine
    if (auto* s = app_state_machine.get_state(START_ID))
    {
        s->on_enter = start_enter;          // Actions on the state entering 
        s->on_exit  = start_exit;           // Actions on the state exit
        s->state_render = start_render;     // Rendering for the state
    }


    // === MAIN_MENU ===
    app_state_machine.initiate_state(MAIN_MENU_ID, "MAIN_MENU");

    if (auto* s = app_state_machine.get_state(MAIN_MENU_ID))
    {
        s->on_enter = main_menu_enter;
        s->on_exit  = main_menu_exit;
    }


    // === GAME ===
    app_state_machine.initiate_state(GAME_ID, "GAME");

    if (auto* s = app_state_machine.get_state(GAME_ID))
    {
        s->on_enter = game_enter;
        s->on_exit  = game_exit;
    }


    // === LEVEL_GAMEPLAY ===
    app_state_machine.initiate_state(LEVEL_GAMEPLAY_ID, "LEVEL_GAMEPLAY");

    if (auto* s = app_state_machine.get_state(LEVEL_GAMEPLAY_ID))
    {
        s->on_enter = level_gameplay_enter;
        s->on_exit  = level_gameplay_exit;
    }


    // === SMALL_MENU ===
    app_state_machine.initiate_state(SMALL_MENU_ID, "SMALL_MENU");

    if (auto* s = app_state_machine.get_state(SMALL_MENU_ID))
    {
        s->on_enter = small_menu_enter;
        s->on_exit  = small_menu_exit;
    }


    // === EXIT_PROGRAM ===
    app_state_machine.initiate_state(EXIT_PROGRAM_ID, "EXIT_PROGRAM");

    if (auto* s = app_state_machine.get_state(EXIT_PROGRAM_ID))
    {
        s->on_enter = exit_program_enter;
        s->on_exit  = exit_program_exit;
    }

    // At this point, all states are registered in the state machine.
    // State_machine handles connecting parents and children based on IDs,
    // so hierarchical updates and callback chaining will work automatically.
}

// =========================================================================================== INITIALIZATION
