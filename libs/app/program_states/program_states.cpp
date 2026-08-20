// game_states.cpp


// =========================================================================================== IMPORT

#include "program_states.h"

#include <iostream> // for std::cout, std::cerr


// Predeclare
#include "../app.h"


// States import

#include "states_logic/0.0_START/0.0_START.h"
#include "states_logic/1.0_MAIN_MENU/1.0_MAIN_MENU.h"
// #include "states_logic/1.1_FILE_CHOOSE/1.1_FILE_CHOOSE.h"
// #include "states_logic/1.2_MASKS_SETUP/1.2_MASKS_SETUP.h"
// #include "states_logic/1.3_FLOW_PARAMETERS_CALCULATION/1.3_FLOW_PARAMETERS_CALCULATION.h"
// #include "states_logic/1.4_REPORT_FORMING/1.4_REPORT_FORMING.h"
// #include "states_logic/2.0_PROGRAM_END/2.0_PROGRAM_END.h"


// =========================================================================================== IMPORT


// =========================================================================================== CALLBACKS



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

    // === MAIN_MENU ===


    // At this point, all states are registered in the state machine.
    // State_machine handles connecting parents and children based on IDs,
    // so hierarchical updates and callback chaining will work automatically.
}

// =========================================================================================== INITIALIZATION
