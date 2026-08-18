// my_sdl_button.h


#pragma once


// =========================================================================================== IMPORT

#include "../my_sdl_element/my_sdl_element.h"   // Base class import

#include "../my_sdl_textbox/my_sdl_textbox.h"   // Textbox import


// =========================================================================================== IMPORT



// =========================================================================================== TYPES

// Button access type for click callback logic permission
enum button_access_type
{

    BUTTON_DEFAULT_CLICK_PERMISSION,        // Always can be button_clicked
    BUTTON_EXTERN_CLICK_PERMISSION,         // Can be button_clicked only by the true return of the extern function for click permission check

};

struct button_render_data
{
    // Centers
    int sw_cx, sw_cy;   // shadow
    int br_cx, br_cy;   // border
    int bd_cx, bd_cy;   // background

    // Sizes
    unsigned int sw_w, sw_h;
    unsigned int br_w, br_h;
    unsigned int bg_w, bg_h;

    // Radiuses
    unsigned int sw_r, br_r, bg_r;

};

// =========================================================================================== TYPES


// =========================================================================================== My_SDL_button class

class My_SDL_button : public My_SDL_element // SDL_Element
{

    public:

        // ===== CONSTRUCTOR AND DESTRUCTOR =====

        My_SDL_button();                            // Button constructor

        void delete_element() override;             // Button delete (clear links + destructor call)   

        // ===== CONSTRUCTOR AND DESTRUCTOR =====


        // ===== MAIN LOGIC =====


        /**
         * @brief Updates the button.
         *
         * - Performs hover detection and updates hover state
         * - Handles click press/release logic
         * - Applies click access control (default or external permission)
         * - Triggers hover and click callbacks when conditions are met
         * - Ensures single-click behavior (prevents repeat while holding)
         * - Updates visual state (DEFAULT / HOVERED / button_clicked)
         * - Applies palette change if required
         *
         * Must be called every frame inside the main state.update loop.
         * 
         */
        void update() override;                                  


        /**
         * @brief Button access type setter 
         * 
         * Setup the button access type for click permission logic by the
         * button_access_type enum values
         * 
         * @param new_access_type - one of the two button_access_type enum values 
         */
        void set_access_type(button_access_type new_access_type);


        // Toggles the push mode flag (affects pressed-state rendering behavior)
        void switch_push_mode();

        // Switches the button's textbox type without crushes
        void switch_button_textbox_type(app_textbox_type new_type);


        /**
         * @brief External callback for click permission validation.
         *
         * This callback is used when the button access type is set to
         * BUTTON_EXTERN_CLICK_PERMISSION. It must return true if the button
         * is allowed to process a click, or false otherwise.
         *
         * Can be used to implement custom access logic (e.g. state checks,
         * conditions from other UI elements, game logic, etc.).
         *
         * Expected usage:
         * - Assign a function/lambda returning bool
         * - The function will be called during update() when a click occurs
         *
         * Must be set when using external click permission, otherwise
         * click handling will fail.
         * 
         */
        std::function<bool()> extern_click_permission; 


        /**
         * @brief Callback triggered when the button is hovered.
         *
         * Allows attaching custom external logic that will be executed
         * while the cursor is over the button.
         *
         * Can be used for visual effects, sound, tooltips, or any
         * additional hover-related behavior.
         *
         * Called during update() when the button is in hovered state.
         * 
         */
        std::function<void()> on_hover;                    

        
        /**
         * @brief Click callback (single-shot).
         *
         * Executed once after a complete click action (press + release).
         * The callback is triggered only once per click and is blocked
         * until the next valid click occurs.
         *
         * Used to attach custom logic to button activation.
         *
         * Called during update() on click release.
         * 
         */
        std::function<void()> on_click;     
            

        // ===== MAIN LOGIC =====


        // ===== GUI ======

        /**
         * @brief Renders the button based on its current state and visual configuration.
         *
         * - Selects the active palette (static or dynamic)
         * - Resolves colors for current state (DEFAULT / HOVERED / button_clicked)
         * - Applies global opacity to all visual components
         * - Calculates geometry for shadow, border, and background
         * - Simulates button press effect using offset (push mode)
         * - Renders shape (rectangle / rounded rectangle / circle)
         * - Updates and renders content (text/texture) centered inside the button
         *
         * Rendering depends on state flags updated in update().
         * 
         */
        void render(SDL_Renderer* renderer) override;


        // Render point

        /**
         * @brief Button render point setter
         * 
         * Setup the button center-center render point for the button rendering
         * by the coordinate system of the WINDOW, which contains the element 
         * 
         * 
         * @param x_cc_rp x coordinate of the center-center render point
         * @param y_cc_rp y coordinate of the center-center render point
         * 
         */
        void set_render_point(int x_cc_rp, int y_cc_rp) override;


        // Size - unique logic for every element

