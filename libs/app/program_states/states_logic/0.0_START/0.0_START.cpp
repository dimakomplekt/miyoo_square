// 0.0_START.cpp


// =========================================================================================== IMPORT

#include "0.0_START.h"


#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"

#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"




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
        // TEST
        Start_textbox->set_content(str_by_dictionary(gd_press_any_key));

    }
}


void start_actions()
{

    // Switch the state to MAIN MENU if ENTER pressed

    if (App_inputs.is_just_released(Key_actions::START_KA))
    {
        this_app.app_sm.request_state_change(MAIN_MENU_ID);
    }

    if (App_inputs.is_just_released(Key_actions::SELECT_KA))
    {
        // App_palette.switch_to_the_next_palette();
        // App_lang.switch_to_next_lang();
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

    line_draw
    (
        1, 10,
        600, 10,
        5,
        hex_to_sdl_color("#fd0404", 255),
        renderer
    );

}   

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


/*

// 0.0_START.cpp


// =========================================================================================== IMPORT

#include "0.0_START.h"


#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"

#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"




// Predeclare for switching states
#include "../../program_states.h"

#include "../../../app.h"

// Log
#include <iostream>

// =========================================================================================== IMPORT


// =========================================================================================== LOG

#define START_LOG(x) std::cout << "[START] " << x << std::endl

// =========================================================================================== LOG


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
    START_LOG("========================================");
    START_LOG("ENTERING START");
    START_LOG("========================================");


    // ===== State allocation =====

    START_LOG("Starting element creation");

    start_elements_create();

    START_LOG("Element creation finished");

    // ===== State allocation =====


    // Elements setup

    START_LOG("Starting element setup");

    start_elements_setup();

    START_LOG("Element setup finished");


    START_LOG("START ENTER finished");
}



void start_exit()
{
    START_LOG("========================================");
    START_LOG("EXITING START");
    START_LOG("========================================");


    // ===== State deallocation =====

    START_LOG("Starting element deallocation");

    start_elements_free_and_nullptr();

    START_LOG("Element deallocation finished");

    // ===== State deallocation =====


    START_LOG("START EXIT finished");
}


void start_update()
{
    // Update inputs

    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {
        START_LOG("HZ_1000 -> App_inputs.update()");

        App_inputs.update();

        START_LOG("App_inputs.update() finished");
    }


    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {
        START_LOG("HZ_240 -> GUI update");

        start_elements_update();

        START_LOG("Elements update finished");


        START_LOG("Processing START actions");

        start_actions();

        START_LOG("START actions finished");
    }
}


void start_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        START_LOG("HZ_120 -> Render");

        start_elements_render(renderer);

        START_LOG("Render finished");
    }
}




// =========================================================================================== MAIN STATE API




// =========================================================================================== INNER STATE FUNCTIONS


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void start_elements_create()
{
    START_LOG("----------------------------------------");
    START_LOG("CREATE ELEMENTS");
    START_LOG("----------------------------------------");


    // =======================================================================================
    // START PANEL

    START_LOG("Creating Start_panel");

    Start_panel = new My_SDL_panel();

    START_LOG("Start_panel created: " << Start_panel);


    // =======================================================================================
    // START TEXTBOX

    START_LOG("Creating Start_textbox");

    Start_textbox = new My_SDL_textbox();

    START_LOG("Start_textbox created: " << Start_textbox);


    START_LOG("CREATE ELEMENTS finished");
}


void start_elements_setup()
{
    START_LOG("----------------------------------------");
    START_LOG("SETUP ELEMENTS");
    START_LOG("----------------------------------------");


    // =======================================================================================
    // START PANEL SETUP

    START_LOG("Setting up Start_panel");

    START_LOG(
        "Start_panel render point = "
        << MAIN_WINDOW_H_SIZE / 2
        << ", "
        << MAIN_WINDOW_V_SIZE / 2
    );

    Start_panel->set_render_point(
        MAIN_WINDOW_H_SIZE / 2,
        MAIN_WINDOW_V_SIZE / 2
    );

    START_LOG("Start_panel render point set");


    START_LOG(
        "Start_panel size = "
        << MAIN_WINDOW_H_SIZE
        << " x "
        << MAIN_WINDOW_V_SIZE
    );

    Start_panel->set_size(
        MAIN_WINDOW_H_SIZE,
        MAIN_WINDOW_V_SIZE
    );

    START_LOG("Start_panel size set");


    START_LOG("Setting Start_panel border radius = 0");

    Start_panel->set_border_radius(0);

    START_LOG("Start_panel setup finished");


    // =======================================================================================
    // START TEXTBOX SETUP

    START_LOG("Setting up Start_textbox");


    START_LOG("Switching Start_textbox type -> HEADER_2");

    Start_textbox->switch_textbox_type(HEADER_2);

    START_LOG("Start_textbox type set");


    START_LOG("Setting Start_textbox content");

    Start_textbox->set_content(
        str_by_dictionary(gd_press_any_key)
    );

    START_LOG("Start_textbox content set");


    START_LOG("Enabling Start_textbox blinking mode");

    Start_textbox->switch_blinking_mode_flag(true);

    START_LOG("Start_textbox blinking mode enabled");


    // =======================================================================================
    // ADD TEXTBOX TO PANEL

    START_LOG("Adding Start_textbox to Start_panel");

    START_LOG(
        "Textbox position inside panel = "
        << Start_panel->get_width_size() * 0.5
        << ", "
        << Start_panel->get_height_size() * 0.5
    );

    Start_panel->add_element(

        Start_textbox,
        (Start_panel->get_width_size()) * 0.5,
        (Start_panel->get_height_size()) * 0.5,
        1
    );

    START_LOG("Start_textbox added to Start_panel");


    START_LOG("SETUP ELEMENTS finished");
}


void start_elements_free_and_nullptr()
{
    START_LOG("----------------------------------------");
    START_LOG("FREE ELEMENTS");
    START_LOG("----------------------------------------");


    // Protection

    if (Start_panel == nullptr)
    {
        START_LOG("Start_panel == nullptr");

        START_LOG("Elements already freed");

        return;
    }


    // =======================================================================================
    // FREE ELEMENTS

    START_LOG("Deleting Start_panel");

    Start_panel->delete_element();

    START_LOG("Start_panel deleted");


    // =======================================================================================
    // NULLPTR

    START_LOG("Setting pointers to nullptr");

    Start_panel = nullptr;
    Start_textbox = nullptr;

    START_LOG("Start_panel = nullptr");
    START_LOG("Start_textbox = nullptr");


    START_LOG("FREE ELEMENTS finished");
}


void start_elements_update()
{
    START_LOG("start_elements_update()");


    // =======================================================================================
    // LANGUAGE

    START_LOG("Checking language reset flag");

    reset_passed_by_dictionary_textboxes_if_language_switched();

    START_LOG("Language reset check finished");


    // =======================================================================================
    // UPDATE ELEMENTS

    START_LOG("Updating Start_panel");

    Start_panel->update();

    START_LOG("Start_panel update finished");


    START_LOG("start_elements_update() finished");
}


void reset_passed_by_dictionary_textboxes_if_language_switched()
{
    if (App_lang.get_lang_reset_flag())
    {
        START_LOG("LANGUAGE RESET FLAG DETECTED");

        START_LOG("Updating Start_textbox content");

        Start_textbox->set_content(
            str_by_dictionary(gd_press_any_key)
        );

        START_LOG("Start_textbox content updated");
    }
}


void start_actions()
{
    // =======================================================================================
    // START BUTTON

    if (App_inputs.is_just_released(Key_actions::START_KA))
    {
        START_LOG("INPUT: START released");

        START_LOG("Requesting state change -> MAIN_MENU_ID");

        this_app.app_sm.request_state_change(MAIN_MENU_ID);

        START_LOG("State change request sent");
    }


    // =======================================================================================
    // SELECT BUTTON

    if (App_inputs.is_just_released(Key_actions::SELECT_KA))
    {
        START_LOG("INPUT: SELECT released");

        START_LOG("Switching font palette");

        App_fonts.switch_to_the_next_font_palette();

        START_LOG("Font palette switched");


        static bool to_second = false;

        START_LOG("Animation state to_second = " << to_second);


        if (!to_second)
        {
            START_LOG("Animation branch: FIRST -> SECOND");


            int old_x = Start_textbox->get_x_render_point();
            int old_y = Start_textbox->get_y_render_point();


            START_LOG(
                "Current textbox position = "
                << old_x
                << ", "
                << old_y
            );


            // Start_panel->move_to_point(old_x - 100, old_y - 200, EXPONENTIAL, 500);

            START_LOG(
                "Moving textbox to = "
                << old_x + 100
                << ", "
                << old_y + 200
            );

            Start_textbox->move_to_point(
                old_x + 100,
                old_y + 200,
                LINEAR,
                300
            );

            START_LOG("Textbox movement requested");


            to_second = true;

            START_LOG("Animation state changed: to_second = true");
        }
        else
        {
            START_LOG("Animation branch: SECOND -> FIRST");


            int old_x = Start_textbox->get_x_render_point();
            int old_y = Start_textbox->get_y_render_point();


            START_LOG(
                "Current textbox position = "
                << old_x
                << ", "
                << old_y
            );


            // Start_panel->move_to_point(old_x + 100, old_y + 200, EXPONENTIAL, 500);

            START_LOG(
                "Moving textbox to = "
                << old_x - 100
                << ", "
                << old_y - 200
            );

            Start_textbox->move_to_point(
                old_x - 100,
                old_y - 200,
                LINEAR,
                300
            );

            START_LOG("Textbox movement requested");


            to_second = false;

            START_LOG("Animation state changed: to_second = false");
        }
    }
}


void start_elements_render(SDL_Renderer* renderer)
{
    START_LOG("start_elements_render()");


    // =======================================================================================
    // RENDER ALL ELEMENTS

    START_LOG("Rendering Start_panel");

    Start_panel->render(renderer);

    START_LOG("Start_panel render finished");


    START_LOG("start_elements_render() finished");
}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION
*/