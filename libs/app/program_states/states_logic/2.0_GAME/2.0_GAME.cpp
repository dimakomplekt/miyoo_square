
// 2.0_GAME.cpp


// =========================================================================================== IMPORT

#include "2.0_GAME.h"


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


/*

handle_ctx background_1_ah;
handle_ctx background_2_ah;

handle_ctx floor_1_1_ah;
handle_ctx floor_1_2_ah;
handle_ctx floor_1_3_ah;
handle_ctx floor_2_1_ah;
handle_ctx floor_2_2_ah;
handle_ctx floor_2_3_ah;

handle_ctx wall_1_ah;
handle_ctx wall_2_ah;

handle_ctx square_1_ah;
handle_ctx square_2_ah;


*/

Background_sgo* scene_background = nullptr;


// =========================================================================================== STATE DATA


// =========================================================================================== HELPERS

void state_game_objects_create()
{
    background_assets_init();

    scene_background = new Background_sgo();

}


void state_game_objects_clear()
{
    scene_background->~Background_sgo();
    scene_background = nullptr;


    background_assets_clear();
}


// =========================================================================================== HELPERS


// =========================================================================================== CALLBACKS


void exit_to_menu()
{
    // Exit to main menu call
    this_app.app_sm.request_state_change(MAIN_MENU_ID);
}

// =========================================================================================== CALLBACKS


// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


void game_elements_create();

void game_elements_setup();

void game_elements_free_and_nullptr();


void scene_create();
void scene_setup();
void scene_update();
void scene_actions();
void scene_render(SDL_Renderer* renderer);
void scene_destroy();

void game_elements_update();

void reset_passed_by_dictionary_textboxes_if_language_switched_g();

void game_actions();

void game_elements_render(SDL_Renderer* renderer);

// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


// =========================================================================================== MAIN STATE API


void game_enter()
{
    // Log the enter in console
    std::cout << "Entering MAIN_MENU\n"; 

    // ===== State GUI allocation =====

    game_elements_create();

    scene_create();

    // ===== State GUI allocation =====


    // Elements setup

    game_elements_setup();

    scene_setup();

}


void game_exit()
{
    // ===== State deallocation =====

    game_elements_free_and_nullptr();

    scene_destroy();

    // ===== State deallocation =====


    // Log the exit in console

    std::cout << "Exiting MAIN_MENU\n"; 

}


void game_update()
{
    // Update inputs
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {
        App_inputs.update();
    }

    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {
        game_elements_update();

        game_actions();

        scene_actions();
    }
}


void game_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        game_elements_render(renderer);


        scene_render(renderer);
    }
}

// =========================================================================================== MAIN STATE API


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void game_elements_create()
{


}


// ===== SETUP DATA =====

// ===== Main sizes =====

// Static-size GUI

const int BACKGROUND_WIDTH  = MAIN_WINDOW_H_SIZE;
const int BACKGROUND_HEIGHT  = MAIN_WINDOW_V_SIZE;

// ===== SETUP DATA =====

void game_elements_setup()
{     


}



void scene_create()
{
    state_game_objects_create();
}


void scene_setup()
{

    // Background

    scene_background->set_render_point(BACKGROUND_WIDTH / 2, BACKGROUND_HEIGHT / 2);


    // Floor init


    // Walls init


    // Square inti

}


void scene_destroy()
{

    state_game_objects_clear();

}


void game_elements_free_and_nullptr()
{
    // Protection
   
    // Free all elements


    // Info / settings panel


    // Nullptr the pointers


}



void game_elements_update()
{

    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched_g();

    // Update all elements
    

    scene_update();


}


void scene_update()
{
    //
}




void reset_passed_by_dictionary_textboxes_if_language_switched_g()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        //
    }
}



void game_actions()
{
    // Main state actions

    if (App_inputs.is_just_released(Key_actions::EXIT_KA))
    {
        exit_to_menu();
    }
}


void scene_actions()
{
    // Actions with scene



}


void game_elements_render(SDL_Renderer* renderer)
{

}


void scene_render(SDL_Renderer* renderer)
{
    scene_background->render(renderer);
}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS
