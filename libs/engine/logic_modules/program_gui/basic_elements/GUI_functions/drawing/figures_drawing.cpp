// figures_drawing.cpp

// =========================================================================================== IMPORT

#include "figures_drawing.h"

// =========================================================================================== IMPORT

// TODO Optimize by baking every element into SDL_Texture once 
// on init to avoid per-frame CPU/GPU overhead.

// =========================================================================================== RECTANGLE


void rectangle_borders_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    unsigned int line_width,
    SDL_Color color,

    SDL_Renderer* renderer

)
{
    // Sanity check for dimensions
    if (width < 1 || height < 1 || line_width < 1)
    {
        return;
    }

    // If borders are thicker than the rectangle itself, fallback to a solid rect
    if (line_width >= width || line_width >= height)
    {
        rectangle_draw_by_color(x_render_point, y_render_point, width, height, color, renderer);
        return;
    }

    // Set the drawing color for the renderer (SDL2 uses 0-255 Uint8 channels)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int rect_width  = static_cast<int>(width);
    int rect_height = static_cast<int>(height);
    int lw          = static_cast<int>(line_width);

    // Calculate top-left starting corner coordinates
    int outer_x = x_render_point - rect_width / 2;
    int outer_y = y_render_point - rect_height / 2;
    
    // Optimization: pre-calculate doubled line width to avoid repeating math
    int double_lw = lw * 2;

    // Define 4 float-based rectangles for SDL2
    SDL_FRect rects[4];

    // 1. Upper line
    rects[0].x = static_cast<float>(outer_x);
    rects[0].y = static_cast<float>(outer_y);
    rects[0].w = static_cast<float>(rect_width);
    rects[0].h = static_cast<float>(lw);

    // 2. Downer line
    rects[1].x = static_cast<float>(outer_x);
    rects[1].y = static_cast<float>(outer_y + rect_height - lw);
    rects[1].w = static_cast<float>(rect_width);
    rects[1].h = static_cast<float>(lw);

    // 3. Left line
    rects[2].x = static_cast<float>(outer_x);
    rects[2].y = static_cast<float>(outer_y + lw);
    rects[2].w = static_cast<float>(lw);
    rects[2].h = static_cast<float>(rect_height - double_lw);

    // 4. Right line
    rects[3].x = static_cast<float>(outer_x + rect_width - lw);
    rects[3].y = static_cast<float>(outer_y + lw);
    rects[3].w = static_cast<float>(lw);
    rects[3].h = static_cast<float>(rect_height - double_lw);

    // SDL2 FIX: Call SDL_RenderFillRectsF instead of SDL_RenderFillRects 
    // to correctly process the array of float-based SDL_FRect structures
    SDL_RenderFillRectsF(renderer, rects, 4);
}


void rectangle_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    SDL_Color color,

    SDL_Renderer* renderer

)
{
   // Sanity check for dimensions
    if (width < 1 || height < 1)
    {
        return;
    }

    // Set the drawing color for the renderer (SDL2 uses 0-255 Uint8 channels)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int center_x = x_render_point;
    int center_y = y_render_point;

    int rect_width  = static_cast<int>(width);
    int rect_height = static_cast<int>(height);

    // Define float-based rectangle for SDL2
    SDL_FRect rect;

    // Calculate top-left corner from center point and cast to float
    rect.x = static_cast<float>(center_x - rect_width  / 2);
    rect.y = static_cast<float>(center_y - rect_height / 2);

    rect.w = static_cast<float>(rect_width);
    rect.h = static_cast<float>(rect_height);

    // SDL2 FIX: Call SDL_RenderFillRectF instead of SDL_RenderFillRect
    // to correctly process the float-based SDL_FRect structure
    SDL_RenderFillRectF(renderer, &rect);
}


void rectangle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    SDL_Texture* texture,

    SDL_Renderer* renderer

)
{
    // Sanity check for texture validity and valid dimensions
    if (!texture || width < 1 || height < 1) return;

    // Define float-based rectangle for destination bounds
    SDL_FRect rect;

    rect.w = static_cast<float>(width);
    rect.h = static_cast<float>(height);
    rect.x = static_cast<float>(x_render_point - width / 2);
    rect.y = static_cast<float>(y_render_point - height / 2);

    // SDL2 FIX: Call SDL_RenderCopyF instead of SDL_RenderTexture.
    // In SDL2, the arguments are: renderer, texture, source rect (nullptr for entire texture), destination rect.
    SDL_RenderCopyF(renderer, texture, nullptr, &rect);
}


