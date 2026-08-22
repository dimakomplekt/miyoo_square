// 1.0_MAIN_MENU.cpp


// =========================================================================================== IMPORT

#include "1.0_MAIN_MENU.h"


#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"
#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"
#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_texture/my_sdl_texture.h"





// Predeclare for switching states
#include "../../program_states.h"

#include "../../../app.h"

// Log
#include <iostream>


#include <vector>
#include <algorithm>
#include <cmath>

// =========================================================================================== IMPORT


// =========================================================================================== STATE DATA

// RAII + lifecycle management

// Background
My_SDL_panel* Main_menu_panel = nullptr;


// =========================================================================================== STATE DATA


// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION

void main_menu_elements_create();

void main_menu_elements_setup();

void main_menu_elements_free_and_nullptr();

void main_menu_elements_update();

void reset_passed_by_dictionary_textboxes_if_language_switched_mm();

void main_menu_actions();

void main_menu_elements_render(SDL_Renderer* renderer);

// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


// =========================================================================================== MAIN STATE API

void main_menu_enter()
{
    // Log the enter in console
    std::cout << "Entering MAIN_MENU\n"; 

    // ===== State allocation =====

    main_menu_elements_create();

    // ===== State allocation =====


    // Elements setup

    main_menu_elements_setup();

}


void main_menu_exit()
{
    // ===== State deallocation =====

    main_menu_elements_free_and_nullptr();

    // ===== State deallocation =====


    // Log the exit in console

    std::cout << "Exiting MAIN_MENU\n"; 

}


void main_menu_update()
{
    // Update inputs
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {
        App_inputs.update();
    }

    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {
        main_menu_elements_update();

        main_menu_actions();
    }
}


void main_menu_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        main_menu_elements_render(renderer);
    }
}

// =========================================================================================== MAIN STATE API


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void main_menu_elements_create()
{
    // Background
    Main_menu_panel = new My_SDL_panel();
}


// ===== SETUP DATA =====

// ===== Main sizes =====

// Static-size GUI

const int BACKGROUND_WIDTH  = MAIN_WINDOW_H_SIZE;
const int BACKGROUND_HEIGHT  = MAIN_WINDOW_V_SIZE;

// ====== Main points ======

int main_menu_background_x = MAIN_WINDOW_H_SIZE * 0.5;
int main_menu_background_y = MAIN_WINDOW_V_SIZE * 0.5;



// ===== TESTING RECTANGLE =====

struct test_rectangle
{
    int x_rp;
    int y_rp;
    int width;
    int height;

    int angle;

    int border_thickness;

    SDL_Color fill_color;
    SDL_Color border_color;
    
};


std::vector<SDL_Color> border_colors =
{
    hex_to_sdl_color("#0808d5", 254),
    hex_to_sdl_color("#babad0", 254),
    hex_to_sdl_color("#090926", 254),
    hex_to_sdl_color("#20d90b", 254)

};

std::vector<SDL_Color> fill_colors =
{
    hex_to_sdl_color("#1bd79f", 254),
    hex_to_sdl_color("#02020e", 254),
    hex_to_sdl_color("#e40a2e", 254),
    hex_to_sdl_color("#c52ee3", 254)

};


test_rectangle my_rectangle;

void rectangle_init()
{
    my_rectangle.x_rp = BACKGROUND_WIDTH / 2;
    my_rectangle.y_rp = BACKGROUND_HEIGHT / 2;

    my_rectangle.width = 200;
    my_rectangle.height = 200;

    my_rectangle.angle = 0;

    my_rectangle.border_thickness = 5;

    my_rectangle.border_color = border_colors[0];
    my_rectangle.fill_color = fill_colors[0];
}


