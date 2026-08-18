// figures_drawing.h

#pragma once

// =========================================================================================== IMPORT

#include "../../../../engine/engine.h"     // SDL3 and SDL ttf import

#include <algorithm>                       // For std::min()

// =========================================================================================== IMPORT


enum element_form
{

    RECTANGLE_EF,       
    ROUNDED_RECTANGLE_EF,
    CIRCLE_EF       

};


/**
 * @brief Renders a filled rectangle using a solid color.
 *
 * Draws a rectangle centered at the given render point using the specified width,
 * height and color.
 *
 * @param x_render_point X coordinate of rectangle center.
 * @param y_render_point Y coordinate of rectangle center.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param color Fill color.
 * @param renderer SDL renderer used for drawing.
 * 
 */
void rectangle_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    SDL_Color color,

    SDL_Renderer* renderer

);


/**
 * @brief Renders a textured rectangle.
 *
 * Draws a rectangle centered at the given render point and maps the provided
 * texture onto it.
 *
 * @param x_render_point X coordinate of rectangle center.
 * @param y_render_point Y coordinate of rectangle center.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param texture SDL texture to render.
 * @param renderer SDL renderer used for drawing.
 * 
 */
void rectangle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    SDL_Texture* texture,

    SDL_Renderer* renderer

);


/**
 * @brief Renders a filled rounded rectangle using a solid color.
 *
 * Draws a rectangle with rounded corners centered at the given render point.
 * If radius is zero, falls back to regular rectangle rendering.
 *
 * @param x_render_point X coordinate of rectangle center.
 * @param y_render_point Y coordinate of rectangle center.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param radius Corner radius.
 * @param color Fill color.
 * @param renderer SDL renderer used for drawing.
 * 
 */
void rounded_rectangle_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    unsigned int radius,

    SDL_Color color,

    SDL_Renderer* renderer

);


/**
 * @brief Renders a filled rounded rectangle using a solid color.
 *
 * Draws a rectangle with rounded corners centered at the given render point.
 * If radius is zero, falls back to regular rectangle rendering.
 *
 * @param x_render_point X coordinate of rectangle center.
 * @param y_render_point Y coordinate of rectangle center.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param radius Corner radius.
 * @param color Fill color.
 * @param renderer SDL renderer used for drawing.
 * 
 */
void rounded_rectangle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    unsigned int radius,

    SDL_Texture* texture,

    SDL_Renderer* renderer
);


/**
 * @brief Renders a filled circle using a solid color.
 *
 * Draws a circle centered at the given render point using triangle fan geometry.
 *
 * @param x_render_point X coordinate of circle center.
 * @param y_render_point Y coordinate of circle center.
 * @param radius Circle radius.
 * @param color Fill color.
 * @param renderer SDL renderer used for drawing.
 * 
 */
void circle_draw_by_color(

    int x_render_point,
    int y_render_point,

    unsigned int radius,

    SDL_Color color,

    SDL_Renderer* renderer
    
);


/**
 * @brief Renders a textured circle.
 *
 * Draws a circle centered at the given render point and maps the texture onto it
 * using polar texture coordinates.
 *
 * @param x_render_point X coordinate of circle center.
 * @param y_render_point Y coordinate of circle center.
 * @param radius Circle radius.
 * @param texture SDL texture to render.
 * @param renderer SDL renderer used for drawing.
 * 
 */
void circle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int radius,

    SDL_Texture* texture,

    SDL_Renderer* renderer

);
