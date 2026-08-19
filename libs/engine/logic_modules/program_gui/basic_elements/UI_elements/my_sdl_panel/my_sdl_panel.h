// my_sdl_panel.h


#pragma once


// =========================================================================================== IMPORT

#include <vector>                                   // For std::vector
#include "../my_sdl_element/my_sdl_element.h"       // Base class import

// =========================================================================================== IMPORT


// =========================================================================================== TYPES

// Struct for inner elements with local positioning
struct panel_inner_element
{

    My_SDL_element* element_pointer;

    int local_x_position;
    int local_y_position;

    unsigned int local_z_position;      // Current z for this element inside the panel

};


struct panel_render_data
{

    // Shadow render data
    int sw_cx;
    int sw_cy;
    unsigned int sw_w;
    unsigned int sw_h;
    unsigned int sw_r;

    // Border render data
    int br_cx;
    int br_cy;
    unsigned int br_w;
    unsigned int br_h;
    unsigned int br_r;

    // Background render data
    int bd_cx;
    int bd_cy;

    unsigned int bg_w;
    unsigned int bg_h;
    unsigned int bg_r;

};


// =========================================================================================== TYPES


// =========================================================================================== My_SDL_panel class

class My_SDL_panel : public My_SDL_element
{

    public:

        // ===== CONSTRUCTOR AND DESTRUCTOR =====
       
        My_SDL_panel();                            // Panel constructor.
        
        void delete_element() override;            // Panel delete (delete_element call for all links, clear links + destructor call)   

        // ===== CONSTRUCTOR AND DESTRUCTOR =====


        // ===== MAIN LOGIC =====

        /**
         * @brief Updates the panel and all its inner elements.
         *
         * Calls update() on all registered inner elements.
         * 
         */
        void update() override;


        /**
         * @brief Sets the panel's global opacity and change the elements opacity by it.
         *
         * Updates the alpha value applied to all button visual elements,
         * including background, border, shadow, and content.
         *
         * @param new_opacity Opacity value (0 = fully transparent, 255 = fully opaque)
         * 
         */
        void set_opacity(Uint8 new_opacity) override;


        // Element visible condition setter override for panel
        
        void set_visible_flag(bool new_flag) override;


        // ===== MAIN LOGIC =====


        // ===== PANEL SPECIFIC =====

        /**
         * @brief Adds an element to the panel at specified local coordinates.
         *
         * The element's position will be relative to the panel's center.
         * Call element render position setters by the element type. in dependence
         * of current panel position
         * 
         * Setup the parent_panel atribute link inside base class by this-> in implementation
         * 
         * Sorts the inner_elements list by the local-z increase inside. 
         *
         * @param element_pointer Pointer to the element to add
         * @param local_x Local X position relative to tl-point
         * @param local_y Local Y position relative to tl-point
         * @param local_z Local Z position relative to the current panel
         * 
         */
        void add_element(My_SDL_element* element_pointer, int local_x, int local_y, unsigned int local_z);


        /**
         * @brief Removes an element from the panel and call it's destructor.
         *
         * @param element_pointer Pointer to the element to remove
         * 
         */
        void remove_element(My_SDL_element* element_pointer);


        /**
         * @brief Clears all elements from the panel and call their destructors.
         * 
         * Just the iterative call of the remove_element(My_SDL_element* element_pointer) by vector
         * 
         */
        void clear_elements();


        /**
         * @brief Change the element coordinates by the new local coordinates inside the panel.
         *
         * The element's position will be relative to the panel's center.
         * Call element render position setters by the element type. in dependence
         * of current panel position
         * 
         * Sorts the inner_elements list by the local-z increase inside. 
         *
         * @param element_pointer Pointer to the element to add
         * @param new_local_x New local X position relative to tl-point
         * @param new_local_y New local Y position relative to tl-point
         * @param new_local_z New local Z position relative to the current panel
         * 
         */
        void change_element_local_coordinate(My_SDL_element* element_pointer, int new_local_x, int new_local_y, unsigned int new_local_z);

        // Inner method for global coordinate change
        void change_inner_elements_global_coordinates();

        // Global x-coordinate getter
        int global_x_by_local_x(int local_x);

        // Global y-coordinate getter
        int global_y_by_local_y(int local_y);


        // Local x-coordinate getter
        int local_x_by_global_x(int global_x);

