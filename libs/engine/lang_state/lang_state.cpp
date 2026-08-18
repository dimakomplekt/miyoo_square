// lang_state.cpp


// =========================================================================================== IMPORT

#include "lang_state.h"

#include <iostream>

// =========================================================================================== IMPORT


// =========================================================================================== LANG_STATE SINGLETON

Lang_state& Lang_state::Instance()
{
    // Local static ensures thread-safe lazy initialization in C++11+
    // Guarantees that only one instance of Lang_state exists throughout the program.
    static Lang_state instance;

    return instance;
}


Lang_state::Lang_state()
{
    this->current_lang = DEFAULT_LANG; // Initialize the current language to default (EN)

    this->lang_reset_flag = true;
    this->lang_reset_flag_loops_counter = 0;


    // Constructor is private and only called once by Instance().
    // No dynamic memory allocation needed here.
}


Lang_list Lang_state::get_lang() const
{
    return current_lang; // Simply return the currently active language
}


void Lang_state::set_lang(Lang_list language)
{
    // Validate input: must be within the enum range
    if (language < Lang_list::EN || language >= Lang_list::LIMIT)
    {
        std::cerr << "Wrong language pass";
        return;
    }

    // Assign the selected language
    // Could extend this switch to initialize other language-specific resources if needed
    switch (language)
    {
        case Lang_list::EN:

            current_lang = Lang_list::EN;
            break;

        case Lang_list::RU:
        
            current_lang = Lang_list::RU;
            break;

        // Defensive fallback, should never hit because of validation above
        default: return;
    }

    // Loop counter and flag reset for language switch in dictionary-oriented textboxes
    this->lang_reset_flag = true;
    this->lang_reset_flag_loops_counter = 0;
}


void Lang_state::switch_to_next_lang()
{
    // Convert enum to underlying integer
    unsigned int next = static_cast<unsigned int>(current_lang) + 1;

    // Wrap around using LIMIT
    if (next >= static_cast<unsigned int>(Lang_list::LIMIT))
    {
        next = static_cast<unsigned int>(Lang_list::EN);
    }

    // Apply new language
    this->set_lang(static_cast<Lang_list>(next));
}



bool Lang_state::get_lang_reset_flag() const
{
    return this->lang_reset_flag;
}


void Lang_state::lang_reset_flag_state_loop_update()
{
    // If the flag is set, we need to track how many loops have passed
    if (lang_reset_flag)
    {
        lang_reset_flag_loops_counter++;

        // After 2 loops, reset the flag to avoid constant resetting
        if (lang_reset_flag_loops_counter >= 2)
        {
            lang_reset_flag = false;
            lang_reset_flag_loops_counter = 0; // Reset counter for next time
        }
    }
}

// =========================================================================================== LANG_STATE SINGLETON
