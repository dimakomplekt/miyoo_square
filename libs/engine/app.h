#include "platform.h"
#include "../state_machine/game_states.h"


enum SDL_AppResult {

    SDL_APP_CONTINUE,
    SDL_APP_SUCCESS,
    SDL_APP_FAILURE

};


// Single app
struct sdl_app_ctx {

    SDL_AppResult app_state = SDL_APP_CONTINUE;
    
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    State_machine app_sm;

};

// Functions which calls callbacks for current state from state machine
bool SDL_app_init(sdl_app_ctx* app, int w, int h, const char* title);
void SDL_app_event(sdl_app_ctx* app, SDL_Event* event);
bool SDL_app_iterate(sdl_app_ctx* app);
void SDL_app_shutdown(sdl_app_ctx* app);
bool SDL_app_cycle(sdl_app_ctx* app);