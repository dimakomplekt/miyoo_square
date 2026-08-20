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

const int FIRST_ZONE_WIDTH = static_cast<int>(MAIN_WINDOW_H_SIZE * 0.4);
const int SECOND_ZONE_WIDTH = MAIN_WINDOW_H_SIZE - FIRST_ZONE_WIDTH;

const int SCREEN_MARGIN_1 = 50;
const int SCREEN_MARGIN_2 = 50;

int mm_buttons_h_size = FIRST_ZONE_WIDTH - 2 * SCREEN_MARGIN_1;
int mm_buttons_v_size = static_cast<int>((MAIN_WINDOW_V_SIZE - 5 * SCREEN_MARGIN_1) / 4);

int mm_panels_h_size = SECOND_ZONE_WIDTH - 2 * SCREEN_MARGIN_2;
int mm_panels_v_size = static_cast<int>((MAIN_WINDOW_V_SIZE - 3 * SCREEN_MARGIN_1) * 0.5);

int mini_buttons_h_size = (mm_panels_h_size - 3 * SCREEN_MARGIN_1) * 0.5;
int mini_buttons_v_size = (mm_panels_v_size - 3 * SCREEN_MARGIN_1) * 0.5;

// ====== Main points ======

int main_menu_background_x = MAIN_WINDOW_H_SIZE * 0.5;
int main_menu_background_y = MAIN_WINDOW_V_SIZE * 0.5;

int analysis_start_x = FIRST_ZONE_WIDTH * 0.5;
int analysis_start_y = (SCREEN_MARGIN_1 + mm_buttons_v_size * 0.5) + 0*(mm_buttons_v_size + SCREEN_MARGIN_1);

int information_x = analysis_start_x;
int information_y = (SCREEN_MARGIN_1 + mm_buttons_v_size * 0.5) + 1*(mm_buttons_v_size + SCREEN_MARGIN_1);

int settings_x = analysis_start_x;
int settings_y = (SCREEN_MARGIN_1 + mm_buttons_v_size * 0.5) + 2*(mm_buttons_v_size + SCREEN_MARGIN_1);

int exit_x = analysis_start_x;
int exit_y = (SCREEN_MARGIN_1 + mm_buttons_v_size * 0.5) + 3*(mm_buttons_v_size + SCREEN_MARGIN_1);


// PSMVA textbox static position

int psmva_textbox_x = MAIN_WINDOW_H_SIZE - SECOND_ZONE_WIDTH * 0.5;
int psmva_textbox_y = (SCREEN_MARGIN_1 + 0.5 * mm_panels_v_size);

// Upper panel dynamic positions

int dynamic_panel_x_1 = 2 * (MAIN_WINDOW_H_SIZE - SECOND_ZONE_WIDTH * 0.5);
int dynamic_panel_y_1 = (SCREEN_MARGIN_1 + 0.5 * mm_panels_v_size);

int dynamic_panel_x_2 = MAIN_WINDOW_H_SIZE - SECOND_ZONE_WIDTH * 0.5;
int dynamic_panel_y_2 = (SCREEN_MARGIN_1 + 0.5 * mm_panels_v_size);

// Bottom panel static position

int static_panel_x = dynamic_panel_x_2;
int static_panel_y = MAIN_WINDOW_V_SIZE - (SCREEN_MARGIN_1 + 0.5 * mm_panels_v_size);

// ===== SETUP DATA =====

void main_menu_elements_setup()
{     
    // ===== Setup =====

    // Background setup

    Main_menu_panel->set_render_point(main_menu_background_x, main_menu_background_y);
    Main_menu_panel->set_size(BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
    Main_menu_panel->set_border_radius(0);
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

    if (App_inputs.is_just_released(Key_actions::EXIT))
    {
        this_app.app_sm.request_state_change(START_ID);
    }

    // Switch the state to MAIN MENU if ENTER pressed

    if (App_inputs.is_just_released(Key_actions::ENTER))
    {
        this_app.app_sm.request_state_change(START_ID);
    }

    if (App_inputs.is_just_released(Key_actions::SPECIAL_1))
    {
        App_palette.switch_to_the_next_palette();
        App_fonts.switch_to_the_next_font_palette();
        App_lang.switch_to_next_lang();
    }
}


void main_menu_elements_render(SDL_Renderer* renderer)
{
    // Render all elements

    // Update all elements
    
    // Background
    Main_menu_panel->render(renderer);

}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS
