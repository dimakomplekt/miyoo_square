// lang_state.h

#pragma once

// =========================================================================================== IMPORT

#include "lang_types.h"

// =========================================================================================== IMPORT

// =========================================================================================== LANG_STATE SINGLETON

/**
 * @brief Singleton class that holds the current language state.
 *
 * Ensures there is only one global instance of language state.
 * Provides methods to get and set the current language.
 *
 * Usage:
 * 
 * Lang_state::Instance().set_lang(Lang_list::RU);
 * 
 * auto current = Lang_state::Instance().get_lang();
 * 
 */
class Lang_state
{
    public:

        // ===== Instance =====

        /**
         * @brief Returns the singleton instance.
         *
         * Guarantees a single global instance of Lang_state.
         * 
         */
        static Lang_state& Instance();

        // ===== Instance =====


        // ===== Language workflow =====

        /**
         * @brief Returns the current language.
         *
         * @return Currently set language as Lang_list.
         * 
         */
        Lang_list get_lang() const;


        /**
         * @brief Sets the current language.
         *
         * @param lang New language to set.
         * 
         */
        void set_lang(Lang_list lang);


        // Switches language to the next language inside the list
        void switch_to_next_lang();

        // ===== Language workflow =====


        // ===== Language reset =====

        /**
         * @brief Gets the language reset flag.
         *
         * @return true if the language has been reset, false otherwise.
         * 
         */
        bool get_lang_reset_flag() const;

        // Function, called inside the main loop, before state loop
        // sets lang_reset_flag to false after 2 loops, to avoid constant resettings.
        // Called inside SDL_app_cycle(SDL_app_ctx* app) in app.cpp
        void lang_reset_flag_state_loop_update();

        // ===== Language reset =====


    private:

        // ===== Constructor and Destuctor =====

        // Private constructor ensures no external instances can be created.
        Lang_state();

        // Default destructor is fine; no dynamic allocation to clean up.
        ~Lang_state() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        Lang_state(const Lang_state&) = delete;


        // Assignment operator is deleted to prevent copying the singleton.
        Lang_state& operator=(const Lang_state&) = delete;

        // ===== Constructor and Destuctor =====


        // ===== Language =====

        // Currently active language
        Lang_list current_lang;

        // ===== Language =====


        // ===== Reset =====

        // Flag to indicate if the language has been reset (for UI updates, etc.)
        bool lang_reset_flag;

        // Counter to track how many loops have passed since the language reset flag was set
        unsigned int lang_reset_flag_loops_counter;

        // ===== Reset =====
};

// =========================================================================================== LANG_STATE SINGLETON


// =========================================================================================== LANG_STATE SINGLETON FOR USE

// Global singleton instance of GI_mouse for easy access throughout the program
inline Lang_state& App_lang = Lang_state::Instance();


// ===========================================================================================  LANG_STATE SINGLETON FOR USE
