// 1.0_MAIN_MENU.cpp


// =========================================================================================== IMPORT

#include "1.0_MAIN_MENU.h"


#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_texture/my_sdl_texture.h"


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


// Next state
My_SDL_button* Analysis_start_button = nullptr;


// Information panel

My_SDL_button* Information_button = nullptr;

My_SDL_panel* Information_panel = nullptr;


My_SDL_textbox* Info_1_textbox = nullptr;

My_SDL_textbox* Info_2_textbox = nullptr;

My_SDL_textbox* Info_3_textbox = nullptr;

My_SDL_textbox* Info_4_textbox = nullptr;

My_SDL_textbox* Info_5_textbox = nullptr;



// Settings panel

My_SDL_button* Settings_button = nullptr;


My_SDL_panel* Settings_panel = nullptr;

My_SDL_button* Switch_theme_button = nullptr;

My_SDL_button* Switch_language_button = nullptr;

My_SDL_button* Switch_font_button = nullptr;

My_SDL_button* Settings_accept_button = nullptr;


// Exit button

My_SDL_button* Exit_button = nullptr;


// Panel with plasma spraying screen of video

My_SDL_panel* Plasma_spraying_panel = nullptr;

My_SDL_textbox* PSMVA_textbox = nullptr;



// ===== Analysis start =====

void analysis_start_on_click();

// ===== Control the panel show regime =====

enum class right_panel_state_ctx
{

    NONE,
    INFORMATION,
    SETTINGS

};

right_panel_state_ctx right_panel_state = right_panel_state_ctx::NONE;

void switch_right_panel_state(right_panel_state_ctx new_state);

bool need_to_move_dynamic_panels = false;


void information_on_click();

void settings_on_click();


// ===== Exit =====

void exit_on_click();


// ===== Informaton =====

void information_close_on_click();


// ===== Settings =====

