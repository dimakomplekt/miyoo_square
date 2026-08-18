// my_sdl_element.h


#pragma once


// =========================================================================================== IMPORT

#include <functional>                                                   // for std::function
#include <cmath>                                                        // for std::round()


// #include "../../../../program_logic/global_data/global_data.h" 

#include "../../../../engine/engine.h"                                  // SDL3 and SDL ttf import

#include "../../GUI_functions/drawing/figures_drawing.h"                // Basic figures

#include "../../global_palette/global_palette.h"                        // Global palette for the default colors and HEX to SDL_Color translator

#include "../../global_fonts/global_fonts.h"                            // Global fonts for the default fonts and font path getters

// =========================================================================================== IMPORT


// =========================================================================================== DEFINES

#define DELTA_FOR_HOVER_CLICK_CHECKS 3  // 3px delta for the hover and click checks with better stability

// =========================================================================================== DEFINES


// =========================================================================================== TYPES

// Element signal type for GUI change logic
enum element_gui_type
{

    STATIC_ELEMENT_GUI,        // One color palette
    DYNAMIC_ELEMENT_GUI,       // Different color palettes for different extern logic

};


// ===== Movement =====


// Descartes coordinate for  SDL3 2D space.
struct desc_c_2D
{

    int x;    // Coordinate by x-axes (width).
    int y;    // Coordinate by y-axes (height).

};


// Enum for movement animation type define
enum movement_easing
{
    
    LINEAR,                      // Linear movement
    EXPONENTIAL,                 // Exponential movement
    LOGARITHMIC,                 // Logarithmic movement 
    BACKWARD_LOGARITHMIC,        // Logarithmic movement

};


struct movement_ctx
{

    bool active = false;

    desc_c_2D start_pos;
    desc_c_2D end_pos;

    Uint64 start_time;
    Uint64 duration;

    float progress = 0.0f;

    movement_easing easing;

    bool opacity_change = false;

    Uint8 start_opacity;
    Uint8 end_opacity;

};
// ===== Movement =====


// Element rectangle boundaries struct for logic like hover and click checks
struct element_rect_boundaries
{

    int left_boundary;
    int right_boundary;
    int top_boundary;
    int bottom_boundary;

};

// Element states for rendering logic
enum element_state
{

    DEFAULT_ES,
    HOVERED_ES,
    CLICKED_ES

};



/**
 * @brief Nine key anchor points of the image in local (not rotated) space.
 *
 * These points allow flexible alignment:
 *
 * 
 *  [TL]---[TC]---[TR]
 * 
 * 
 *  [CL]---[CC]---[CR]
 * 
 * 
 *  [BL]---[BC]---[BR]
 *
 * 
 * This is useful for positioning sprites relative to
 * characters, physics bodies, or UI layout.
 */
struct anchor_points {

    desc_c_2D top_left;
    desc_c_2D top_center;
    desc_c_2D top_right;
    desc_c_2D center_left;
    desc_c_2D center_center;
    desc_c_2D center_right;
    desc_c_2D bottom_left;
    desc_c_2D bottom_center;
    desc_c_2D bottom_right;

};


// =========================================================================================== TYPES


// =========================================================================================== HELPER-FUNCTIONS


bool hover_check_by_boundaries(const element_rect_boundaries& boundaries);


// =========================================================================================== HELPER-FUNCTIONS


// =========================================================================================== My_SDL_element class


// Predeclare the panel class for the upper_container atribute include
class My_SDL_panel; 


class My_SDL_element
{

    public:

        // ===== CONSTRUCTOR AND DESTRUCTOR =====
        
        /**
         * @brief Layered element delete - destructor call with two-way relationships, taking into account
         * 
         * DESTROY BY FABRIC - i can't just control the destructor cycle - it starts to delete smthing even before 
         * custom logic inside destructor, so i must wrapped it by some new function and use only it for basic memory clean
         * 
         * There is 3 levels of object destruct:
         * 
         * 1) Logic destruction - clears the element link from all of the lists, which include it (like vector of the inner 
         * components in My_SDL_panel class), turn off object iterative logic (like update() and render())
         * 
         * 2) Inner destruction - object must clear it's memory, close it's files / texts / buffers, nullptr it's links
         * 
         * 3) Physical destruction - delete, memory clean and destructor call
         * 
         */
        virtual void delete_element() = 0;

        
         // Element destructor - NEVER CALLS - PLS WORK WITH delete_element()
        virtual ~My_SDL_element() = default;

        // ===== CONSTRUCTOR AND DESTRUCTOR =====


        // ===== MAIN LOGIC =====

        // Virtual method for inheritance (to do something like: "for (auto& el : elements) el->update();")
        virtual void update() = 0;


        // Container-panel getter
        My_SDL_panel* get_element_container() const;

        
        // Element visible condition setter
        virtual void set_visible_flag(bool new_flag);

        // ===== MAIN LOGIC =====


        // ===== GUI =====


        /**
         * @brief Virtual render method for the UI-element
         * 
         */
        virtual void render(SDL_Renderer* renderer) = 0;


        /**
         * @brief Element render point setter
         * 
         * Setup the element center-center render point for the element rendering
         * by the coordinate system of the WINDOW, which contains the element 
         * 
         * 
         * @param x_cc_rp x coordinate of the center-center render point
         * @param y_cc_rp y coordinate of the center-center render point
         * 
         */
        virtual void set_render_point(int x_cc_rp, int y_cc_rp) = 0;


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

        );


        // Movement reset function
        void movement_reset();


        /**
         * @brief Returns the X coordinate of the element's render center.
         *
         * @return X coordinate (center of the element for rendering)
         * 
         */
        int get_x_render_point() const;


        /**
         * @brief Returns the Y coordinate of the element's render center.
         *
         * @return Y coordinate (center of the element for rendering)
         * 
         */
        int get_y_render_point() const;


        // Element anchor points getter function
        anchor_points get_anchor_points() const;
        

        /**
         * @brief Sets the element's global opacity.
         *
         * Updates the alpha value applied to all button visual elements,
         * including background, border, shadow, and content.
         * 
         * virtual - for logic change for the panel element (will call set_opacity(basic_opacity) 
         * for all inner elements - and in case if we got panel in panel it will become recursive setting)
         * and for the other elements, which can have some specific logic 
         * of the opacity change
         *
         * @param new_opacity Opacity value (0 = fully transparent, 255 = fully opaque)
         * 
         */
        virtual void set_opacity(Uint8 new_opacity);


        /**
         * @brief Get the element's global opacity.
         *
         * @return Opacity value (0 = fully transparent, 255 = fully opaque)
         * 
         */
        Uint8 get_opacity() const;


        /**
         * @brief Get the element's basic opacity.
         *
         * @return Opacity value (0 = fully transparent, 255 = fully opaque)
         * 
         */
        Uint8 get_basic_opacity() const;


        // Onetime autoset of the opacity inside the update() loops of element
        // and inside the set_opacity() method
        void recalculate_opacity_by_container();


        /**
         * @brief GUI type setter
         * 
         * Choose the gui type (static or dynamic) for the elementby the element_gui_type enum values
         * 
         * @param new_gui_type one of the two element_gui_type enum values
         * 
         */
        void set_gui_type(element_gui_type new_gui_type);


        // passed_by_palette flag switcher for the basic GUI elements color settings
        void switch_passed_by_palette_flag(bool new_status);


        // Virtual method for the color reset of the basic GUI elements, 
        // which colors are set by the palette, in case of the palette switch,
        // called inside the update() method of the element, if the palette_reset_flag
        // of the App_palette singleton is true and the element passed_by_palette flag is true
        virtual void reset_colors_if_palette_switched() = 0;

        // ===== GUI =====


    // Panel friendship for direct use of methods
    friend class My_SDL_panel;

    protected:

            // ===== CONSTRUCTOR AND DESTRUCTOR =====

            My_SDL_element();                                   // Element constructor (never calls, only for inheritance)
            
            // ===== CONSTRUCTOR AND DESTRUCTOR =====


            // ===== MAIN LOGIC =====

            My_SDL_panel* element_container = nullptr;          // NON-owning


            movement_ctx element_movement;                      // Element movement context

            virtual void movement_logic_in_update_loop();       // Element movement context update  


            bool visible_flag;                                  // Element visible flag (used unside render() to choose render or not)

            // ===== MAIN LOGIC =====


            // ===== GUI =====


            // GUI type
            element_gui_type gui_type;

            // Render points (center-center)

            // Center-center x-render point
            int x_render_point;

            // Center-center y-render point
            int y_render_point;


            // Element anchor points - uses for elements positioning by each other
            anchor_points element_anchor_points;


            // Element anchor points reset function - set up the new element_anchor_points struct values
            // by the current element inheritor type overrided function
            virtual void reset_anchor_points() = 0; 


            // Element opacity for SDL rendering with
            // calling of commands like:
            // SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, alpha);
            // Calculates differently by cases - fully equal to basic opacity if element ain't got container and
            // equal to (basic_opacity * container_opacity / 255)
            Uint8 opacity; // 0 = fully transperent, 255 = fully opaque

            // Opacity, passed to element by the set_opacity()
            Uint8 basic_opacity;


            // Flag for the basic GUI elements color settings by the palette (true 
            // if element colors are set by the palette, false if not)
            bool passed_by_palette;
            
            // ===== GUI =====
};
