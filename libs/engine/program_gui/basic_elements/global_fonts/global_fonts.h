// global_fonts.h

#pragma once

// ==============================================s============================================= IMPORT

#include <string>
#include <vector>


// Absolute path by relative path from .exe folder (with autocopy of fonts inside the build folder), 
// for easy font path setup without hardcoding absolute paths on different machines

#include "../../../engine/engine.h"

// Predeclare trick for nullptr allow of ttf_font_link on compilation stage without including SDL_ttf header
// here, to avoid SDL_ttf dependency in the header file and possible issues with that in the future
struct TTF_Font;

// =========================================================================================== IMPORT

// Struct for font data, which contains font path and SDL font link
struct app_font_ctx
{

    std::string font_path;                         // Font path for text

    // SDL TTF font link, used for rendering text with SDL_ttf
    // Initialized to nullptr, should be set to a valid TTF_Font* before use

    TTF_Font* ttf_font_link = nullptr;             // TTF Font pointer (void* to avoid SDL_ttf dependency in header)

    
    unsigned int font_size;                        // TTF font size
};


// App textbox type enum
// use for UI text palette choose inside textbox element
enum app_textbox_type
{

    HEADER_1,
    HEADER_2,
    HEADER_3,
    ORDINARY_TEXT,
    SMALL_TEXT,
    BUTTON_TEXT,
    EMOJI_TEXT,

    NO_TYPE,

    LIMIT

};

// Struct for app fonts, which contains all basic fonts for basic UI
struct app_fonts_palette_ctx
{

    app_font_ctx header_1_font;
    
    app_font_ctx header_2_font;
    
    app_font_ctx header_3_font;
    
    app_font_ctx ordinary_text_font;

    app_font_ctx small_text_font;

    app_font_ctx button_text_font;

    app_font_ctx emoji_text_font;

};

// =========================================================================================== APP FONTS SINGLETON CLASS


/**
 * @brief Singleton class that holds the fonts structure and current font palette select.
 *
 * Ensures there is only one global instance of app palette.
 * 
 * Provides methods to set the font palettes list, get the current fonts palette 
 * and switch current fonts palette to the specific palette or next palette inside the palettes
 * list.
 *
 */
class Global_fonts
{

    public:

        // ===== Instance =====

        /**
         * @brief Returns the singleton instance.
         *
         * Guarantees a single global instance of Global_palette.
         * 
         */
        static Global_fonts& Instance();

        // ===== Instance =====


        // ===== Fonts workflow =====

        // Automatically initializes / reinitializes the SDL_TTF fonts in the main update loop, 
        // if the palette reset flag is set 
        void fonts_management_in_update_loop();

        /**
         * @brief Adds a fonts palette to the internal fonts palette list.
         *
         * @param new_font_palette Fonts palette data to be stored.
         *
         * Palette is copied into internal std::vector storage.
         * No dynamic allocation or ownership transfer is involved.
         * 
         */
        void add_fonts_palette(const app_fonts_palette_ctx& new_fonts_palette);


        /**
         * @brief Gets the current active fonts palette.
         *
         * @return Const reference to the current fonts palette.
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
        const app_fonts_palette_ctx& get_current_fonts_palette() const;


        /**
         * @brief Switches to the next fonts palette in the list.
         *
         * Cycles through available palettes.
         * Wraps to the first palette when the end is reached.
         *
         * Also sets reset flag to notify UI about palette change.
         * 
         */
        void switch_to_the_next_font_palette();


        /**
         * @brief Switches to a specific fonts palette by id.
         *
         * @param palette_id Index of fonts palette in internal list.
         *
         * Sets current palette if id is valid.
         * Preferred over pointer-based access to avoid invalid memory references.
         *
         * Also sets reset flag to notify UI about palette change.
         * 
         */
        void switch_to_the_fonts_palette(unsigned int palette_id);

        // ===== Palette workflow =====


        // ===== Palette reset =====

        /**
         * @brief Gets the palette reset flag.
         *
         * @return true if the palette has been reset, false otherwise.
         * 
         */
        bool get_fonts_palette_reset_flag() const;

        // Function, called inside the main loop, before state loop
        // sets palette_reset_flag to false after 2 loops, to avoid constant resettings.
        // Called inside SDL_app_cycle(SDL_app_ctx* app) in app.cpp
        void fonts_palette_reset_flag_state_loop_update();

        // ===== Palette reset =====
        

        private:

        // ===== Constructor and Destructor =====
        
        // Private constructor ensures no external instances can be created.
        Global_fonts();

        // Default destructor is fine; no dynamic allocation to clean up.
        ~Global_fonts() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        Global_fonts(const Global_fonts&) = delete;

        // Assignment operator is deleted to prevent copying the singleton.
        Global_fonts& operator=(const Global_fonts&) = delete;

        // ===== Constructor and Destructor =====


        // ===== Palettes =====

        // Palettes list
        std::vector<app_fonts_palette_ctx> fonts_palettes_list;


        // Currently active palette ID (by palettes list)
        unsigned int current_fonts_palette_id;

        // ===== Palettes =====


        // ===== Reset logic =====

        // Flag to indicate if the palette has been reset (for UI updates, etc.)
        bool fonts_palette_reset_flag;
        bool fonts_palette_init_flag;

        // Counter to track how many loops have passed since the palette reset flag was set
        unsigned int fonts_palette_reset_flag_loops_counter;

        // ===== Reset logic =====
};

// =========================================================================================== APP FONTS SINGLETON CLASS


// =========================================================================================== APP FONTS PALETTE SINGLETON FOR USE

// Global singleton instance of app palette for easy access throughout the program
inline Global_fonts& App_fonts = Global_fonts::Instance();

// =========================================================================================== APP FONTS PALETTE SINGLETON FOR USE
