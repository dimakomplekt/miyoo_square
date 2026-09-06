// app.cpp

// =========================================================================================== IMPORT

#include "app.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

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


    // TEST
    printf("SDL compiled video drivers:\n");

    int driver_count = SDL_GetNumVideoDrivers();

    for (int i = 0; i < driver_count; ++i)
    {
        printf("  [%d] %s\n", i, SDL_GetVideoDriver(i));
    }


    // APP init (4rth argument by the new string type, translated to the old string type)
    if (!SDL_app_init(&this_app, MAIN_WINDOW_H_SIZE, MAIN_WINDOW_V_SIZE, this_app.this_app_name.c_str())) 
    {
        std::cerr << "Failed to initialize SDL application." << std::endl;
        return false;
    }


    // ===== SDL3 AND SDL2 CONFLICT =====

    // SDL TTF init SDL3
    // if (!SDL_TTF_init()) return false;

    // SDL TTF init SDL2
    if (SDL_TTF_init()) return false;

    // ===== SDL3 AND SDL2 CONFLICT =====

    
    // SDL image init
    
    if (!SDL_Image_init()) return false;


    // ===== CONSOLE SETUP FOR GCC =====

    #ifdef _WIN32

    // Setup Windows consol to UTF-8 symbols
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    #endif

    // ===== CONSOLE SETUP FOR GCC =====


    // Initialize app state machine and states (by initialization function from program_states.cpp)
    init_program_states(this_app.app_sm);

    // Set the initial state to START_ID
    if (!this_app.app_sm.go_to(START_ID))
    {
        std::cerr << "Failed to set initial state to START." << std::endl;
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
    // SDL3 and SDL2 CONFLICT
    if (TTF_Init()) 
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return true;
    }
    else
    {
        SDL_Log("TTF_Init succeeded!");
        return false;
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

        // ===== SDL3 AND SDL2 CONFLICT =====
        app->app_state = false;
        return false;
    }

    // Create an window and renderer

    // SDL2 FIX: SDL_CreateWindowAndRenderer doesn't support custom renderer flags (like VSync).
    // We separate window and renderer creation to explicitly pass the SDL_RENDERER_PRESENTVSYNC flag.

    #ifdef PLATFORM_WINDOWS
    
        app->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED, // Initial X position
            SDL_WINDOWPOS_CENTERED, // Initial Y position
            w,
            h,
            THIS_APP_WINDOW_FLAG    // Your global window flags (now Uint32)
        );

    #elif defined(PLATFORM_MIYOO)

            app->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED, // Initial X position
            SDL_WINDOWPOS_UNDEFINED, // Initial Y position
            w,
            h,
            THIS_APP_WINDOW_FLAG    // Your global window flags (now Uint32)
        );

    #endif

    if (!app->window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        app->app_state = false;
        return false;
    }


    #ifdef PLATFORM_WINDOWS

    // SDL2 FIX: Creating the renderer with accelerated hardware rendering and built-in VSync.
    // Index '-1' automatically picks the first graphics driver that matches the requested flags.
    app->renderer = SDL_CreateRenderer(
        app->window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    #endif

    #ifdef PLATFORM_MIYOO   

    // SDL2 FIX: Creating the renderer with accelerated hardware rendering and built-in VSync.
    // Index '-1' automatically picks the first graphics driver that matches the requested flags.
    app->renderer = SDL_CreateRenderer(
        app->window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    #endif


    if (!app->renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        
        // Clean up the window since renderer initialization failed
        SDL_DestroyWindow(app->window);
        app->window = nullptr;

        app->app_state = false;
        return false;
    }

    // ===== SDL3 AND SDL2 CONFLICT =====
    // SDL_SetRenderVSync(app->renderer, 1);

    #ifdef PLATFORM_WINDOWS
        SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetWindowTitle(app->window, title);
    #endif


    // ===== SDL3 AND SDL2 CONFLICT =====
    
    app->app_state = true;
    return true;
}


// ===== MAIN LOOP =====

bool this_app_loop()
{

    while (this_app.app_state == true)
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
    if (event->type == SDL_QUIT)
    {
        // ===== SDL3 AND SDL2 CONFLICT =====
        app->app_state = false;
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


        // ===== SDL3 AND SDL2 CONFLICT =====

        // state changed -> skip this frame to avoid mixed execution
        return app->app_state == true;
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


        /*
                        SDL
                        │
                        ▼
            SDL_RenderPresent()
                        │
                        ▼
            FlushRenderCommands()
                        │
                        ▼
                Mini backend
                /           \
                /             \
                ▼               ▼
        Mini_QueueCopy    Mini_RenderPresent
                │               │
                ▼               ▼
            GFX_Copy          GFX_Flip
                │               │
                └───────┬───────┘
                        ▼
                framebuffer
                        │
                        ▼
                    Miyoo

        */

        if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
        {
            SDL_RenderClear(app->renderer);

            
            app->app_sm.state_render(app->renderer);


            SDL_RenderPresent(app->renderer);

        }

        /*

            // WHY I DID THIS OUTSIDE OF FPS ZONE???

            // BECAUSE IN SOME PROGRAMS I USE DIFFERENT SPEED OF RENDERING 
            // FOR DIFFERENT PARTS OF THE STATE

            // But maybe it's not the best solution

            app->app_sm.state_render(app->renderer);

            SDL_RenderPresent(app->renderer);

        */
    }

    // ===== SDL3 AND SDL2 CONFLICT =====
    return app->app_state == true;
}


void SDL_app_shutdown(SDL_app_ctx* app)
{
    if (app->renderer) SDL_DestroyRenderer(app->renderer);
    if (app->window) SDL_DestroyWindow(app->window);

    SDL_Quit();
}


// =========================================================================================== APP LOOP INNER FUNCTIONS
