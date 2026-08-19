// lang_types.h

#pragma once


// =========================================================================================== IMPORT

#include <array>
#include <string>

// =========================================================================================== IMPORT


// =========================================================================================== LANGUAGE ENUM

/**
 * @brief Enum for all supported languages in the application.
 *
 * Each value represents a language option. The LIMIT value is used
 * to indicate the total number of languages and can help in validation.
 */
enum class Lang_list : unsigned int
{

    EN = 0,  // English
    RU = 1,  // Russian
    LIMIT = 2 // Sentinel for number of languages
    
};


// Default language if none is explicitly set
#define DEFAULT_LANG Lang_list::RU

// =========================================================================================== LANGUAGE ENUM


// =========================================================================================== DICTIONARY ARRAY

using Lang_string_type = std::array<std::string, static_cast<size_t>(Lang_list::LIMIT)>;

// =========================================================================================== DICTIONARY ARRAY
