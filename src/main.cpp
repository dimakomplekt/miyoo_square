
#include <SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("=== SDL MINI TEST ===\n");

    printf("SDL_Init...\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init ERROR: %s\n", SDL_GetError());
        return 1;
    }

    printf("SDL_Init OK\n");

    SDL_Window *window = SDL_CreateWindow(
        "Miyoo Mini Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    if (!window) {
        printf("SDL_CreateWindow ERROR: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    printf("Window created: 640x480\n");

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        printf("SDL_CreateRenderer ERROR: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("Renderer created\n");

    SDL_RendererInfo info;

    if (SDL_GetRendererInfo(renderer, &info) == 0) {
        printf("Renderer name: %s\n", info.name);
        printf("Renderer flags: 0x%08X\n", info.flags);
        printf("Max texture: %dx%d\n",
               info.max_texture_width,
               info.max_texture_height);
    }

    printf("Clearing screen...\n");

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    printf("Present...\n");


    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect rect = {
        100,
        100,
        200,
        100
    };

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

    printf("Frame presented\n");

    SDL_Delay(3000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("=== TEST END ===\n");

    return 0;
}



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