// =========================================================================================== RECTANGLE



// =========================================================================================== ROUNDED RECTANGLE


#ifdef PLATFORM_WINDOWS

void rounded_rectangle_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    unsigned int radius,

    SDL_Color color,

    SDL_Renderer* renderer

)
{
   // Sanity check for minimum required dimensions and renderer validity
    if (width < 3 || height < 3 || !renderer)
    {
        return;
    }

    // Fallback to a sharp rectangle if radius is zero
    if (radius == 0)
    {
        rectangle_draw_by_color(x_render_point, y_render_point, width, height, color, renderer);
        return;
    }

    // Set the drawing color for the regular rect fills
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int cx = x_render_point;
    int cy = y_render_point;

    int w = static_cast<int>(width);
    int h = static_cast<int>(height);
    int r = static_cast<int>(radius);

    // Ensure the radius does not exceed half of the dimensions
    int max_r = std::min((w - 1) / 2, (h - 1) / 2);
    if (r > max_r) r = max_r;

    int hw = w / 2;
    int hh = h / 2;

    // ========================= CENTER BLOCK

    // SDL2 FIX: Defined float-based rectangles and used SDL_RenderFillRectF instead of SDL_RenderFillRect
    SDL_FRect center_rect{
        (float)(cx - hw + r),
        (float)(cy - hh),
        (float)(w - 2 * r),
        (float)(h)
    };
    SDL_RenderFillRectF(renderer, &center_rect);

    // ========================= SIDE BLOCKS

    SDL_FRect left_rect{
        (float)(cx - hw),
        (float)(cy - hh + r),
        (float)(r),
        (float)(h - 2 * r)
    };
    SDL_RenderFillRectF(renderer, &left_rect);

    SDL_FRect right_rect{
        (float)(cx + hw - r),
        (float)(cy - hh + r),
        (float)(r),
        (float)(h - 2 * r)
    };
    SDL_RenderFillRectF(renderer, &right_rect);

    // ========================= SMOOTH CORNERS via Geometry

    // Calculate segments based on radius size
    const int segments = std::max(8, (int)(r * 10.0f));

    // SDL2 FIX: SDL2 does not have SDL_PI_F, so we define local PI as float
    const float LOCAL_PI_F = 3.14159265f;
    const float step = (LOCAL_PI_F * 0.5f) / segments;

    // SDL2 FIX: SDL_RenderGeometry in SDL2 uses standard SDL_Color (0-255 channels), not SDL_FColor
    SDL_Color geom_color = color;

    // Lambda to generate triangle fans for rounded corner arcs
    auto draw_corner = [&](float center_x, float center_y, float start_angle)
    {
        for (int i = 0; i < segments; ++i)
        {
            float a1 = start_angle + i * step;
            float a2 = start_angle + (i + 1) * step;

            SDL_Vertex verts[3];

            // Center vertex of the fan
            // SDL2 COMPILATION FIX: Explicit assignment to struct members instead of brace-enclosed lists
            verts[0].position.x = center_x;
            verts[0].position.y = center_y;
            verts[0].color = geom_color;

            // First outer arc vertex
            // SDL2 COMPILATION FIX: Explicit assignment to struct members instead of brace-enclosed lists
            verts[1].position.x = center_x + r * cosf(a1);
            verts[1].position.y = center_y + r * sinf(a1);
            verts[1].color = geom_color;

            // Second outer arc vertex
            // SDL2 COMPILATION FIX: Explicit assignment to struct members instead of brace-enclosed lists
            verts[2].position.x = center_x + r * cosf(a2);
            verts[2].position.y = center_y + r * sinf(a2);
            verts[2].color = geom_color;

            // Render the single triangle segment (In SDL2 arguments order is same as SDL3 here)
            SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
        }
    };

    // Draw the 4 corner arcs
    // Left Top (PI)
    draw_corner((float)(cx - hw + r), (float)(cy - hh + r), LOCAL_PI_F);

    // Right Top (-PI * 0.5)
    draw_corner((float)(cx + hw - r), (float)(cy - hh + r), -LOCAL_PI_F * 0.5f);

    // Left Bottom (PI * 0.5)
    draw_corner((float)(cx - hw + r), (float)(cy + hh - r), LOCAL_PI_F * 0.5f);

    // Right Bottom (0.0)
    draw_corner((float)(cx + hw - r), (float)(cy + hh - r), 0.0f);
}

