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

#if MIYOO_TEST_START_LOG
    #define START_LOG(x) std::cout << "[START] " << x << std::endl
#else
    #define START_LOG(x) do {} while (0)
#endif

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

    if (App_inputs.is_just_released(Key_actions::SELECT_KA) || App_inputs.is_just_released(Key_actions::Y_KA))
    {
        START_LOG("INPUT: SELECT released");

        SDL_app_shutdown(&this_app);

        START_LOG("INPUT: SELECT released processing end");
    }
}

static int test_int = 0;
bool draw_lines = false;


void start_elements_render(SDL_Renderer* renderer)
{
    START_LOG("start_elements_render()");


    // =======================================================================================
    // RENDER ALL ELEMENTS

    START_LOG("Rendering Start_panel");

    Start_panel->render(renderer);

    test_int += 10;

    if (test_int % 100 == 0) draw_lines = true;
    if (test_int % 200 == 0) draw_lines = false;



    if (draw_lines)
    {
            line_draw
            (
                25, MAIN_WINDOW_V_SIZE * 0.15,
                MAIN_WINDOW_H_SIZE - 25, MAIN_WINDOW_V_SIZE * 0.15,
                5,
                hex_to_sdl_color("#212dd7", 255),
                renderer
            );


            line_draw
            (
                25, MAIN_WINDOW_V_SIZE * 0.85,
                MAIN_WINDOW_H_SIZE - 25, MAIN_WINDOW_V_SIZE * 0.85,
                5,
                hex_to_sdl_color("#a9ec2c", 255),
                renderer
            );

    }


    START_LOG("Start_panel render finished");


    START_LOG("start_elements_render() finished");
}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION
