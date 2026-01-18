#include <iostream>


#include "../libs/engine/app_logic/app.h"
#include "../libs/game_logic/game_states/game_states.h"

int main()
{
    sdl_app_ctx app_test;

    // Initialize SDL application
    if (!SDL_app_init(&app_test, 800, 600, "Miyoo Square"))
    {
        std::cerr << "Failed to initialize SDL application." << std::endl;
        return -1;
    }

    // Initialize game states
    init_game_states(app_test.app_sm);

    // Set the initial state to START_ID
    if (!app_test.app_sm.go_to(START_ID))
    {
        std::cerr << "Failed to set initial state to START_ID." << std::endl;
        SDL_app_shutdown(&app_test);
        return -1;
    }

    // Main loop
    SDL_Event event;

    while (app_test.app_state == SDL_APP_CONTINUE)
    {
        // Handle events
        while (SDL_PollEvent(&event))
        {
            SDL_app_event(&app_test, &event);
        }

        // Update and render
        if (!SDL_app_cycle(&app_test))
        {
            break;
        }
    }

    // Shutdown SDL application
    SDL_app_shutdown(&app_test);

    return 0;
}