#endif

#ifdef PLATFORM_MIYOO

void rounded_rectangle_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    unsigned int radius,

    SDL_Color color,

    SDL_Renderer* renderer

)
{
    if (width < 3 || height < 3 || !renderer)
    {
        return;
    }

    if (radius == 0)
    {
        rectangle_draw_by_color(
            x_render_point,
            y_render_point,
            width,
            height,
            color,
            renderer
        );

        return;
    }

    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );

    const int cx = x_render_point;
    const int cy = y_render_point;

    const int w = static_cast<int>(width);
    const int h = static_cast<int>(height);

    int r = static_cast<int>(radius);

    const int max_r = std::min(
        (w - 1) / 2,
        (h - 1) / 2
    );

    if (r > max_r)
    {
        r = max_r;
    }

    const int hw = w / 2;
    const int hh = h / 2;

    // ========================================================
    // CENTER
    // ========================================================

    SDL_Rect center_rect{
        cx - hw + r,
        cy - hh,
        w - 2 * r,
        h
    };

    SDL_RenderFillRect(
        renderer,
        &center_rect
    );

    // ========================================================
    // SIDES
    // ========================================================

    SDL_Rect left_rect{
        cx - hw,
        cy - hh + r,
        r,
        h - 2 * r
    };

    SDL_RenderFillRect(
        renderer,
        &left_rect
    );

    SDL_Rect right_rect{
        cx + hw - r,
        cy - hh + r,
        r,
        h - 2 * r
    };

    SDL_RenderFillRect(
        renderer,
        &right_rect
    );

    // ========================================================
    // ROUNDED CORNERS
    // ========================================================

    const int segments = std::max(
        8,
        r * 4
    );

    const float PI = 3.14159265f;

    auto draw_corner =
        [&](int center_x, int center_y, float start_angle)
    {
        for (int i = 0; i < segments; ++i)
        {
            const float a1 =
                start_angle +
                (PI * 0.5f) * i / segments;

            const float a2 =
                start_angle +
                (PI * 0.5f) * (i + 1) / segments;

            const int x1 =
                center_x +
                static_cast<int>(r * cosf(a1));

            const int y1 =
                center_y +
                static_cast<int>(r * sinf(a1));

            const int x2 =
                center_x +
                static_cast<int>(r * cosf(a2));

            const int y2 =
                center_y +
                static_cast<int>(r * sinf(a2));

            // Рисуем дугу
            SDL_RenderDrawLine(
                renderer,
                x1,
                y1,
                x2,
                y2
            );

            // Заполняем область между центром
            // и дугой горизонтальными линиями
            const int min_x =
                std::min(x1, x2);

            const int max_x =
                std::max(x1, x2);

            const int min_y =
                std::min(y1, y2);

            const int max_y =
                std::max(y1, y2);

            for (int y = min_y; y <= max_y; ++y)
            {
                SDL_RenderDrawLine(
                    renderer,
                    center_x,
                    y,
                    min_x,
                    y
                );

                SDL_RenderDrawLine(
                    renderer,
                    center_x,
                    y,
                    max_x,
                    y
                );
            }
        }
    };

    // ========================================================
    // FOUR CORNERS
    // ========================================================

    draw_corner(
        cx - hw + r,
        cy - hh + r,
        PI
    );

    draw_corner(
        cx + hw - r,
        cy - hh + r,
        -PI * 0.5f
    );

    draw_corner(
        cx - hw + r,
        cy + hh - r,
        PI * 0.5f
    );

    draw_corner(
        cx + hw - r,
        cy + hh - r,
        0.0f
    );
}