        // Local y-coordinate getter
        int local_y_by_global_y(int global_y);


        // Movement override for panel
        void movement_logic_in_update_loop() override;

        // ===== PANEL SPECIFIC =====


        // ===== GUI =====

        /**
         * @brief Renders the panel and all its inner elements.
         *
         * Draws the panel's background, border, and shadow, then renders all inner elements
         * with their positions adjusted to the panel's coordinate system.
         *
         * @param renderer SDL renderer for drawing
         * 
         */
        void render(SDL_Renderer* renderer) override;


        /**
         * @brief Sets the panel's render point (center position).
         *
         * Updates the panel's position and adjusts the render points of all inner elements accordingly.
         *
         * @param x_cc_rp X coordinate of the center-center render point
         * @param y_cc_rp Y coordinate of the center-center render point
         * 
         */
        void set_render_point(int x_cc_rp, int y_cc_rp) override;


        /**
         * @brief Sets the panel's size.
         *
         * @param new_width New width of the panel in pixels
         * @param new_height New height of the panel in pixels
         * 
         */
        void set_size(unsigned int new_width, unsigned int new_height);


        /**
         * @brief Gets the panel's width.
         *
         * @return Width of the panel in pixels
         * 
         */
        unsigned int get_width_size() const;

        /**
         * @brief Gets the panel's height.
         *
         * @return Height of the panel in pixels
         * 
         */
        unsigned int get_height_size() const;


        /**
         * @brief Sets the border width.
         *
         * @param new_size New border width in pixels
         * 
         */
        void set_border_width_size(unsigned int new_size);


        /**
         * @brief Sets the border radius for rounded corners.
         *
         * @param new_radius_size New border radius in pixels
         * 
         */
        void set_border_radius(unsigned int new_radius_size);


        /**
         * @brief Sets the shadow offset.
         *
         * @param new_x_offset X offset for the shadow
         * @param new_y_offset Y offset for the shadow
         * 
         */
        void set_shadow_offset(int new_x_offset, int new_y_offset);


        /**
         * @brief Sets the shadow scale factor.
         *
         * @param new_scale_factor Scale factor for the shadow size
         * 
         */
        void set_shadow_scale_factor(float new_scale_factor);


        // Color setters

        // Sets the default background color for the panel.
        void set_panel_background_color(SDL_Color new_color);

        // Sets the default border color for the panel.
        void set_panel_border_color(SDL_Color new_color);

        // Sets the default shadow color for the panel.
        void set_panel_shadow_color(SDL_Color new_color);


        void reset_colors_if_palette_switched() override;

        // ===== GUI =====

    private:

        // ===== CONSTRUCTOR AND DESTRUCTOR =====

        // Destructor for My_SDL_panel.
        ~My_SDL_panel() = default;

        // ===== CONSTRUCTOR AND DESTRUCTOR =====


        // ===== MAIN LOGIC =====

        // Container for inner elements - fills by add_element() and used inside 
        // the update_elements_positions() and clear elements 
        std::vector<panel_inner_element> inner_elements;

        // Setup the new element position (atribute inside basic class) after operations, which could change them,
        // like element add inside the panel
        void update_inner_element_position(My_SDL_element* element_pointer, int local_x, int local_y);

        // Inner elements position update (calls with panel coordinates switch)
        void update_elements_positions();


        // Panel override for anchor points reset function

        void reset_anchor_points() override;

        // ===== MAIN LOGIC =====


        // ===== GUI =====

        // Panel dimensions

        unsigned int panel_width_size;
        unsigned int panel_height_size;


        // Styling attributes

        unsigned int border_width_size;
        unsigned int border_radius_size;

        int shadow_offset_x;
        int shadow_offset_y;

        float shadow_scale_factor;

        element_form current_form;                     // Current element form by the element_form enum

        void reset_current_form();                     // Automatic current form reset

        
        panel_render_data current_render_data;         // Current render data struct for the panel, which fills inside the render() method and uses for the panel rendering
        
        void render_data_reset();                      // Render data reset for the new render cycle

        // Colors

        SDL_Color background_color;
        SDL_Color border_color;
        SDL_Color shadow_color;


        SDL_Color render_background_color;
        SDL_Color render_border_color;
        SDL_Color render_shadow_color;


        // Palette prepare for rendering
        void panel_palette_prepare();  


        // ===== GUI =====

};

// =========================================================================================== My_SDL_panel class
