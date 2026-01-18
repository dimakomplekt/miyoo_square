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
    // Чистим экран
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Рисуем красный круг
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
    auto start = std::make_unique<State>(START_ID, "START");
    start->on_enter = start_enter;                                  // Called when we switch into START
    start->on_exit  = start_exit;                                   // Called when we leave START
    start->state_render = start_render;
    app_state_machine.add_state(std::move(start));                  // Add to the state machine

    // === MAIN_MENU ===
    auto main_menu = std::make_unique<State>(MAIN_MENU_ID, "MAIN_MENU");
    main_menu->on_enter = main_menu_enter;
    main_menu->on_exit  = main_menu_exit;
    app_state_machine.add_state(std::move(main_menu));

    // === GAME ===
    auto game = std::make_unique<State>(GAME_ID, "GAME");
    game->on_enter = game_enter;
    game->on_exit  = game_exit;
    app_state_machine.add_state(std::move(game));

    // === LEVEL_GAMEPLAY ===
    auto level_gameplay = std::make_unique<State>(LEVEL_GAMEPLAY_ID, "LEVEL_GAMEPLAY");
    level_gameplay->on_enter = level_gameplay_enter;
    level_gameplay->on_exit  = level_gameplay_exit;
    app_state_machine.add_state(std::move(level_gameplay));

    // === SMALL_MENU ===
    auto small_menu = std::make_unique<State>(SMALL_MENU_ID, "SMALL_MENU");
    small_menu->on_enter = small_menu_enter;
    small_menu->on_exit  = small_menu_exit;
    app_state_machine.add_state(std::move(small_menu));

    // === EXIT_PROGRAM ===
    auto exit_program = std::make_unique<State>(EXIT_PROGRAM_ID, "EXIT_PROGRAM");
    exit_program->on_enter = exit_program_enter;
    exit_program->on_exit  = exit_program_exit;
    app_state_machine.add_state(std::move(exit_program));

    // At this point, all states are registered in the state machine.
    // State_machine handles connecting parents and children based on IDs,
    // so hierarchical updates and callback chaining will work automatically.
}

// =========================================================================================== INITIALIZATION