#endif



void rounded_rectangle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int width,
    unsigned int height,

    unsigned int radius,

    SDL_Texture* texture,

    SDL_Renderer* renderer)
{
     // Sanity check for texture validity and minimum dimensions
    if (!texture || width < 3 || height < 3) return;

    // Fallback: If radius is 0, simply stretch the texture over the entire rectangle
    if (radius == 0)
    {
        // Define float-based destination rectangle
        SDL_FRect rect{
            static_cast<float>(x_render_point - static_cast<int>(width) / 2),
            static_cast<float>(y_render_point - static_cast<int>(height) / 2),
            static_cast<float>(width),
            static_cast<float>(height)
        };

        // SDL2 FIX: Use SDL_RenderCopyF instead of SDL_RenderTexture
        SDL_RenderCopyF(renderer, texture, nullptr, &rect);
        return;
    }

    // ---------------------- Render target creation

    // SDL2 FIX: In SDL2, the texture access flag is SDL_TEXTUREACCESS_TARGET
    SDL_Texture* target = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        static_cast<int>(width),
        static_cast<int>(height)
    );

    if (!target) return;

    // Save old render target to restore it later
    SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

    // Switch rendering to our temporary target texture
    SDL_SetRenderTarget(renderer, target);

    // Clear the new target texture with completely transparent pixels first
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // ---------------------- Render texture by target

    // Copy the original texture to fill the entire temporary target texture
    SDL_FRect full_rect{0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderCopyF(renderer, texture, nullptr, &full_rect);

    // ---------------------- Render the mask of rounded corners
    
    // SDL2 FIX: To physically overwrite pixels with 0 alpha (erase corners), 
    // we must temporarily set the draw blend mode to NONE.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 

    int diameter = static_cast<int>(radius) * 2;
    int r_sq = static_cast<int>(radius * radius);

    // Loop through the corner bounding box to mask out sharp edges
    for (int dx = 0; dx < diameter; dx++)
    {
        for (int dy = 0; dy < diameter; dy++)
        {
            // Mathematical check if the current pixel is outside the rounding arc radius
            if (dx * dx + dy * dy > r_sq)
            {
                // SDL2 FIX: Use SDL_RenderDrawPointF instead of SDL_RenderPoint
                
                // Top left corner
                SDL_RenderDrawPointF(renderer, static_cast<float>(dx), static_cast<float>(dy));

                // Top right corner
                SDL_RenderDrawPointF(renderer, static_cast<float>(width - diameter + dx), static_cast<float>(dy));

                // Bottom left corner
                SDL_RenderDrawPointF(renderer, static_cast<float>(dx), static_cast<float>(height - diameter + dy));

                // Bottom right corner
                SDL_RenderDrawPointF(renderer, static_cast<float>(width - diameter + dx), static_cast<float>(height - diameter + dy));
            }
        }
    }

    // Reset the renderer blend mode back to standard alpha blending
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Restore the main window render target
    SDL_SetRenderTarget(renderer, old_target);

    // ---------------------- Draw the final masked texture to the screen

    SDL_FRect dst{
        static_cast<float>(x_render_point - static_cast<int>(width) / 2),
        static_cast<float>(y_render_point - static_cast<int>(height) / 2),
        static_cast<float>(width),
        static_cast<float>(height)
    };

    // Ensure the generated target texture supports transparency when drawn onto the screen
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
    
    // SDL2 FIX: Use SDL_RenderCopyF to draw the final result
    SDL_RenderCopyF(renderer, target, nullptr, &dst);

    // Clean up the temporary target texture resources to prevent memory leaks
    SDL_DestroyTexture(target);

}

// =========================================================================================== ROUNDED RECTANGLE



// =========================================================================================== CIRCLE


#ifdef PLATFORM_WINDOWS