void rectangle_render(SDL_Renderer* renderer)
{
    if (!renderer)
        return;

    const float cx = static_cast<float>(my_rectangle.x_rp);
    const float cy = static_cast<float>(my_rectangle.y_rp);

    const float half_width = my_rectangle.width * 0.5f;
    const float half_height = my_rectangle.height * 0.5f;

    const float border = static_cast<float>(my_rectangle.border_thickness);

    /*
        Внешний прямоугольник
    */

    const float outer_half_width = half_width;
    const float outer_half_height = half_height;

    /*
        Внутренний прямоугольник.

        Не даём border уничтожить прямоугольник.
    */

    const float inner_half_width =
        std::max(0.0f, half_width - border);

    const float inner_half_height =
        std::max(0.0f, half_height - border);


    /*
        Функция вращения точки вокруг центра.
    */

    auto rotate_point =
        [&](float x, float y) -> SDL_FPoint
        {
            const float angle_rad =
                my_rectangle.angle * static_cast<float>(M_PI) / 180.0f;

            const float cos_a = std::cos(angle_rad);
            const float sin_a = std::sin(angle_rad);

            return
            {
                cx + x * cos_a - y * sin_a,
                cy + x * sin_a + y * cos_a
            };
        };


    /*
        ============================================================
        OUTER RECTANGLE
        ============================================================
    */

    SDL_FPoint outer_points[4] =
    {
        rotate_point(-outer_half_width, -outer_half_height),
        rotate_point( outer_half_width, -outer_half_height),
        rotate_point( outer_half_width,  outer_half_height),
        rotate_point(-outer_half_width,  outer_half_height)
    };


    /*
        ============================================================
        BORDER
        ============================================================

        Рисуем четыре полосы как отдельные квадраты.
        Благодаря этому border тоже вращается вместе с объектом.
    */

    SDL_Vertex border_vertices[16];

    auto set_vertex =
        [&](SDL_Vertex& vertex,
            const SDL_FPoint& point,
            const SDL_Color& color)
        {
            vertex.position = point;
            vertex.color = color;
            vertex.tex_coord = { 0.0f, 0.0f };
        };


    /*
        Верхняя граница
    */

    SDL_FPoint top[4] =
    {
        rotate_point(-outer_half_width, -outer_half_height),
        rotate_point( outer_half_width, -outer_half_height),
        rotate_point( outer_half_width, -inner_half_height),
        rotate_point(-outer_half_width, -inner_half_height)
    };

    /*
        Правая граница
    */

    SDL_FPoint right[4] =
    {
        rotate_point(inner_half_width, -inner_half_height),
        rotate_point(outer_half_width, -outer_half_height),
        rotate_point(outer_half_width, outer_half_height),
        rotate_point(inner_half_width, inner_half_height)
    };

    /*
        Нижняя граница
    */

    SDL_FPoint bottom[4] =
    {
        rotate_point(-outer_half_width, inner_half_height),
        rotate_point(outer_half_width, inner_half_height),
        rotate_point(outer_half_width, outer_half_height),
        rotate_point(-outer_half_width, outer_half_height)
    };

    /*
        Левая граница
    */

    SDL_FPoint left[4] =
    {
        rotate_point(-outer_half_width, -outer_half_height),
        rotate_point(-inner_half_width, -inner_half_height),
        rotate_point(-inner_half_width, inner_half_height),
        rotate_point(-outer_half_width, outer_half_height)
    };


    /*
        Заполняем вершины border.
    */

    for (int i = 0; i < 4; i++)
    {
        set_vertex(border_vertices[i],      top[i],    my_rectangle.border_color);
        set_vertex(border_vertices[i + 4],  right[i],  my_rectangle.border_color);
        set_vertex(border_vertices[i + 8],  bottom[i], my_rectangle.border_color);
        set_vertex(border_vertices[i + 12], left[i],  my_rectangle.border_color);
    }


    int border_indices[] =
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15
    };


    SDL_RenderGeometry(
        renderer,
        nullptr,
        border_vertices,
        16,
        border_indices,
        24
    );


    /*
        ============================================================
        FILL
        ============================================================
    */

    SDL_FPoint inner_points[4] =
    {
        rotate_point(-inner_half_width, -inner_half_height),
        rotate_point( inner_half_width, -inner_half_height),
        rotate_point( inner_half_width,  inner_half_height),
        rotate_point(-inner_half_width,  inner_half_height)
    };


    SDL_Vertex fill_vertices[4];

    for (int i = 0; i < 4; i++)
    {
        set_vertex(
            fill_vertices[i],
            inner_points[i],
            my_rectangle.fill_color
        );
    }


    int fill_indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };


    SDL_RenderGeometry(
        renderer,
        nullptr,
        fill_vertices,
        4,
        fill_indices,
        6
    );
}


void rectangle_move_left()
{
    my_rectangle.x_rp -= 5;
}


void rectangle_move_up()
{
    my_rectangle.y_rp -= 5;
}


void rectangle_move_right()
{
    my_rectangle.x_rp += 5;
}


void rectangle_move_down()
{
    my_rectangle.y_rp += 5;
}


void rectangle_change_width(int new_width)
{
    const int min_width = my_rectangle.border_thickness * 2 + 1;

    if (new_width < min_width)
        new_width = min_width;

    my_rectangle.width = new_width;
}


void rectangle_change_height(int new_height)
{
    const int min_height = my_rectangle.border_thickness * 2 + 1;

    if (new_height < min_height)
        new_height = min_height;

    my_rectangle.height = new_height;
}