        /**
         * @brief Sets the button size.
         *
         * Updates the button's width and height for rendering and layout.
         *
         * @param new_width New width of the button in pixels
         * @param new_height New height of the button in pixels
         * 
         */
        void set_size(unsigned int new_width, unsigned int new_height);


        /**
         * @brief Returns the button's width.
         *
         * @return Width of the button in pixels
         * 
         */
        unsigned int get_width_size() const;


        /**
         * @brief Returns the button's height.
         *
         * @return Height of the button in pixels
         * 
         */
        unsigned int get_height_size() const;


        // Styling

        /**
         * @brief Sets the border width of the button.
         *
         * Updates the button's border width while ensuring it does not exceed
         * half of the button's width or height, and does not exceed the border radius.
         * 
         * If an invalid value is passed, the width is not changed and an error is logged.
         *
         * @param new_size New border width in pixels
         * 
         */
        void set_border_width_size(unsigned int new_size);


        /**
         * @brief Sets the border radius of the button.
         *
         * Updates the corner radius while ensuring it does not exceed
         * half of the button's width or height.
         * 
         * If an invalid value is passed, the radius is not changed and an error is logged.
         * Also resets the current form to apply the new radius correctly.
         *
         * @param new_size New border radius in pixels
         * 
         */
        void set_border_radius(unsigned int new_size);


        /**
         * @brief Sets the shadow offset of the button.
         *
         * Updates the horizontal and vertical displacement of the button's shadow
         * relative to the button's position.
         *
         * @param new_x_offset Horizontal shadow offset in pixels
         * @param new_y_offset Vertical shadow offset in pixels
         * 
         */
        void set_shadow_offset(int new_x_offset, int new_y_offset);
        

        /**
         * @brief Sets the shadow scale factor.
         *
         * Adjusts the scaling of the shadow relative to the button's size.
         *
         * @param new_scale_factor Shadow scale multiplier (e.g., 1.0 = normal size)
         * 
         */
        void set_shadow_scale_factor(float new_scale_factor);


        // Color setters

        // Palette 1 - basic button
        
        // Sets the default background color for palette 1
        void set_basic_background_color(SDL_Color new_color);

        // Sets the default border color for palette 1
        void set_basic_border_color(SDL_Color new_color);

        // Sets the default content color for palette 1
        void set_basic_content_color(SDL_Color new_color);

        // Sets the default shadow color for palette 1
        void set_basic_shadow_color(SDL_Color new_color);

        // Sets the hovered background color for palette 1
        void set_hover_background_color(SDL_Color new_color);

        // Sets the hovered border color for palette 1
        void set_hover_border_color(SDL_Color new_color);

        // Sets the hovered content color for palette 1
        void set_hover_content_color(SDL_Color new_color);

        // Sets the hovered shadow color for palette 1
        void set_hover_shadow_color(SDL_Color new_color);

        // Sets the button_clicked background color for palette 1
        void set_click_background_color(SDL_Color new_color);

        // Sets the button_clicked border color for palette 1
        void set_click_border_color(SDL_Color new_color);

        // Sets the button_clicked content color for palette 1
        void set_click_content_color(SDL_Color new_color);

        // Sets the button_clicked shadow color for palette 1
        void set_click_shadow_color(SDL_Color new_color);



        // Palette 2 - controlled access 

        // Sets the access denied default background color for palette 2
        void set_access_denied_background_color(SDL_Color new_color);

        // Sets the access denied default border color for palette 2
        void set_access_denied_border_color(SDL_Color new_color);

        // Sets the access denied default content color for palette 2
        void set_access_denied_content_color(SDL_Color new_color);

        // Sets the access denied default shadow color for palette 2
        void set_access_denied_shadow_color(SDL_Color new_color);


        // Sets the access permitted default background color for palette 2
        void set_access_permitted_background_color(SDL_Color new_color);

        // Sets the access permitted default border color for palette 2
        void set_access_permitted_border_color(SDL_Color new_color);

        // Sets the access permitted default content color for palette 2
        void set_access_permitted_content_color(SDL_Color new_color);

        // Sets the access permitted default shadow color for palette 2
        void set_access_permitted_shadow_color(SDL_Color new_color);

        // Sets the access permitted hovered background color for palette 2
        void set_access_permitted_hover_background_color(SDL_Color new_color);

        // Sets the access permitted hovered border color for palette 2
        void set_access_permitted_hover_border_color(SDL_Color new_color);

        // Sets the access permitted hovered content color for palette 2
        void set_access_permitted_hover_content_color(SDL_Color new_color);

        // Sets the access permitted hovered shadow color for palette 2
        void set_access_permitted_hover_shadow_color(SDL_Color new_color);

        // Sets the access permitted button_clicked background color for palette 2
        void set_access_permitted_click_background_color(SDL_Color new_color);