void circle_draw_by_color(
    
    int x_render_point,
    int y_render_point,

    unsigned int radius,

    SDL_Color color,

    SDL_Renderer* renderer
)
{
    // Sanity check for radius validity and renderer pointer
    if (radius == 0 || !renderer)
    {
        return;
    }

    const float cx = static_cast<float>(x_render_point);
    const float cy = static_cast<float>(y_render_point);
    const float r  = static_cast<float>(radius);

    // Segment quantity calculation based on radius size
    const int segments = std::max(12, static_cast<int>(r * 10.0f));

    // SDL2 FIX: SDL2 does not have SDL_PI_F, so we define local PI as float
    const float LOCAL_PI_F = 3.14159265f;
    const float step = 2.0f * LOCAL_PI_F / segments;

    // SDL2 FIX: SDL2 geometry features use standard SDL_Color (0-255) instead of SDL_FColor
    SDL_Color geom_color = color;

    for (int i = 0; i < segments; ++i)
    {
        float a1 = i * step;
        float a2 = (i + 1) * step;

        float x1 = cx + r * cosf(a1);
        float y1 = cy + r * sinf(a1);

        float x2 = cx + r * cosf(a2);
        float y2 = cy + r * sinf(a2);

        // Define a triangle for the geometry renderer
        SDL_Vertex verts[3];

        // Center vertex of the circle fan
        // SDL2 COMPILATION FIX: Explicit member assignment for strict GCC compliance
        verts[0].position.x = cx;
        verts[0].position.y = cy;
        verts[0].color = geom_color;
        verts[0].tex_coord.x = 0.0f;
        verts[0].tex_coord.y = 0.0f;

        // First edge vertex on the circle perimeter
        // SDL2 COMPILATION FIX: Explicit member assignment for strict GCC compliance
        verts[1].position.x = x1;
        verts[1].position.y = y1;
        verts[1].color = geom_color;
        verts[1].tex_coord.x = 0.0f;
        verts[1].tex_coord.y = 0.0f;

        // Second edge vertex on the circle perimeter
        // SDL2 COMPILATION FIX: Explicit member assignment for strict GCC compliance
        verts[2].position.x = x2;
        verts[2].position.y = y2;
        verts[2].color = geom_color;
        verts[2].tex_coord.x = 0.0f;
        verts[2].tex_coord.y = 0.0f;

        // Render the triangle segment (In SDL2 arguments order is identical to SDL3)
        SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
    }
}

void circle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int radius,

    SDL_Texture* texture,

    SDL_Renderer* renderer

)
{
    // Sanity check for texture validity, non-zero radius, and valid renderer pointer
    if (!texture || radius == 0 || !renderer) return;

    const float cx = static_cast<float>(x_render_point);
    const float cy = static_cast<float>(y_render_point);
    const float r  = static_cast<float>(radius);

    // Segments quantity calculation based on radius size
    const int segments = std::max(12, static_cast<int>(r * 10.0f));

    // SDL2 FIX: SDL2 does not have SDL_PI_F, so we define local PI as float
    const float LOCAL_PI_F = 3.14159265f;
    const float step = 2.0f * LOCAL_PI_F / segments;

    // SDL2 FIX: Vertices in SDL2 geometry use regular SDL_Color (Uint8 channels 0-255), not SDL_FColor
    SDL_Color white_color = {255, 255, 255, 255};

    for (int i = 0; i < segments; ++i)
    {
        float a1 = i * step;
        float a2 = (i + 1) * step;

        float x1 = cx + r * cosf(a1);
        float y1 = cy + r * sinf(a1);

        float x2 = cx + r * cosf(a2);
        float y2 = cy + r * sinf(a2);

        // Define a triangle with texture mapping for the geometry renderer
        SDL_Vertex verts[3];

        // Center vertex mapped to the middle of the texture (0.5, 0.5)
        // SDL2 COMPILATION FIX: Explicit member assignment for strict GCC compliance
        verts[0].position.x = cx;
        verts[0].position.y = cy;
        verts[0].tex_coord.x = 0.5f;
        verts[0].tex_coord.y = 0.5f;
        verts[0].color = white_color;

        // First edge vertex mapped to the perimeter of the texture circle
        // SDL2 COMPILATION FIX: Explicit member assignment for strict GCC compliance
        verts[1].position.x = x1;
        verts[1].position.y = y1;
        verts[1].tex_coord.x = 0.5f + cosf(a1) * 0.5f;
        verts[1].tex_coord.y = 0.5f + sinf(a1) * 0.5f;
        verts[1].color = white_color;

        // Second edge vertex mapped to the perimeter of the texture circle
        // SDL2 COMPILATION FIX: Explicit member assignment for strict GCC compliance
        verts[2].position.x = x2;
        verts[2].position.y = y2;
        verts[2].tex_coord.x = 0.5f + cosf(a2) * 0.5f;
        verts[2].tex_coord.y = 0.5f + sinf(a2) * 0.5f;
        verts[2].color = white_color;

        // Render the textured triangle segment
        SDL_RenderGeometry(renderer, texture, verts, 3, nullptr, 0);
    }
}

