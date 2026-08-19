// 0.0_START.cpp


// =========================================================================================== IMPORT

#include "0.0_START.h"


#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"

#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"


// Predeclare for switching states
#include "../../program_states.h"

#include "../../../app.h"

// Log
#include <iostream>

// =========================================================================================== IMPORT


// =========================================================================================== STATE DATA

// RAII + lifecycle management

My_SDL_panel* Start_panel = nullptr;

My_SDL_textbox* Start_textbox = nullptr;

// =========================================================================================== STATE DATA


// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION

void start_elements_create();

void start_elements_setup();

void start_elements_free_and_nullptr();

void start_elements_update();

void reset_passed_by_dictionary_textboxes_if_language_switched();

void start_actions();

void start_elements_render(SDL_Renderer* renderer);

// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


// =========================================================================================== MAIN STATE API


void start_enter()
{
    // Log the enter in console
    std::cout << "Entering START\n"; 

    // ===== State allocation =====

    start_elements_create();

    // ===== State allocation =====


    // Elements setup

    start_elements_setup();

}



void start_exit()
{
    // ===== State deallocation =====

    start_elements_free_and_nullptr();

    // ===== State deallocation =====


    // Log the exit in console

    std::cout << "Exiting START\n"; 

}


void start_update()
{
    // Update inputs
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {
        App_inputs.update();
    }

    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {

        start_elements_update();

        start_actions();

    }
}


void start_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        start_elements_render(renderer);
    }
}




// =========================================================================================== MAIN STATE API




// =========================================================================================== INNER STATE FUNCTIONS


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void start_elements_create()
{
    // Start panel create
    Start_panel = new My_SDL_panel();

    // Start textbox create
    Start_textbox = new My_SDL_textbox();


}


void start_elements_setup()
{
    // Start panel setup

    Start_panel->set_render_point(MAIN_WINDOW_H_SIZE / 2, MAIN_WINDOW_V_SIZE / 2);
    Start_panel->set_size(MAIN_WINDOW_H_SIZE, MAIN_WINDOW_V_SIZE);
    Start_panel->set_border_radius(0);

    // Start textbox setup
    Start_textbox->switch_textbox_type(HEADER_2);

    Start_textbox->set_content(str_by_dictionary(gd_press_any_key));

    Start_textbox->switch_blinking_mode_flag(true);


    // Put the texture inside the middle of the panel

    Start_panel->add_element(

        Start_textbox,
         (Start_panel->get_width_size()) * 0.5,
          (Start_panel->get_height_size()) * 0.5,
           1
    );


}


void start_elements_free_and_nullptr()
{
    // Free all elements

    Start_panel->delete_element();

    // Nullptr the pointers

    Start_panel = nullptr;
    Start_textbox = nullptr;
}


void start_elements_update()
{
    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched();

    // Update all elements
    Start_panel->update();

}


void reset_passed_by_dictionary_textboxes_if_language_switched()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        Start_textbox->set_content(str_by_dictionary(gd_press_any_key));
    }
}


void start_actions()
{
    // Switch the state to EXIT if EXIT pressed

    if (App_inputs.is_just_released(Key_actions::EXIT))
    {
        this_app.app_sm.request_state_change(MASKS_SETUP_ID);
    }

    // Switch the state to MAIN MENU if ENTER pressed

    if (App_inputs.is_just_released(Key_actions::ENTER))
    {
        this_app.app_sm.request_state_change(MAIN_MENU_ID);
    }

    if (App_inputs.is_just_released(Key_actions::SPECIAL_1))
    {
        App_palette.switch_to_the_next_palette();
        App_lang.switch_to_next_lang();
        App_fonts.switch_to_the_next_font_palette();
    
        static bool to_second = false;
    
        if (!to_second)
        {
            int old_x = Start_textbox->get_x_render_point();
            int old_y = Start_textbox->get_y_render_point();
    
            // Start_panel->move_to_point(old_x - 100, old_y - 200, EXPONENTIAL, 500);
            Start_textbox->move_to_point(old_x + 100, old_y + 200, LINEAR, 300);

            to_second = true;
        }
        else
        {
            int old_x = Start_textbox->get_x_render_point();
            int old_y = Start_textbox->get_y_render_point();

            // Start_panel->move_to_point(old_x + 100, old_y + 200, EXPONENTIAL, 500);
            Start_textbox->move_to_point(old_x - 100, old_y - 200, LINEAR, 300);

            to_second = false;
        }
    }
}


void start_elements_render(SDL_Renderer* renderer)
{
    // Render all elements
    Start_panel->render(renderer);

}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION
