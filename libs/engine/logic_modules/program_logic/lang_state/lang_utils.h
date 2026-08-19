// lang_utils.h

// =========================================================================================== IMPORT

#include "lang_state.h"

// =========================================================================================== IMPORT


// =========================================================================================== STRING BY DICTIONARY

inline std::string str_by_dictionary(const Lang_string_type& strings_pack)
{
    return strings_pack[static_cast<size_t>(App_lang.get_lang())];
}

// =========================================================================================== STRING BY DICTIONARY
