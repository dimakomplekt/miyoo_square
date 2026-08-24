/*

// main.cpp

// =========================================================================================== IMPORT

#include <iostream>


// Import of the main libraries
#include "../libs/app/app.h"

// =========================================================================================== IMPORT


// =========================================================================================== MAIN

int main()
{
    return SDL_app_init_and_run();
}

// =========================================================================================== MAIN

*/

#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
    fprintf(stderr, "TEST: program started\n");

    fprintf(stderr, "TEST: calling SDL_Init...\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "TEST: SDL_Init FAILED: %s\n", SDL_GetError());
        return 1;
    }

    fprintf(stderr, "TEST: SDL_Init OK\n");

    fprintf(stderr, "TEST: compiled video drivers:\n");

    int driver_count = SDL_GetNumVideoDrivers();

    for (int i = 0; i < driver_count; ++i)
    {
        fprintf(stderr, "  [%d] %s\n", i, SDL_GetVideoDriver(i));
    }

    fprintf(stderr, "TEST: current video driver: %s\n",
            SDL_GetCurrentVideoDriver());

    fprintf(stderr, "TEST: calling SDL_CreateWindow...\n");

    SDL_Window *window = SDL_CreateWindow(
        "MIYOO TEST",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        fprintf(stderr,
                "TEST: SDL_CreateWindow FAILED: %s\n",
                SDL_GetError());

        SDL_Quit();
        return 1;
    }

    fprintf(stderr, "TEST: SDL_CreateWindow OK\n");

    fprintf(stderr, "TEST: window created, sleeping...\n");

    SDL_Delay(1000);





  fprintf(stderr, "TEST: calling SDL_CreateRenderer...\n");

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer)
    {
        fprintf(stderr,
                "TEST: SDL_CreateRenderer FAILED: %s\n",
                SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    fprintf(stderr, "TEST: SDL_CreateRenderer OK\n");

    fprintf(stderr, "TEST: calling SDL_RenderClear...\n");

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (SDL_RenderClear(renderer) != 0)
    {
        fprintf(stderr,
                "TEST: SDL_RenderClear FAILED: %s\n",
                SDL_GetError());
    }
    else
    {
        fprintf(stderr, "TEST: SDL_RenderClear OK\n");
    }

    fprintf(stderr, "TEST: calling SDL_RenderPresent...\n");

    SDL_RenderPresent(renderer);

    fprintf(stderr, "TEST: SDL_RenderPresent RETURNED\n");

    SDL_Delay(1000);

    fprintf(stderr, "TEST: shutting down\n");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    fprintf(stderr, "TEST: SUCCESS\n");

    return 0;
}