void switch_theme_button_on_click();
void switch_language_button_on_click();
void switch_font_button_on_click();
void settings_accept_button_on_click();

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


    // Next state
    Analysis_start_button = new My_SDL_button();


    // Information panel

    Information_button = new My_SDL_button();

    Information_panel = new My_SDL_panel();


    Info_1_textbox = new My_SDL_textbox();

    Info_2_textbox = new My_SDL_textbox();

    Info_3_textbox = new My_SDL_textbox();

    Info_4_textbox = new My_SDL_textbox();

    Info_5_textbox = new My_SDL_textbox();



    // Settings panel

    Settings_button = new My_SDL_button();

    Settings_panel = new My_SDL_panel();

    Switch_theme_button = new My_SDL_button();

    Switch_language_button = new My_SDL_button();

    Switch_font_button = new My_SDL_button();

    Settings_accept_button = new My_SDL_button();


    // Exit button

    Exit_button = new My_SDL_button();


    // Panel with plasma spraying screen of video

    Plasma_spraying_panel = new My_SDL_panel();

    PSMVA_textbox = new My_SDL_textbox();

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


    // Analysis button
    Analysis_start_button->switch_button_textbox_type(HEADER_2);
    Analysis_start_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_analysis_start_button));
    Analysis_start_button->set_size(mm_buttons_h_size, mm_buttons_v_size);
    Analysis_start_button->set_render_point( analysis_start_x, analysis_start_y);
    Analysis_start_button->on_click = analysis_start_on_click;

    
    // Information button
    Information_button->switch_button_textbox_type(HEADER_2);
    Information_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_information_button));
    Information_button->set_size(mm_buttons_h_size, mm_buttons_v_size);
    Information_button->set_render_point(information_x, information_y);
    Information_button->on_click = information_on_click;

    // Settings button
    Settings_button->switch_button_textbox_type(HEADER_2);
    Settings_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_settings_button));
    Settings_button->set_size(mm_buttons_h_size, mm_buttons_v_size);
    Settings_button->set_render_point(settings_x, settings_y);
    Settings_button->on_click = settings_on_click;

    // Exit button
    Exit_button->switch_button_textbox_type(HEADER_2);
    Exit_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_exit_button));
    Exit_button->set_size(mm_buttons_h_size, mm_buttons_v_size);
    Exit_button->set_render_point(exit_x, exit_y);
    Exit_button->on_click = exit_on_click;

    // Information panel basic position

    Information_panel->set_size(mm_panels_h_size, mm_panels_v_size);
    Information_panel->set_render_point(dynamic_panel_x_1, dynamic_panel_y_1);


    Info_1_textbox->set_content(str_by_dictionary(gd_information_textbox_1));
    Info_1_textbox->switch_textbox_type(HEADER_3);

    Info_2_textbox->set_content(str_by_dictionary(gd_information_textbox_2));
    Info_2_textbox->switch_textbox_type(HEADER_3);

    Info_3_textbox->set_content(str_by_dictionary(gd_information_textbox_3));
    Info_3_textbox->switch_textbox_type(HEADER_3);

    Info_4_textbox->switch_textbox_type(HEADER_3);
    Info_4_textbox->set_content(str_by_dictionary(gd_information_textbox_4));

    Info_5_textbox->switch_textbox_type(HEADER_3);
    Info_5_textbox->set_content(str_by_dictionary(gd_information_textbox_5));


    // Settings panel

    Settings_panel->set_size(mm_panels_h_size, mm_panels_v_size);
    Settings_panel->set_render_point(dynamic_panel_x_1, dynamic_panel_y_1);


    Switch_theme_button->switch_button_textbox_type(EMOJI_TEXT);
    Switch_theme_button->get_button_content_textbox()->set_content("🎨");
    Switch_theme_button->set_size(mini_buttons_h_size, mini_buttons_v_size);
    Switch_theme_button->on_click = switch_theme_button_on_click;


    Switch_language_button->switch_button_textbox_type(EMOJI_TEXT);
    Switch_language_button->get_button_content_textbox()->set_content("🌐");
    Switch_language_button->set_size(mini_buttons_h_size, mini_buttons_v_size);
    Switch_language_button->on_click = switch_language_button_on_click;


    Switch_font_button->switch_button_textbox_type(EMOJI_TEXT);
    Switch_font_button->get_button_content_textbox()->set_content("🔤");
    Switch_font_button->set_size(mini_buttons_h_size, mini_buttons_v_size);
    Switch_font_button->on_click = switch_font_button_on_click;


    Settings_accept_button->switch_button_textbox_type(EMOJI_TEXT);
    Settings_accept_button->get_button_content_textbox()->set_content("✔");
    Settings_accept_button->set_size(mini_buttons_h_size, mini_buttons_v_size);
    Settings_accept_button->on_click = settings_accept_button_on_click;


    // ===== Add elements into panels =====

    Settings_panel->add_element(

        Switch_theme_button,
        mini_buttons_h_size * 0.5 + 1 * SCREEN_MARGIN_1,
        mini_buttons_v_size * 0.5 + 1 * SCREEN_MARGIN_1,
        1
        
    );


    Settings_panel->add_element(

        Switch_language_button,
        mini_buttons_h_size * 1.5 + 2 * SCREEN_MARGIN_1,
        mini_buttons_v_size * 0.5 + 1 * SCREEN_MARGIN_1,
        1
        
    );

    Settings_panel->add_element(

        Switch_font_button,
        mini_buttons_h_size * 0.5 + 1 * SCREEN_MARGIN_1,
        mini_buttons_v_size * 1.5 + 2 * SCREEN_MARGIN_1,
        1
        
    );

    Settings_panel->add_element(

        Settings_accept_button,
        mini_buttons_h_size * 1.5 + 2 * SCREEN_MARGIN_1,
        mini_buttons_v_size * 1.5 + 2 * SCREEN_MARGIN_1,
        1
        
    );


    // Information panel

    Information_panel->add_element(

        Info_1_textbox,
        mm_panels_h_size * 0.5,
        mm_panels_v_size * ((1.0 / 6) * 1),
        1
        
    );

    Information_panel->add_element(

        Info_2_textbox,
        mm_panels_h_size * 0.5,
        mm_panels_v_size * ((1.0 / 6) * 2),
        1
        
    );

    Information_panel->add_element(

        Info_3_textbox,
        mm_panels_h_size * 0.5,
        mm_panels_v_size * ((1.0 / 6) * 3),
        1
        
    );

    Information_panel->add_element(

        Info_4_textbox,
        mm_panels_h_size * 0.5,
        mm_panels_v_size * ((1.0 / 6) * 4),
        1
        
    );

    Information_panel->add_element(

        Info_5_textbox,
        mm_panels_h_size * 0.5,
        mm_panels_v_size * ((1.0 / 6) * 5),
        1
        
    );


    // Panel with plasma spraying screen of video
    Plasma_spraying_panel->set_size(mm_panels_h_size, mm_panels_v_size);
    Plasma_spraying_panel->set_render_point(static_panel_x, static_panel_y);

    // PSMVA textbox
    PSMVA_textbox->switch_textbox_type(HEADER_1);
    PSMVA_textbox->set_content(THIS_APP_NAME);
    


    Plasma_spraying_panel->add_element(

        PSMVA_textbox,
        mm_panels_h_size * 0.5,
        mm_panels_v_size * 0.5,
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


    // Next state
    Analysis_start_button->delete_element();


    // Information panel

    Information_button->delete_element();

    Information_panel->delete_element();


    // Settings panel

    Settings_button->delete_element();

    Settings_panel->delete_element();

    // Exit button

    Exit_button->delete_element();


    // Panel with plasma spraying screen of video

    Plasma_spraying_panel->delete_element();


    // Nullptr the pointers

    // Background
    Main_menu_panel = nullptr;


    // Next state
    Analysis_start_button = nullptr;


    // Information panel

    Information_button = nullptr;

    Information_panel = nullptr;


    Info_1_textbox = nullptr;

    Info_2_textbox = nullptr;

    Info_3_textbox = nullptr;

    Info_4_textbox = nullptr;

    Info_5_textbox = nullptr;


    // Settings panel

    Settings_button = nullptr;

    Settings_panel = nullptr;

    Switch_theme_button = nullptr;

    Switch_language_button = nullptr;

    Switch_font_button = nullptr;

    Settings_accept_button = nullptr;


    // Exit button

    Exit_button = nullptr;


    // Panel with plasma spraying screen of video

    Plasma_spraying_panel = nullptr;

    PSMVA_textbox = nullptr;

}


