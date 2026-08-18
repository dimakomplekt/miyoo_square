// app.h


#pragma once

// =========================================================================================== IMPORT

#include "program_states/program_states.h"

#include <string>

// =========================================================================================== IMPORT


// =========================================================================================== TYPES AND CONSTANTS


/**
 * @brief Global SDL application context.
 *
 * Stores all core runtime data required by the program:
 * - application name
 * - current execution state
 * - SDL window handle
 * - SDL renderer handle
 * - App current state machine
 *
 * This structure acts as the central runtime container
 * for initialization, loop execution and shutdown.
 * 
 */
struct SDL_app_ctx
{
    std::string this_app_name = THIS_APP_NAME;

    SDL_AppResult app_state = SDL_APP_CONTINUE;
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    State_machine app_sm;

};


/**
 * @brief Global application context instance.
 *
 * Provides shared access to the current application state
 * from main loop, states and utility modules.
 * 
 */
extern SDL_app_ctx this_app;

// =========================================================================================== TYPES AND CONSTANTS


// =========================================================================================== APP RENDERER GETTER

/**
 * @brief Returns pointer to the global application renderer.
 *
 * Useful for modules that require rendering access
 * without passing renderer through long call chains.
 *
 * @return Pointer to SDL_Renderer or nullptr if not initialized.
 * 
 */
inline SDL_Renderer* get_app_renderer()
{
    return this_app.renderer;
}

// =========================================================================================== APP RENDERER GETTER


// =========================================================================================== APP LOOP

/**
 * @brief Runs the full SDL application lifecycle.
 *
 * Includes:
 * - SDL/TTF initialization
 * - window and renderer creation
 * - state machine setup
 * - main loop execution
 * - shutdown and cleanup
 *
 * @return 0 on successful exit, negative value on failure.
 * 
 */
int SDL_app_init_and_run();

// =========================================================================================== APP LOOP


// =========================================================================================== APP LOOP INNER FUNCTIONS

// Functions which calls callbacks for current state from state machine


// ===== Initialization =====

// Basic initialization wrapper
bool this_app_init();


/**
 * @brief Initializes SDL_ttf subsystem.
 *
 * Must be called before any font loading or text rendering.
 *
 * @return true if initialization succeeded, false otherwise.
 * 
 */
bool SDL_TTF_init();


/**
 * @brief Initializes SDL_image subsystem.
 *
 * Must be called before any image loading or rendering.
 *
 * @return true if initialization succeeded, false otherwise.
 * 
 */
bool SDL_Image_init();


/**
 * @brief Initializes SDL application context.
 *
 * Creates window, renderer and prepares runtime state.
 *
 * @param app Pointer to application context.
 * @param w Window width in pixels.
 * @param h Window height in pixels.
 * @param title Window title text.
 *
 * @return true if initialization succeeded, false otherwise.
 * 
 */
bool SDL_app_init(SDL_app_ctx* app, int w, int h, const char* title);


// ===== Initialization =====


// ===== MAIN LOOP =====


// Basic loop wrapper
bool this_app_loop();


/**
 * @brief Processes a single SDL event.
 *
 * Handles system events and forwards input/events
 * into the active state machine.
 *
 * @param app Pointer to application context.
 * @param event Pointer to SDL event data.
 * 
 */
void SDL_app_event(SDL_app_ctx* app, SDL_Event* event);


/**
 * @brief Performs one application iteration step.
 *
 * Usually contains update logic and frame rendering.
 *
 * @param app Pointer to application context.
 *
 * @return true if application should continue running.
 * 
 */
bool SDL_app_iterate(SDL_app_ctx* app);


/**
 * @brief Shuts down SDL application.
 *
 * Releases renderer, window and other subsystems.
 *
 * @param app Pointer to application context.
 * 
 */
void SDL_app_shutdown(SDL_app_ctx* app);


/**
 * @brief Executes one full application cycle.
 *
 * Includes:
 * - pending state changes
 * - state update
 * - render clear
 * - state render
 * - present frame
 *
 * @param app Pointer to application context.
 *
 * @return true if application should continue running.
 * 
 */
bool SDL_app_cycle(SDL_app_ctx* app);


// ===== MAIN LOOP =====

// =========================================================================================== APP LOOP INNER FUNCTIONS