// lang_state.cpp


// =========================================================================================== IMPORT

#include "lang_state.h"

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
    : Curr_lang(DEFAULT_LANG) // Initialize the current language to default (EN)
{
    // Constructor is private and only called once by Instance().
    // No dynamic memory allocation needed here.
}


Lang_list Lang_state::Get_lang() const
{
    return Curr_lang; // Simply return the currently active language
}


bool Lang_state::Set_lang(Lang_list language)
{
    // Validate input: must be within the enum range
    if (language < Lang_list::EN || language >= Lang_list::LIMIT) return false;

    // Assign the selected language
    // Could extend this switch to initialize other language-specific resources if needed
    switch (language)
    {
        case Lang_list::EN:
            Curr_lang = Lang_list::EN;
            break;

        case Lang_list::RU:
            Curr_lang = Lang_list::RU;
            break;

        // Defensive fallback, should never hit because of validation above
        default: return false;
    }


    return true; // Language successfully set
}

// =========================================================================================== LANG_STATE SINGLETON
