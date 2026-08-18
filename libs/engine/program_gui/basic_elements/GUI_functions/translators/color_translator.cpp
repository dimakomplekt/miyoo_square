// color_translator.cpp


// =========================================================================================== IMPORT

#include "color_translator.h"

// =========================================================================================== IMPORT


SDL_Color hex_to_sdl_color(const std::string& hex, Uint8 opacity)
{
    SDL_Color color;

    color.r = std::stoi(hex.substr(1, 2), nullptr, 16);
    color.g = std::stoi(hex.substr(3, 2), nullptr, 16);
    color.b = std::stoi(hex.substr(5, 2), nullptr, 16);
    color.a = opacity;

    return color;
}
