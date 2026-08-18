// app.cpp

// =========================================================================================== IMPORT

#include "app.h"

#include <iostream>

// =========================================================================================== IMPORT


// =========================================================================================== APP MAIN LOOP

SDL_app_ctx this_app;
SDL_Event event;


int SDL_app_init_and_run()
{
    // ===== Initialization =====

    if (!this_app_init()) return -1;


    // ===== Main loop =====


    if(!this_app_loop())
    {
        // Exit app when loop is over
        SDL_app_shutdown(&this_app);

        return 0;
    }

    // Impossible state, but just in case
    return -1;
}

// =========================================================================================== APP MAIN LOOP


// =========================================================================================== APP LOOP INNER FUNCTIONS

// ===== Initialization =====

bool this_app_init()
{
    // Full application initialization wrapper, including SDL and TTF initialization, window and renderer creation,

    // SDL TTF init
    if (!SDL_TTF_init()) return false;

    // SDL image init
    if (!SDL_Image_init()) return false;

    // APP init (4rth argument by the new string type, translated to the old string type)
    if (!SDL_app_init(&this_app, MAIN_WINDOW_H_SIZE, MAIN_WINDOW_V_SIZE, this_app.this_app_name.c_str())) 
    {
        std::cerr << "Failed to initialize SDL application." << std::endl;
        return false;
    }


    // Initialize app state machine and states (by initialization function from program_states.cpp)
    init_program_states(this_app.app_sm);

    // Set the initial state to START_ID
    if (!this_app.app_sm.go_to(START_ID))
    {
        std::cerr << "Failed to set initial state to START_ID." << std::endl;
        SDL_app_shutdown(&this_app);
        return false;
    }
    else
    {
        std::cout << this_app.app_sm.get_current_state()->id.string() << std::endl;
    }

    return true;
}


bool SDL_TTF_init()
{
    if (!TTF_Init()) 
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return false;
    }
    else
    {
        SDL_Log("TTF_Init succeeded!");
        return true;
    }
}


bool SDL_Image_init()
{
    // No actions for SDL3
    return true;
}




bool SDL_app_init(SDL_app_ctx* app, int w, int h, const char* title)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        app->app_state = SDL_APP_FAILURE;
        return false;
    }

    // Create an window and renderer

    if (!SDL_CreateWindowAndRenderer(title, w, h, THIS_APP_WINDOW_FLAG , &app->window, &app->renderer))
    {
        // Error handling
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        app->app_state = SDL_APP_FAILURE;
        return false;
    }

    SDL_SetRenderVSync(app->renderer, 1);

    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    SDL_SetWindowTitle(app->window, title);

    app->app_state = SDL_APP_CONTINUE;
    return true;
}


// ===== MAIN LOOP =====

bool this_app_loop()
{

    while (this_app.app_state == SDL_APP_CONTINUE)
    {
        // Update the timer and check if the execute zones are reached
        App_timer_1.update();
    
        // Pumping events
        SDL_PumpEvents();
    
        // Event polling and handling
        while (SDL_PollEvent(&event))
            SDL_app_event(&this_app, &event);
    
        
        // Main cycle execution (state updates, rendering, state changes)
        SDL_app_cycle(&this_app);
    

        App_timer_1.end_cycle(); // Reset execute permissions for the next cycle
    }

    // On exit
    return false;
}


void SDL_app_event(SDL_app_ctx* app, SDL_Event* event)
{
    // Main SDL events handler
    if (event->type == SDL_EVENT_QUIT)
    {
        app->app_state = SDL_APP_SUCCESS;
        return;
    }

    // Other functions delegation to state machine
    if (app->app_sm.get_current_state()) app->app_sm.state_handle_event(*event);
}


bool SDL_app_cycle(SDL_app_ctx* app)
{
    // State change requests handler
    if (app->app_sm.check_state_change())
    {
        // Perform exit/enter here (with inner state_change.clear() call)
        app->app_sm.go_to(app->app_sm.consume_next_state());

        // state changed -> skip this frame to avoid mixed execution
        return app->app_state == SDL_APP_CONTINUE;
    }

    
    // ===== GLOBAL GUI ELEMENTS UPDATES =====

    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {
        
        // Counter and flag update for language reset in dictionary-oriented textboxes
        App_lang.lang_reset_flag_state_loop_update();

        // Counter and flag update for palette reset in palette-oriented elements
        App_palette.palette_reset_flag_state_loop_update();

        // Font initialization in case of palette reset, to avoid constant reinitialization and related performance issues
        App_fonts.fonts_management_in_update_loop();

        App_fonts.fonts_palette_reset_flag_state_loop_update();

    }
    
    // ===== GLOBAL GUI ELEMENTS UPDATES =====


    // State update
    if (app->app_sm.get_current_state()) app->app_sm.state_update();

    // State rendering
    if (app->app_sm.get_current_state())
    {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);

        if (App_timer_1.can_execute(Execute_zone_ID::HZ_60))
        {
            SDL_RenderClear(app->renderer);
        }

        app->app_sm.state_render(app->renderer);

        SDL_RenderPresent(app->renderer);
    }

    return app->app_state == SDL_APP_CONTINUE;
}


void SDL_app_shutdown(SDL_app_ctx* app)
{
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window) SDL_DestroyWindow(app->window);

    SDL_Quit();
}


// =========================================================================================== APP LOOP INNER FUNCTIONS
