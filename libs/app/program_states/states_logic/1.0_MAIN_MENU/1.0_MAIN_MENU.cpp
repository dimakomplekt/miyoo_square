
// 1.0_MAIN_MENU.cpp


// =========================================================================================== IMPORT

#include "1.0_MAIN_MENU.h"

#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"
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


// Main buttons 

My_SDL_button* Game_button = nullptr;
My_SDL_button* Exit_button = nullptr;


// Info / settings panel

My_SDL_textbox* App_name_textbox = nullptr;

My_SDL_panel* Elements_panel = nullptr;


// =========================================================================================== STATE DATA


// =========================================================================================== HELPERS

const int MENU_ROWS = 2; // Количество строк/элементов

struct controlled_button
{

    int row_number = 1;

};

controlled_button state_1_controlled_button;


void control_selected_button()
{
    switch (state_1_controlled_button.row_number)
    {
        case 1:
        {
            Game_button->set_hovered_state(true);
            Game_button->set_clicked_state(false);

            Exit_button->set_hovered_state(false);
            Exit_button->set_clicked_state(false);

            break;
        }

        
        case 2:
        {
            Game_button->set_hovered_state(false);
            Game_button->set_clicked_state(false);

            Exit_button->set_hovered_state(true);
            Exit_button->set_clicked_state(false);

            break;
        }

        default: break;
    }


    Game_button->drop_clicked_temp();
    Exit_button->drop_clicked_temp();
}


void control_selected_button_clicked(bool clicked)
{
    My_SDL_button* button;


    switch (state_1_controlled_button.row_number)
    {
        case 1:
        {
            button = Game_button;
            break;
        }

        
        case 2:
        {
            button = Exit_button;
            break;
        }

        default: break;
    }


    button->set_clicked_state(clicked);
}


// =========================================================================================== HELPERS


// =========================================================================================== CALLBACKS

void start_game()
{
    // Switch call

    // TEST
    this_app.app_sm.request_state_change(START_ID);
}


void exit_game()
{
    // Exit call

    SDL_app_shutdown(&this_app);
}

// =========================================================================================== CALLBACKS


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


    // Main buttons 

    Game_button = new My_SDL_button;
    Exit_button = new My_SDL_button;


    // Info / settings panel

    App_name_textbox = new My_SDL_textbox;

    Elements_panel = new My_SDL_panel;

}


// ===== SETUP DATA =====

// ===== Main sizes =====

// Static-size GUI

const int BACKGROUND_WIDTH  = MAIN_WINDOW_H_SIZE;
const int BACKGROUND_HEIGHT  = MAIN_WINDOW_V_SIZE;

int margin_size_1 = 25;

int panel_2_width = BACKGROUND_WIDTH * 0.6 - margin_size_1 * 1.5;
int panel_2_height = BACKGROUND_HEIGHT - 2 * margin_size_1;

int button_width = BACKGROUND_WIDTH * 0.4 - 1.5 * margin_size_1;
int button_height = (BACKGROUND_HEIGHT - 3 * margin_size_1) / 2;

// ====== Main points ======

int main_menu_background_x = MAIN_WINDOW_H_SIZE * 0.5;
int main_menu_background_y = MAIN_WINDOW_V_SIZE * 0.5;

int panel_2_x = BACKGROUND_WIDTH - margin_size_1 - panel_2_width * 0.5;
int panel_2_y = BACKGROUND_HEIGHT / 2; 


// ===== SETUP DATA =====

void main_menu_elements_setup()
{     
    // ===== Setup =====

    // Background setup

    Main_menu_panel->set_render_point(main_menu_background_x, main_menu_background_y);
    Main_menu_panel->set_size(BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
    Main_menu_panel->set_border_radius(0);


    Game_button->switch_button_textbox_type(ORDINARY_TEXT);
    Game_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_play));
    Game_button->set_size(button_width, button_height);
    Game_button->set_render_point(margin_size_1 + 0.5 * button_width, margin_size_1 + 0.5 * button_height);
    Game_button->on_click = start_game;
    Game_button->set_border_radius(15);
    Game_button->switch_outer_control_flag(true);
    Game_button->set_shadow_scale_factor(1);
    Game_button->set_shadow_offset(0, 0);

    Exit_button->switch_button_textbox_type(ORDINARY_TEXT);
    Exit_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_exit));
    Exit_button->set_size(button_width, button_height);
    Exit_button->set_render_point(margin_size_1 + 0.5 * button_width, 2 * margin_size_1 + 1.5 * button_height);
    Exit_button->on_click = exit_game;
    Exit_button->set_border_radius(15);
    Exit_button->switch_outer_control_flag(true);
    Exit_button->set_shadow_scale_factor(1);
    Exit_button->set_shadow_offset(0, 0);

    // Init value
    state_1_controlled_button.row_number = 1;
    control_selected_button();


    App_name_textbox->switch_textbox_type(HEADER_3);
    App_name_textbox->set_content(THIS_APP_NAME);


    Elements_panel->set_render_point(panel_2_x , panel_2_y);
    Elements_panel->set_size(panel_2_width, panel_2_height);
    Elements_panel->set_border_radius(15);
    Elements_panel->set_shadow_scale_factor(1);
    Elements_panel->set_shadow_offset(0, 0);

    Elements_panel->add_element(

        App_name_textbox,
        panel_2_width * 0.5,
        panel_2_height * 0.5,
        1

    );

}


void main_menu_elements_free_and_nullptr()
{
    // Protection
    if (Main_menu_panel == nullptr) return;
    
    // Free all elements
    // Background
    Main_menu_panel->delete_element();

    // Main buttons 

    Game_button->delete_element();
    Exit_button->delete_element();


    // Info / settings panel

    Elements_panel->delete_element();


    // Nullptr the pointers

    // Background
    Main_menu_panel = nullptr;


    Game_button = nullptr;
    Exit_button = nullptr;


    // Info / settings panel

    App_name_textbox = nullptr;

    Elements_panel = nullptr;

}


void main_menu_elements_update()
{

    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched_mm();


    // Update all elements

    // Background
    Main_menu_panel->update();

    
    Game_button->update();
    Exit_button->update();

    Elements_panel->update();

}


void reset_passed_by_dictionary_textboxes_if_language_switched_mm()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        Game_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_play));

        Exit_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_exit));
    }
}


void main_menu_actions()
{
    // Check switch between buttons

    if (App_inputs.is_just_released(Key_actions::UP_KA))
    {
        state_1_controlled_button.row_number = (state_1_controlled_button.row_number + MENU_ROWS - 2) % MENU_ROWS + 1;
    
        control_selected_button();
    }

    if (App_inputs.is_just_released(Key_actions::DOWN_KA))
    {
        state_1_controlled_button.row_number = (state_1_controlled_button.row_number % MENU_ROWS) + 1;
    
        control_selected_button();
    }


    // Check click
    if (App_inputs.is_held(Key_actions::A_KA) || App_inputs.is_held(Key_actions::SELECT_KA))
    {
        control_selected_button_clicked(true);
    }

    if (App_inputs.is_just_released(Key_actions::A_KA) || App_inputs.is_just_released(Key_actions::SELECT_KA))
    {
        control_selected_button_clicked(false);
    }


}


void main_menu_elements_render(SDL_Renderer* renderer)
{
    line_draw
    (
        1, 300,
        600, 300,
        5,
        hex_to_sdl_color("#1a36d6", 255),
        renderer
    );


    Main_menu_panel->render(renderer);

    Game_button->render(renderer);
    Exit_button->render(renderer);

    Elements_panel->render(renderer);
}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS
