// program_dictionary.h

#pragma once


// =========================================================================================== IMPORT

#include "../../../engine/logic_modules/program_logic/lang_state/lang_types.h"

// =========================================================================================== IMPORT



// =========================================================================================== GLOBAL DICTIONARY


inline Lang_string_type gd_coordinate = {

    "Сoordinate",
    "Координата"

};

inline Lang_string_type gd_pixels = {

    "px",
    "пкс"

};

// =========================================================================================== GLOBAL DICTIONARY



// =========================================================================================== STATE 0.0 DICTIONARY

// Could be not inline - replace with extern and define in cpp, but for simplicity - inline, as it's just a small 
// struct with const char* and no dynamic memory management

inline Lang_string_type gd_press_any_key = {

    "Press ENTER to continue...",
    u8"Нажмите ENTER для продолжения..."

};



// =========================================================================================== STATE 0.0 DICTIONARY
