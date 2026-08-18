// my_sdl_textbox.h


#pragma once


// =========================================================================================== IMPORT

#include "../my_sdl_element/my_sdl_element.h" // Base class import

#include "../../global_fonts/global_fonts.h" // Font types

// =========================================================================================== IMPORT


// =========================================================================================== TYPES

// Stucture for blinking element implementation
struct blinking_textbox_ctx
{

    bool blinking_mode_on;

    Uint64 blinking_period;
    float blinking_duty;

    Uint64 active_time;
    Uint64 phase;

    bool active_now;

};

// =========================================================================================== TYPES


// =========================================================================================== My_SDL_textbox class

class My_SDL_textbox : public My_SDL_element // SDL_Element
{
    public:

        // ===== CONSTRUCTOR AND DESTRUCTOR =====

        My_SDL_textbox();                           // Text constructor

        ~My_SDL_textbox();                          // Text destructor  (call in state.exit)  

        void delete_element() override;             // Text delete (clear links + destructor call)   

        void cleanup();                             // Text cleanup (clear links + destructor call) for textboxes inside buttons (call in button.remove_element())  

        // ===== CONSTRUCTOR AND DESTRUCTOR =====   


        // ===== MAIN LOGIC =====

        void update() override;


        void switch_textbox_type(app_textbox_type new_type);

        app_textbox_type get_textbox_type() const;

        app_textbox_type get_prev_textbox_type() const;

        // ===== MAIN LOGIC =====


        // ===== GUI ======


        /**
         * @brief Renders the text, based on its current state and visual configuration.
         *
         * 
         */
        void render(SDL_Renderer* renderer) override; 


        // Render point

        /**
         * @brief Text render point setter
         * 
         * Setup the text center-center render point for the text rendering
         * by the coordinate system of the WINDOW, which contains the element 
         * 
         * After that - reset anchor points.
         * 
         * 
         * @param x_cc_rp x coordinate of the center-center render point
         * @param y_cc_rp y coordinate of the center-center render point
         * 
         */
        void set_render_point(int x_cc_rp, int y_cc_rp) override;


        /**
         * @brief Returns the text's width.
         *
         * @return Width of the text in pixels
         * 
         */
        unsigned int get_width_size() const;


        /**
         * @brief Returns the text's height.
         *
         * @return Height of the text in pixels
         * 
         */
        unsigned int get_height_size() const;


        /**
         * @brief Sets the text displayed on the text.
         *
         * Updates the text's content string and reset content sizes, then calls the
         * anchor points reset
         *
         * @param new_text New text to display on the text
         * 
         */
        void set_content(const std::string& new_text);



        // Switches the flag for showing that the font path and ttf_font_link setted by the font palette, 
        // automatically set to true inside the class constructor
        void switch_passed_by_font_palette_flag(bool new_flag);

        // Returns passed by font palette status
        bool get_passed_by_font_palette_flag() const;


        /**
         * @brief Sets the font file path for the text.
         *
         * Updates the path to the font file used for text rendering.
         * If the string is empty, the font path is not set and an error is logged.
         *
         * @param new_font_path Path to the font file
         * 
         */
        void set_font_path(const std::string& new_font_path);


        // Font path getter function
        std::string get_font_path() const;


        /**
         * @brief Sets the TTF font link for the text.
         *
         * Assigns a pointer to a TTF_Font used for rendering text.
         * If the pointer is null, the font is not set and an error is logged.
         *
         * @param new_ttf_font_link Pointer to a valid TTF_Font
         * 
         */
        void set_ttf_font_link(TTF_Font* new_ttf_font_link);


        /**
         * @brief Sets the font size for the text.
         *
         * Updates the font size used for rendering. Must be greater than 0.
         * If zero is passed, the font size is not changed and an error is logged.
         *
         * @param new_size Font size in points
         * 
         */
        void set_font_size(unsigned int new_size);


        /**
         * @brief Font size getter.
         *
         * Returns the font size value.
         *
         * @return Current font size value
         * 
         */
        unsigned int get_font_size() const;


        /**
         * Content color setter
         *                       
         * For elements with dynamic content color textboxes should                       
         * call switch_passed_by_palette_flag(false) for inner textbox               
         * and switch content color by the pallet or not by the palette by themselves
         * for double calls protection   
         *           
         */                                  
        void set_content_color(SDL_Color new_color);

        /**
         * Content color setter for palette switch logic
         *                       
         * For elements with dynamic content color textboxes should                       
         * call switch_passed_by_palette_flag(false) for inner textbox               
         * and switch content color by the pallet or not by the palette by themselves
         * for double calls protection   
         *           
         */                                  
        void set_content_color_if_palette_switched(SDL_Color new_color);


        
        // Texture setters

        // Sets the content texture of the text (replaces content color or text rendering)
        void set_content_texture(SDL_Texture* new_texture);

        
        /**
         * Content color autoset by app palette
         *                       
         * Elements with dynamic content color  textboxes (like buttons) should                       
         * call switch_passed_by_palette_flag(false) for inner textbox               
         * and switch content color by the pallet or not by the palette by themselves
         * for double calls protection   
         *           
         */       
        void reset_colors_if_palette_switched() override;


        /**
         * @brief Blinking textbox status setter
         * 
         * Setup the textbox as blinking or not blinking
         * at the renderer
         * 
         */
        void switch_blinking_mode_flag(bool new_flag);


        /**
         * @brief Blinking period setter
         * 
         * Setup the textbox blinking period time
         * 
         * @param new_blinking_period Time of the blinking cycle in ms
         * 
         */
        void set_blinking_period(Uint64 new_blinking_period);

        /**
         * 
         * @brief Blinking duty percentage setter
         * 
         * Setup the textbox blinking duty cycle percentage
         * 
         * @param new_blinking_duty % / 100 - (value from 0 to 1) of active time 
         * 
         */
        void set_blinking_duty(float new_blinking_duty);


        // ===== GUI ======


        // Friend classes access permission (for example - change something by call from button)
        
        friend class My_SDL_button;

    protected:

        // ===== MAIN LOGIC =====

        // Textbox type
        app_textbox_type textbox_type;

        // Previous textbox type
        app_textbox_type prev_textbox_type;

        bool textbox_type_changed;
        

        void update_font();


        // Font ownership setters - the ONLY valid ways to change ttf_font_link inside the class code

        /**
         * @brief Sets a SHARED (palette-owned) font as the current font.
         *
         * Closes the previously owned font (if there was one), then links the shared font
         * without taking ownership. Shared fonts are NEVER closed by the textbox.
         *
         * @param shared_font Pointer to a font, owned by the App_fonts palette (or another external owner)
         *
         */
        void set_shared_font(TTF_Font* shared_font);

        /**
         * @brief Sets an OWNED (self-opened) font as the current font.
         *
         * Closes the previously owned font (if there was one), then links the new font
         * with taking ownership. The owned font closes on the next font change or inside cleanup().
         *
         * @param new_owned_font Pointer to a font, opened by the textbox (or by the button around it)
         *
         */
        void set_owned_font(TTF_Font* new_owned_font);

        // ===== GUI ======

        // Only one string realization in this version (Time isn't on my side... No it isn't! (c))

        // Content

        std::string font_path;                         // Font path for text

        TTF_Font* ttf_font_link = nullptr;             // TTF Font pointer

        TTF_Font* owned_font = nullptr;                // Font, OWNED by this textbox (self-opened by TTF_OpenFont); nullptr - when ttf_font_link is a SHARED (palette-owned) font. Invariant: owned_font is nullptr or equal to ttf_font_link

        bool passed_by_font_palette;                   // Flag for showing that the font path and ttf_font_link setted by the font palette, automatically set to true inside the basic class constructor

        unsigned int font_size;                        // Content size


        std::string content;                           // Text


        // Variables for rendering with autoset 
        
        bool textbox_sizes_initiation_update_flag;
        mutable int content_width;  
        mutable int content_height;


        // Text's override for anchor points reset function
        
        void reset_anchor_points() override;


        // Flag for render recalculation after new content or new content settings set
        mutable bool content_dirty;


        // Text texture for rendering
        SDL_Texture* content_texture;


        /**
         * @brief Updates the text texture.
         * 
         * Calls inside render
         * 
         * Creates a new SDL_Texture from the current content string, font, and specified color.
         * If the content is unchanged or no font/text is set, the function does nothing.
         * Any existing texture is destroyed before creating a new one.
         * The resulting texture dimensions are stored in content_width and content_height.
         * Marks the content as clean (content_dirty = false).
         * 
         */
        void update_content_texture(SDL_Renderer* renderer, SDL_Color new_color);

        // Color for text rendering
        SDL_Color content_render_color;

        // Text drawing inner function
        void text_draw(SDL_Renderer* renderer);


        // Blinking mode context
        blinking_textbox_ctx blinking_mode_context;


        // ===== GUI ======

};

// =========================================================================================== My_SDL_textbox class