        // Sets the access permitted button_clicked border color for palette 2
        void set_access_permitted_click_border_color(SDL_Color new_color);

        // Sets the access permitted button_clicked content color for palette 2
        void set_access_permitted_click_content_color(SDL_Color new_color);

        // Sets the access permitted button_clicked shadow color for palette 2
        void set_access_permitted_click_shadow_color(SDL_Color new_color);



        void reset_colors_if_palette_switched() override;

        // Resets the button textbox if the font palette switched, to update the font link and path 
        // if they are setted by the font palette
        void reset_button_textbox_if_font_palette_switched(); 

        // Switch the flag for showing that the button textbox font path and ttf_font_link setted by the font palette
        void button_textbox_font_passed_by_font_palette_flag_switch(bool new_flag);

        // Button textbox getter 
        My_SDL_textbox* get_button_content_textbox();



        /**
         * @brief Element movement setter
         * 
         * Moves the element from point to point in every frame (until it reaches
         * the end of trajectory) by element_movement
         * 
         * 
         * @param x_cc_rp x coordinate of the movement end center-center render point
         * @param y_cc_rp y coordinate of the movement end center-center render point
         * @param easing  type of movement by the movement_easing enum
         * @param movement_time movement time in ms
         * 
         */
        virtual void move_to_point(
            
            int new_x_cc_rp, 
            int new_y_cc_rp,

            movement_easing easing,

            Uint64 movement_time

        ) override;


        // ===== GUI ======s


    private:

        // ===== CONSTRUCTOR AND DESTRUCTOR =====

        ~My_SDL_button();                           // Button destructor  (call in state.exit)  

        void cleanup();                             // Non palette-font destructor

        // ===== CONSTRUCTOR AND DESTRUCTOR =====


        // ===== MAIN LOGIC =====

        // Only onetime click realization in this version

        // Control flags

        button_access_type click_access_type;        // Button click access type for click callback logic permission


        bool click_permission;                       // Button click permission flag


        bool button_hovered;                         // Button hover state
        void button_hover_check();                   // Button hover check method (to be called in the main loop)


        bool button_clicked;                         // Button click state
        bool button_clicked_tmp;                     // Button click state temp for callback block until the next click
        
        bool push_mode_on;                           // Current push display mode
        int press_offset;                            // Current press offset for push animation

        // Data for press offset
        TTF_Font* po_base_font;
        int po_base_size;
        bool po_base_font_owned;    // true - the base font was owned (self-opened) by the textbox before the press
        bool po_cached;


        element_state current_button_state;
        element_state prev_button_state;

        // ===== MAIN LOGIC =====
        

        // ===== GUI ======

        unsigned int prev_palette_number;    // 1 or 2
        unsigned int current_palette_number; // 1 or 2


        // Sizes

        unsigned int button_width_size;                // Element width
        unsigned int button_height_size;               // Element height

        unsigned int border_width_size;                // Element border width          
        unsigned int border_radius_size;               // Element border radius 


        int shadow_offset_x;                           // Element x-shadow offset            
        int shadow_offset_y;                           // Element y-shadow offset        

        float shadow_scale_factor;                     // Shadow scale factor - multiplies basic element size to get shadow size

        
        element_rect_boundaries boundaries_points;     // Element rectangle bounds by the element_rect_boundaries struct

        void reset_boundaries_points();                // Element bounds automatic recalculation           


        element_form current_form;                     // Current element form by the element_form enum

        void reset_current_form();                     // Automatic current form reset


        // Content (composition)
        My_SDL_textbox button_textbox;

        // Flag for showing that the button textbox font path and ttf_font_link setted by the font palette
        bool button_textbox_font_passed_by_font_palette;

        
        // Button override for anchor points reset function
        
        void reset_anchor_points() override;


        // Button basic colors by SDL type  

        SDL_Color basic_background_color;
        SDL_Color basic_border_color;
        SDL_Color basic_content_color;
        SDL_Color basic_shadow_color;
        
        SDL_Color hover_background_color;
        SDL_Color hover_border_color;
        SDL_Color hover_content_color;
        SDL_Color hover_shadow_color;

        SDL_Color click_background_color;
        SDL_Color click_border_color;
        SDL_Color click_content_color;
        SDL_Color click_shadow_color;


        // Button additional colors for different access states

        SDL_Color access_denied_background_color;
        SDL_Color access_denied_border_color;
        SDL_Color access_denied_content_color;
        SDL_Color access_denied_shadow_color;


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


        // Variables for final rendering
        SDL_Color render_background_color;
        SDL_Color render_border_color;
        SDL_Color render_content_color;
        SDL_Color render_shadow_color;


        // Inner palette prepare method
        void button_palette_prepare();
        

        button_render_data render_data;


        void render_data_recalculation();

        // ===== GUI ======
};


// =========================================================================================== My_SDL_button class