#endif


#ifdef PLATFORM_MIYOO

void circle_draw_by_color(

    int x_render_point,
    int y_render_point,

    unsigned int radius,

    SDL_Color color,

    SDL_Renderer* renderer

)
{
    if (radius == 0 || !renderer)
    {
        return;
    }

    const int cx =
        x_render_point;

    const int cy =
        y_render_point;

    const int r =
        static_cast<int>(radius);


    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );


    /*
        ============================================================
        CIRCLE SCANLINE
        ============================================================

        Для каждого Y вычисляем половину ширины круга:

            x^2 + y^2 = r^2

        =>

            x = sqrt(r^2 - y^2)

        После этого просто рисуем горизонтальную линию.
    */

    const int radius_squared =
        r * r;


    for (int y = -r; y <= r; ++y)
    {
        const int y_squared =
            y * y;

        const int x =
            static_cast<int>(
                std::sqrt(
                    static_cast<float>(
                        radius_squared - y_squared
                    )
                )
            );


        SDL_RenderDrawLine(
            renderer,
            cx - x,
            cy + y,
            cx + x,
            cy + y
        );
    }
}


void circle_draw_by_texture(

    int x_render_point,
    int y_render_point,

    unsigned int radius,

    SDL_Texture* texture,

    SDL_Renderer* renderer

)
{
    if (!texture || radius == 0 || !renderer)
    {
        return;
    }


    const int cx =
        x_render_point;

    const int cy =
        y_render_point;

    const int r =
        static_cast<int>(radius);


    /*
        ============================================================
        GET TEXTURE SIZE
        ============================================================
    */

    int texture_width = 0;
    int texture_height = 0;

    if (
        SDL_QueryTexture(
            texture,
            nullptr,
            nullptr,
            &texture_width,
            &texture_height
        ) != 0
    )
    {
        return;
    }


    if (
        texture_width <= 0 ||
        texture_height <= 0
    )
    {
        return;
    }


    /*
        ============================================================
        CIRCLE EQUATION
        ============================================================
    */

    const int diameter =
        r * 2;


    const int radius_squared =
        r * r;


    /*
        ============================================================
        DRAW HORIZONTAL TEXTURE STRIPS
        ============================================================
    */

    for (int y = -r; y <= r; ++y)
    {
        const int y_squared =
            y * y;


        const int half_width =
            static_cast<int>(
                std::sqrt(
                    static_cast<float>(
                        radius_squared - y_squared
                    )
                )
            );


        if (half_width <= 0)
        {
            continue;
        }


        /*
            Destination rectangle.

            Only the part of the horizontal row that belongs
            to the circle is rendered.
        */

        SDL_Rect destination_rect
        {
            cx - half_width,
            cy + y,

            half_width * 2 + 1,
            1
        };


        /*
            Map circle Y coordinate to texture Y coordinate.

            -r ... +r
                ↓
             0 ... texture_height
        */

        const float normalized_y =
            static_cast<float>(y + r) /
            static_cast<float>(diameter);


        int source_y =
            static_cast<int>(
                normalized_y *
                texture_height
            );


        source_y =
            std::max(
                0,
                std::min(
                    texture_height - 1,
                    source_y
                )
            );


        /*
            Take the corresponding horizontal row
            from the texture.
        */

        SDL_Rect source_rect
        {
            0,
            source_y,
            texture_width,
            1
        };


        SDL_RenderCopy(
            renderer,
            texture,
            &source_rect,
            &destination_rect
        );
    }
}

#endif

// =========================================================================================== CIRCLE
