
#include <SDL.h>
#include <stdio.h>

static void print_rect(const char *label, const SDL_Rect *rect)
{
    printf("%s: x=%d y=%d w=%d h=%d\n", label, rect->x, rect->y, rect->w, rect->h);
}

static void draw_quadrant_test(SDL_Renderer *renderer)
{
    const int window_w = 640;
    const int window_h = 480;
    const int margin = 40;
    const int half_w = (window_w - margin * 3) / 2;
    const int half_h = (window_h - margin * 3) / 2;

    SDL_Rect rects[4] = {
        { margin, margin, half_w, half_h },
        { margin * 2 + half_w, margin, half_w, half_h },
        { margin, margin * 2 + half_h, half_w, half_h },
        { margin * 2 + half_w, margin * 2 + half_h, half_w, half_h }
    };

    SDL_Color colors[4] = {
        { 255, 0, 0, 255 },
        { 0, 255, 0, 255 },
        { 0, 0, 255, 255 },
        { 255, 255, 0, 255 }
    };

    printf("Quadrant test: 4 fill rects at 25%% / 75%% positions\n");

    for (int i = 0; i < 4; ++i) {
        print_rect(i == 0 ? "top-left" : i == 1 ? "top-right" : i == 2 ? "bottom-left" : "bottom-right",
                  &rects[i]);

        SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        SDL_RenderFillRect(renderer, &rects[i]);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect center = { window_w / 2 - 12, window_h / 2 - 12, 24, 24 };
    SDL_RenderFillRect(renderer, &center);

    SDL_Rect screen_border = { 0, 0, window_w - 1, window_h - 1 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &screen_border);
}

int main(int argc, char *argv[])
{
    printf("=== SDL MINI QUADRANT TEST ===\n");
    printf("Goal: verify SDL window-space to Miyoo framebuffer-space mapping\n");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init ERROR: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Miyoo Mini Quadrant Test",
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

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        printf("SDL_CreateRenderer ERROR: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) == 0) {
        printf("Renderer name: %s\n", info.name);
        printf("Renderer flags: 0x%08X\n", info.flags);
    }

    printf("Clearing screen to black...\n");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_quadrant_test(renderer);

    printf("Presenting quadrant test frame...\n");
    SDL_RenderPresent(renderer);

    printf("Frame presented. Waiting 5s before exit.\n");
    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("=== SDL MINI QUADRANT TEST END ===\n");
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