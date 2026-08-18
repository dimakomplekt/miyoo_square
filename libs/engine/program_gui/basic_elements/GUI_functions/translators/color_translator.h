// color_translator.h

#pragma once

// =========================================================================================== IMPORT

#include "../../../../engine/engine.h"

// =========================================================================================== IMPORT


/**
 * @brief Converts a HEX color string to SDL_Color with specified opacity.
 *
 * Parses a color in HEX format (e.g. "#RRGGBB" or "RRGGBB") and converts it
 * into an SDL_Color structure. The alpha channel is set using the provided
 * opacity value.
 *
 * @param hex String containing HEX color representation.
 * @param opacity Alpha value (0–255) to apply to the resulting color.
 *
 * @return SDL_Color Resulting color with RGBA components.
 */
SDL_Color hex_to_sdl_color(const std::string& hex, Uint8 opacity);