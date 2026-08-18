// global_palette.h

#pragma once

// =========================================================================================== IMPORT

#include "../GUI_functions/translators/color_translator.h"      // Color translator from HEX to SDL_Color

#include <string>
#include <vector>

// =========================================================================================== IMPORT


// =========================================================================================== PALETTE STRUCT

// App palette structure, which contains all basic colors for basic UI
struct app_palette_ctx
{
    // Global background
    SDL_Color global_background_color;

    // Basic background colors (for UI elements - buttons, textboxes, faders)

    SDL_Color basic_background_color;
    SDL_Color basic_border_color;
    SDL_Color basic_content_color;
    SDL_Color basic_shadow_color;

    // Basic background colors (for UI elements - panels)

    SDL_Color inverted_basic_background_color;
    SDL_Color inverted_basic_border_color;
    SDL_Color inverted_basic_content_color;
    SDL_Color inverted_basic_shadow_color;


    // Basic hover and click state colors (for UI elements - buttons, faders)

    SDL_Color hover_background_color;
    SDL_Color hover_border_color;
    SDL_Color hover_content_color;
    SDL_Color hover_shadow_color;

    SDL_Color click_background_color;
    SDL_Color click_border_color;
    SDL_Color click_content_color;
    SDL_Color click_shadow_color;


    // Denied element access colors (for UI elements with click permission logic)

    SDL_Color access_denied_background_color;
    SDL_Color access_denied_border_color;
    SDL_Color access_denied_content_color;
    SDL_Color access_denied_shadow_color;

    // Permitted element access colors (for UI elements with click permission logic)

    SDL_Color access_permitted_background_color;
    SDL_Color access_permitted_border_color;
    SDL_Color access_permitted_content_color;
    SDL_Color access_permitted_shadow_color;

    SDL_Color access_permitted_hover_background_color;
    SDL_Color access_permitted_hover_border_color;
    SDL_Color access_permitted_hover_content_color;
    SDL_Color access_permitted_hover_shadow_color;

    SDL_Color access_permitted_click_background_color;
    SDL_Color access_permitted_click_border_color;
    SDL_Color access_permitted_click_content_color;
    SDL_Color access_permitted_click_shadow_color;

};

// =========================================================================================== PALETTE STRUCT


// =========================================================================================== APP PALETTE SINGLETON CLASS


/**
 * @brief Singleton class that holds the palettes structures and current palette.
 *
 * Ensures there is only one global instance of app palette.
 * 
 * Provides methods to set the palettes list, get the current palette colors 
 * and switch current palette to the specific palette or next palette inside the palettes
 * list.
 *
 */
class Global_palette
{

    public:

        // ===== Instance =====

        /**
         * @brief Returns the singleton instance.
         *
         * Guarantees a single global instance of Global_palette.
         * 
         */
        static Global_palette& Instance();

        // ===== Instance =====


        // ===== Palette workflow =====

        /**
         * @brief Adds a palette to the internal list.
         *
         * @param new_palette Palette data to be stored.
         *
         * Palette is copied into internal std::vector storage.
         * No dynamic allocation or ownership transfer is involved.
         *
         */
        void add_palette(const app_palette_ctx& new_palette);


        /**
         * @brief Gets the current active palette.
         *
         * @return Const reference to the current palette.
         *
         * Returned as const reference to:
         * - avoid copying large structure (performance)
         * - prevent modification of global palette state
         *
         * NOTE:
         * Reference remains valid as long as palettes_list
         * is not structurally modified (no push_back / reallocation).
         * So the list should be filled at the start of the program 
         * and not modified after that.
         * 
         */
        const app_palette_ctx& get_current_palette() const;


        /**
         * @brief Switches to the next palette in the list.
         *
         * Cycles through available palettes.
         * Wraps to the first palette when the end is reached.
         *
         * Also sets reset flag to notify UI about palette change.
         * 
         */
        void switch_to_the_next_palette();


        /**
         * @brief Switches to a specific palette by id.
         *
         * @param palette_id Index of palette in internal list.
         *
         * Sets current palette if id is valid.
         * Preferred over pointer-based access to avoid invalid memory references.
         *
         * Also sets reset flag to notify UI about palette change.
         * 
         */
        void switch_to_the_palette(unsigned int palette_id);

        // ===== Palette workflow =====


        // ===== Palette reset =====

        /**
         * @brief Gets the palette reset flag.
         *
         * @return true if the palette has been reset, false otherwise.
         * 
         */
        bool get_palette_reset_flag() const;

        // Function, called inside the main loop, before state loop
        // sets palette_reset_flag to false after 2 loops, to avoid constant resettings.
        // Called inside SDL_app_cycle(SDL_app_ctx* app) in app.cpp
        void palette_reset_flag_state_loop_update();

        // ===== Palette reset =====
        

    private:

        // ===== Constructor and Destructor =====
        
        // Private constructor ensures no external instances can be created.
        Global_palette();

        // Default destructor is fine; no dynamic allocation to clean up.
        ~Global_palette() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        Global_palette(const Global_palette&) = delete;

        // Assignment operator is deleted to prevent copying the singleton.
        Global_palette& operator=(const Global_palette&) = delete;

        // ===== Constructor and Destructor =====

        
        // ===== Palettes =====

        // Palettes list
        std::vector<app_palette_ctx> palettes_list;


        // Currently active palette ID (by palettes list)
        unsigned int current_palette_id;

        // ===== Palettes =====


        // ===== Reset logic =====

        // Flag to indicate if the palette has been reset (for UI updates, etc.)
        bool palette_reset_flag;

        // Counter to track how many loops have passed since the palette reset flag was set
        unsigned int palette_reset_flag_loops_counter;

        // ===== Reset logic =====
};

// =========================================================================================== APP PALETTE SINGLETON CLASS


// =========================================================================================== APP PALETTE SINGLETON FOR USE

// Global singleton instance of app palette for easy access throughout the program
inline Global_palette& App_palette = Global_palette::Instance();

// =========================================================================================== APP PALETTE SINGLETON FOR USE