void rectangle_rotate(int angle)
{
    my_rectangle.angle += angle;

    /*
        Не даём углу бесконечно расти.
    */

    my_rectangle.angle %= 360;

    if (my_rectangle.angle < 0)
        my_rectangle.angle += 360;
}


void rectangle_change_border_thickness(int new_width)
{

    const int max_border =
        std::min(
            my_rectangle.width,
            my_rectangle.height
        ) / 2;

    my_rectangle.border_thickness = new_width;

    if (my_rectangle.border_thickness > max_border)
        my_rectangle.border_thickness = max_border;
}


void rectangle_change_border_color()
{
    static size_t color_index = 0;

    color_index++;

    if (color_index >= border_colors.size())
        color_index = 0;

    my_rectangle.border_color =
        border_colors[color_index];
}


void rectangle_change_fill_color()
{
    static size_t color_index = 0;

    color_index++;

    if (color_index >= fill_colors.size())
        color_index = 0;

    my_rectangle.fill_color =
        fill_colors[color_index];
}
// ===== TESTING RECTANGLE =====




// ===== SETUP DATA =====

void main_menu_elements_setup()
{     
    // ===== Setup =====

    // Background setup

    Main_menu_panel->set_render_point(main_menu_background_x, main_menu_background_y);
    Main_menu_panel->set_size(BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
    Main_menu_panel->set_border_radius(0);

    rectangle_init();
}


void main_menu_elements_free_and_nullptr()
{
    // Protection
    if (Main_menu_panel == nullptr) return;
    
    // Free all elements
    // Background
    Main_menu_panel->delete_element();



    // Nullptr the pointers

    // Background
    Main_menu_panel = nullptr;

}


void main_menu_elements_update()
{
    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched_mm();


    // Update all elements

    // Background
    Main_menu_panel->update();

}


void reset_passed_by_dictionary_textboxes_if_language_switched_mm()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        //
    }
}


void main_menu_actions()
{
    // Switch the state to EXIT if EXIT pressed

    if (App_inputs.is_just_released(Key_actions::EXIT_KA))
    {
        this_app.app_sm.request_state_change(START_ID);
    }

    // Switch the state to MAIN MENU if ENTER pressed

    if (App_inputs.is_just_released(Key_actions::START_KA))
    {
        // Change color
        rectangle_change_border_color();
    }

    if (App_inputs.is_just_released(Key_actions::SELECT_KA))
    {
        // App_palette.switch_to_the_next_palette();
        // App_fonts.switch_to_the_next_font_palette();
        // App_lang.switch_to_next_lang();
        rectangle_change_fill_color();
    }


    if (App_inputs.is_just_released(Key_actions::LEFT_KA))
    {
        rectangle_move_left();
    }

    
    if (App_inputs.is_just_released(Key_actions::UP_KA))
    {
        rectangle_move_up();
    }

    
    if (App_inputs.is_just_released(Key_actions::RIGHT_KA))
    {
        rectangle_move_right();
    }

    
    if (App_inputs.is_just_released(Key_actions::DOWN_KA))
    {
        rectangle_move_down();
    }


    if (App_inputs.is_just_released(Key_actions::Y_KA))
    {
        rectangle_change_width(my_rectangle.width - 10);
    }   

    
    if (App_inputs.is_just_released(Key_actions::X_KA))
    {
        rectangle_change_height(my_rectangle.height + 10);
    }

    
    if (App_inputs.is_just_released(Key_actions::A_KA))
    {
        rectangle_change_width(my_rectangle.width + 10);
    }

    
    if (App_inputs.is_just_released(Key_actions::B_KA))
    {
        rectangle_change_height(my_rectangle.height - 10);
    }

    if (App_inputs.is_just_released(Key_actions::L_1_KA))
    {
        rectangle_rotate(my_rectangle.angle - 5);
    }

    if (App_inputs.is_just_released(Key_actions::R_1_KA))
    {
        rectangle_rotate(my_rectangle.angle + 5);
    }


    if (App_inputs.is_just_released(Key_actions::L_2_KA))
    {
        rectangle_change_border_thickness(my_rectangle.border_thickness - 1);
    }

    if (App_inputs.is_just_released(Key_actions::R_2_KA))
    {
        rectangle_change_border_thickness(my_rectangle.border_thickness + 1);
    }

}


void main_menu_elements_render(SDL_Renderer* renderer)
{
    // Render all elements

    // Update all elements
    
    // Background
    Main_menu_panel->render(renderer);

    // Test rectangle render
    rectangle_render(renderer);

}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS
