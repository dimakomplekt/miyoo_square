// my_sdl_texture.h

#pragma once

// =========================================================================================== IMPORT

#include "../my_sdl_element/my_sdl_element.h" // Base class import

// =========================================================================================== IMPORT


// =========================================================================================== MY_SDL_TEXTURE


class My_SDL_texture : public My_SDL_element // SDL_Element
{
    public:


        // ===== CONSTRUCTOR AND DESTRUCTOR =====

        My_SDL_texture();                           // Texture constructor

        // TODO: model with ownership of texture and without ownership
        
        void delete_element() override;             // Texture delete (clear links + destructor call)   

        ~My_SDL_texture();                          // Texture destructor

        // ===== CONSTRUCTOR AND DESTRUCTOR =====   



        // ===== MAIN LOGIC =====

        void update() override;  // Nothing now


        /**
         * @brief Sets the texture, which would displayed with render().
         *
         * Updates the texture (with old texture delete) and reset sizes, then calls the
         * anchor points reset
         *
         * @param new_texture New texture to display
         * @param take_ownership Texture ownership status for current object
         * 
         */
        void set_texture(SDL_Texture* new_texture, bool take_ownership);


        /**
         * @brief Sets the texture, which would displayed with render().
         *
         * Updates the texture (with old texture delete) and reset sizes, then calls the
         * anchor points reset
         *
         * @param link Global link to the image which will be translated into texture
         * @param renderer Renderer for texture
         * 
         */
        void My_SDL_texture::set_texture_by_image(const std::string& link, SDL_Renderer* renderer);


        /**
         * @brief Sets the texture owning flag
         *
         * Switches the texture control mode by My_SDL_Texture
         * If the flag is true - Object fully controlling texture
         * otherwise - texture is free and object can't deallocate it
         *
         * @param new_flag New texture owning status
         *          
         */
        void switch_texture_owning_flag(bool new_flag);

        // ===== MAIN LOGIC =====


        // ===== GUI ======

        // Renders the texture, based on its current state and visual configuration.
        void render(SDL_Renderer* renderer) override; 


        // Render point

        /**
         * @brief Texture render point setter
         * 
         * Setup the texture center-center render point for the texture rendering
         * by the coordinate system of the WINDOW, which contains the element 
         * 
         * After that - reset anchor points.
         * 
         * @param x_cc_rp x coordinate of the center-center render point
         * @param y_cc_rp y coordinate of the center-center render point
         * 
         */
        void set_render_point(int x_cc_rp, int y_cc_rp) override;



        /**
         * @brief Returns the texture's basic width.
         *
         * @return Width of the texture in pixels
         * 
         */
        int get_basic_width_size() const;


        /**
         * @brief Returns the texture's basic height.
         *
         * @return Height of the texture in pixels
         * 
         */
        int get_basic_height_size() const;



        /**
         * @brief Returns the texture's width.
         *
         * @return Width of the texture in pixels
         * 
         */
        int get_width_size() const;


        /**
         * @brief Returns the texture's height.
         *
         * @return Height of the texture in pixels
         * 
         */
        int get_height_size() const;


        // Resize

        /**
         * @brief Sets the texture new x-scaler, recalculates sizes and reset anchor points.
         *
         * @param new_x_scaler New float x-scaler
         * 
         */
        void set_x_scaler(float new_x_scaler);

        /**
         * @brief Sets the texture new y-scaler, recalculates sizes and reset anchor points.
         *
         * @param new_y_scaler New float x-scaler
         * 
         */
        void set_y_scaler(float new_y_scaler);


        /**
         * @brief Sets the texture new scalers, recalculates sizes and reset anchor points.
         *
         * @param new_x_scaler New float x-scaler
         * @param new_y_scaler New float x-scaler
         * 
         */
        void set_scalers(float new_x_scaler, float new_y_scaler);



        // x-scaler getter
        float get_x_scaler() const;

        // y-scaler getter
        float get_y_scaler() const;


        /**
         * @brief Sets the texture size, recalculate scalers and reset anchor points.
         *
         * @param new_width New width of the texture in pixels
         * @param new_height New height of the texture in pixels
         * 
         */
        void set_width(int new_width);

        /**
         * @brief Sets the texture size, recalculate scalers and reset anchor points.
         *
         * @param new_height New height of the texture in pixels
         * 
         */
        void set_height(int new_height);

        /**
         * @brief Sets the texture size, recalculate scalers and reset anchor points.
         *
         * @param new_width New width of the texture in pixels
         * @param new_height New height of the texture in pixels
         * 
         */
        void set_size(unsigned int new_width, unsigned int new_height);


        // Return the basic texture size (without scale) - just for some logic, which needs to know the size of the texture, but not to render it with this size
        void reset_size();


        void reset_colors_if_palette_switched() override;

        // ===== GUI ======

        
    protected:

        // ===== GUI ======

        // Texture for rendering
        SDL_Texture* texture;

        bool owns_texture;


        // Variables for rendering with autoset 
        
        int basic_width_size;  
        int basic_height_size;

        // Resize 

        int width_size;  
        int height_size;

        float x_scaler;
        float y_scaler;



        // Texture's override for anchor points reset (by current sizes) function
        
        void reset_anchor_points() override;


        // ===== GUI ======

};

// =========================================================================================== MY_SDL_TEXTURE
