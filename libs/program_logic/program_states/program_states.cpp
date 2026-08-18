// game_states.cpp


// =========================================================================================== IMPORT

#include "program_states.h"

#include <iostream> // for std::cout, std::cerr


// Predeclare
#include "../app.h"


// States import

#include "states_logic/0.0_START/0.0_START.h"
#include "states_logic/1.0_MAIN_MENU/1.0_MAIN_MENU.h"
#include "states_logic/1.1_FILE_CHOOSE/1.1_FILE_CHOOSE.h"
#include "states_logic/1.2_MASKS_SETUP/1.2_MASKS_SETUP.h"
// #include "states_logic/1.3_FLOW_PARAMETERS_CALCULATION/1.3_FLOW_PARAMETERS_CALCULATION.h"
// #include "states_logic/1.4_REPORT_FORMING/1.4_REPORT_FORMING.h"
// #include "states_logic/2.0_PROGRAM_END/2.0_PROGRAM_END.h"


// =========================================================================================== IMPORT


// =========================================================================================== CALLBACKS


void program_end_enter()
{
    std::cout << "Entering EXIT_PROGRAM\n";
}

void program_end_exit()
{

}

void program_end_update()
{

}

void program_end_render(SDL_Renderer* renderer)
{

}


// =========================================================================================== CALLBACKS


// =========================================================================================== INITIALIZATION

void init_program_states(State_machine& app_state_machine)
{
    // Each block below creates a State object, assigns its enter/exit callbacks,
    // and adds it to the state machine. The hierarchy linking is handled
    // automatically by State_machine::add_state.

    // === START ===

    // Smart initialization by the state machine
    app_state_machine.initiate_state(START_ID, "START");

    // Callbacs setting by the state machine
    if (auto* s = app_state_machine.get_state(START_ID))
    {
        s->on_enter = start_enter;          // Actions on the state entering 
        s->on_exit  = start_exit;           // Actions on the state exit
        s->state_update = start_update;     // Actions on the state update
        s->state_render = start_render;     // Rendering for the state
    }

    // === START ===


    // === MAIN_MENU ===

    app_state_machine.initiate_state(MAIN_MENU_ID, "MAIN_MENU");

    if (auto* s = app_state_machine.get_state(MAIN_MENU_ID))
    {
        s->on_enter = main_menu_enter;          // Actions on the state entering 
        s->on_exit  = main_menu_exit;           // Actions on the state exit
        s->state_update = main_menu_update;     // Actions on the state update
        s->state_render = main_menu_render;     // Rendering for the state
    }

    // === FILE_CHOOSE ===

    app_state_machine.initiate_state(FILE_CHOOSE_ID, "FILE_CHOOSE");

    if (auto* s = app_state_machine.get_state(FILE_CHOOSE_ID))
    {
        s->on_enter = file_choose_enter;          // Actions on the state entering 
        s->on_exit  = file_choose_exit;           // Actions on the state exit
        s->state_update = file_choose_update;     // Actions on the state update
        s->state_render = file_choose_render;     // Rendering for the state
    }

    // === MAIN_MENU ===


    // === MASKS_SETUP ===

    app_state_machine.initiate_state(MASKS_SETUP_ID, "MASKS_SETUP");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID))
    {
        s->on_enter = masks_setup_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_exit;           // Actions on the state exit
        s->state_update = masks_setup_update;     // Actions on the state update
        s->state_render = masks_setup_render;     // Rendering for the state
    }

    
    app_state_machine.initiate_state(MASKS_SETUP_ID_1, "MASKS_SETUP_1");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID_1))
    {
        s->on_enter = masks_setup_1_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_1_exit;           // Actions on the state exit
        s->state_update = masks_setup_1_update;     // Actions on the state update
        s->state_render = masks_setup_1_render;     // Rendering for the state
    }


    app_state_machine.initiate_state(MASKS_SETUP_ID_2, "MASKS_SETUP_2");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID_2))
    {
        s->on_enter = masks_setup_2_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_2_exit;           // Actions on the state exit
        s->state_update = masks_setup_2_update;     // Actions on the state update
        s->state_render = masks_setup_2_render;     // Rendering for the state
    }

        
    app_state_machine.initiate_state(MASKS_SETUP_ID_3, "MASKS_SETUP_3");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID_3))
    {
        s->on_enter = masks_setup_3_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_3_exit;           // Actions on the state exit
        s->state_update = masks_setup_3_update;     // Actions on the state update
        s->state_render = masks_setup_3_render;     // Rendering for the state
    }

        
    app_state_machine.initiate_state(MASKS_SETUP_ID_4, "MASKS_SETUP_4");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID_4))
    {
        s->on_enter = masks_setup_4_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_4_exit;           // Actions on the state exit
        s->state_update = masks_setup_4_update;     // Actions on the state update
        s->state_render = masks_setup_4_render;     // Rendering for the state
    }

        
    app_state_machine.initiate_state(MASKS_SETUP_ID_5, "MASKS_SETUP_5");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID_5))
    {
        s->on_enter = masks_setup_5_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_5_exit;           // Actions on the state exit
        s->state_update = masks_setup_5_update;     // Actions on the state update
        s->state_render = masks_setup_5_render;     // Rendering for the state
    }

        
    app_state_machine.initiate_state(MASKS_SETUP_ID_6, "MASKS_SETUP_6");

    if (auto* s = app_state_machine.get_state(MASKS_SETUP_ID_6))
    {
        s->on_enter = masks_setup_6_enter;          // Actions on the state entering 
        s->on_exit  = masks_setup_6_exit;           // Actions on the state exit
        s->state_update = masks_setup_6_update;     // Actions on the state update
        s->state_render = masks_setup_6_render;     // Rendering for the state
    }

    // === MASKS_SETUP ===


    // === EXIT_PROGRAM ===

    app_state_machine.initiate_state(PROGRAM_END_ID, "PROGRAM END");

    if (auto* s = app_state_machine.get_state(PROGRAM_END_ID))
    {
        s->on_enter = program_end_enter;
        s->on_exit  = program_end_exit;
        s->state_update = program_end_update;
        s->state_render = program_end_render;
    }

    // === EXIT_PROGRAM ===

    // At this point, all states are registered in the state machine.
    // State_machine handles connecting parents and children based on IDs,
    // so hierarchical updates and callback chaining will work automatically.
}

// =========================================================================================== INITIALIZATION