void main_menu_elements_update()
{
    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched_mm();


    // Update all elements

    // Background
    Main_menu_panel->update();


    // Next state
    Analysis_start_button->update();


    // Information panel

    Information_button->update();

    Information_panel->update();


    // Settings panel

    Settings_button->update();

    Settings_panel->update();


    // Exit button

    Exit_button->update();


    // Panel with plasma spraying screen of video

    Plasma_spraying_panel->update();

}


void reset_passed_by_dictionary_textboxes_if_language_switched_mm()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        // Next state

        Analysis_start_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_analysis_start_button));

        // Information panel

        Information_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_information_button));


        Info_1_textbox->set_content(str_by_dictionary(gd_information_textbox_1));

        Info_2_textbox->set_content(str_by_dictionary(gd_information_textbox_2));
    
        Info_3_textbox->set_content(str_by_dictionary(gd_information_textbox_3));

        Info_4_textbox->set_content(str_by_dictionary(gd_information_textbox_4));
    
        Info_5_textbox->set_content(str_by_dictionary(gd_information_textbox_5));


        // Settings panel
        Settings_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_settings_button));

        // Exit button
        Exit_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_exit_button));

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


    // Moving of the button

    if (App_inputs.is_just_released(Key_actions::LEFT))
    {
        Analysis_start_button->move_to_point(Analysis_start_button->get_x_render_point() - 25, Analysis_start_button->get_y_render_point(), LOGARITHMIC, 300);
    }

    if (App_inputs.is_just_released(Key_actions::UP))
    {
        Analysis_start_button->move_to_point(Analysis_start_button->get_x_render_point(), Analysis_start_button->get_y_render_point() - 25, LOGARITHMIC, 300);
    }

    if (App_inputs.is_just_released(Key_actions::RIGHT))
    {
        Analysis_start_button->move_to_point(Analysis_start_button->get_x_render_point() + 25, Analysis_start_button->get_y_render_point(), LOGARITHMIC, 300);
    }

    if (App_inputs.is_just_released(Key_actions::DOWN))
    {
        Analysis_start_button->move_to_point(Analysis_start_button->get_x_render_point(), Analysis_start_button->get_y_render_point() + 25, LOGARITHMIC, 300);
    }


    // Moving of the panels
    switch (right_panel_state)
    {
        case right_panel_state_ctx::NONE:

            if (need_to_move_dynamic_panels)
            {
                Information_panel->move_to_point(dynamic_panel_x_1, dynamic_panel_y_1, EXPONENTIAL, 600);
                Settings_panel->move_to_point(dynamic_panel_x_1, dynamic_panel_y_1, EXPONENTIAL, 600);
                need_to_move_dynamic_panels = false;
            }

            break;

        case right_panel_state_ctx::INFORMATION:

            if (need_to_move_dynamic_panels)
            {
                Information_panel->move_to_point(dynamic_panel_x_2, dynamic_panel_y_2, EXPONENTIAL, 900);
                Settings_panel->move_to_point(dynamic_panel_x_1, dynamic_panel_y_1, EXPONENTIAL, 600);
                need_to_move_dynamic_panels = false;
            }

            break;


        case right_panel_state_ctx::SETTINGS:

            if (need_to_move_dynamic_panels)
            {
                Information_panel->move_to_point(dynamic_panel_x_1, dynamic_panel_y_1, EXPONENTIAL, 600);
                Settings_panel->move_to_point(dynamic_panel_x_2, dynamic_panel_y_2, EXPONENTIAL, 900);
                need_to_move_dynamic_panels = false;
            }

            break;


        default:
            break;
    }

}


void main_menu_elements_render(SDL_Renderer* renderer)
{
    // Render all elements

    // Update all elements
    
    // Background
    Main_menu_panel->render(renderer);


    // Next state
    Analysis_start_button->render(renderer);

    Information_button->render(renderer);

    Settings_button->render(renderer);

    Exit_button->render(renderer);


    Information_panel->render(renderer);

    // Settings panel

    Settings_panel->render(renderer);


    // Panel with plasma spraying screen of video

    Plasma_spraying_panel->render(renderer);

}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS

void switch_right_panel_state(right_panel_state_ctx new_state)
{
    right_panel_state = new_state;

    need_to_move_dynamic_panels = true;
}


void analysis_start_on_click()
{
    this_app.app_sm.request_state_change(FILE_CHOOSE_ID);
}


void information_on_click()
{
    switch_right_panel_state(right_panel_state_ctx::INFORMATION);
}

void settings_on_click()
{
    switch_right_panel_state(right_panel_state_ctx::SETTINGS);
}


void exit_on_click()
{
    this_app.app_sm.request_state_change(START_ID);
}


void information_close_on_click()
{
    switch_right_panel_state(right_panel_state_ctx::NONE);
}


void switch_theme_button_on_click()
{
    App_palette.switch_to_the_next_palette();
}


void switch_language_button_on_click()
{
    App_lang.switch_to_next_lang();
}


void switch_font_button_on_click()
{
    App_fonts.switch_to_the_next_font_palette();
}


void settings_accept_button_on_click()
{
    switch_right_panel_state(right_panel_state_ctx::NONE);
}

// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS
