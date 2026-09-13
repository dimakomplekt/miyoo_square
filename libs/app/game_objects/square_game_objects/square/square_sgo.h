// square_sgo.h


#pragma once

// =========================================================================================== IMPORT


#include "../../../../engine/engine.h"

#include "../../../global_data/global_data.h"

#include "../../objects_movement/objects_movement.h"

#include <array>
#include <vector>

// =========================================================================================== IMPORT



/*
    NOTE

        Call square_assets_init() before any square objects creation

        Call square_assets_clear(); after square objects using end

        If you called square_assets_clear(); and need objects once again -
        return to the step 1

*/


// =========================================================================================== CLASS ASSET's

void square_assets_init();
void square_assets_clear();


extern handle_ctx square_1_ah;

extern handle_ctx square_2_ah;


// =========================================================================================== CLASS ASSET's


// =========================================================================================== CLASS HELPERS


/**
 * @brief Square motion states
 * 
 * Exists to decide what actions to implement
 * with square data and assets handles
 * 
 */
enum square_motion_state
{

    MOVING_SMS,
    COMPRESSING_SMS,
    RECOVERING_SMS,
    BOUNCING_SMS

};


struct square_motion_ctx
{
    float max_size_scaler = 1.0;
    float min_size_scaler = 0.3;

    float compression_speed = 2.5f;
    float recovery_speed = 5.0f;
    float bounce_speed = 250.0f;
    float bounce_distance = 150.0f;
    float bounce_remaining = 0.0f;
    float bounce_sub_pixel = 0.0f;

    float curr_size_scaler = 1.0;

    int compressed_direction = 0;
    int contact_edge_x = 0;
};


// =========================================================================================== CLASS HELPERS


// =========================================================================================== SQUARE CLASS

class Square_sgo
{

    public:


        // ===== Lifetime =====

        Square_sgo();

        Square_sgo(const Square_sgo& other);

        ~Square_sgo();
        
        // ===== Lifetime =====


        // ===== Methods =====

        void motion_processing(collision_result outer_check);


        square_motion_state get_motion_state() const;

        void set_render_point(int x, int y);

        void set_render_point_by_delta(int dx, int dy);


        int get_x_render_point() const;

        int get_y_render_point() const;


        void set_size(unsigned int width, unsigned int height);


        unsigned int get_width();

        unsigned int get_height();
        

        const hitbox_points get_hitbox();


        void render(SDL_Renderer* renderer);


        void switch_used_asset();

        // ===== Methods =====


        // ===== DATA =====

        Movement movement;

        square_motion_ctx motion_ctx;

        // ===== DATA =====

    private:

        // ===== Methods =====

        void reset_hitbox();

        void reset_surface();

        // ===== Methods =====

        
        // ===== Data =====

        square_motion_state motion_state;

        movement_settings m_settings;


        int x_render_point;
        int y_render_point;


        // Not rescale the passed instance with change !!!
        // Creates pattern of passed instances
        // For example - we passed 200x200 instance and set
        // this sizes to 300x200 - so we got [instance][half_intance] pattern
        // by reset_surface() and save it inside the class surface_to_use
        // if we use 200x200 with 200x200 - we just use surface from instance by
        // pointer copy 

        unsigned int width;
        unsigned int height;


        bool custom_surface_now;
        bool custom_surface_prev;

        custom_surface_gen_mode custom_mode;


        hitbox_points hitbox;


        std::array<std::array<handle_ctx, 1>, 2> square_instances;
        

        unsigned int current_instance;



        SDL_Surface* surface_to_use;

        // ===== Data =====
};