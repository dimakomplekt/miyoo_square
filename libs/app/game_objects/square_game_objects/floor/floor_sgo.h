// floor_sgo.h


#pragma once

// =========================================================================================== IMPORT


#include "../../../../engine/engine.h"

#include "../../../global_data/global_data.h"


#include <array>
#include <vector>

// =========================================================================================== IMPORT



/*
    NOTE

        Call floor_assets_init() before any floor objects creation

        Call floor_assets_clear(); after floor objects using end

        If you called floor_assets_clear(); and need objects once again -
        return to the step 1

*/


// =========================================================================================== CLASS ASSET's

void floor_assets_init();
void floor_assets_clear();

extern handle_ctx floor_1_1_ah;
extern handle_ctx floor_1_2_ah;
extern handle_ctx floor_1_3_ah;
extern handle_ctx floor_1_4_ah;

extern handle_ctx floor_2_1_ah;
extern handle_ctx floor_2_2_ah;
extern handle_ctx floor_2_3_ah;
extern handle_ctx floor_2_4_ah;

// =========================================================================================== CLASS ASSET's


// =========================================================================================== FLOOR CLASS


class Floor_sgo
{
    public:

        // ===== Lifetime =====

        Floor_sgo();

        Floor_sgo(const Floor_sgo& other); 

        ~Floor_sgo();

        // ===== Lifetime =====


        // ===== Methods =====

        void set_render_point(int x, int y);

        void set_size(unsigned int width);


        unsigned int get_width();

        unsigned int get_height();
        

        const hitbox_points get_hitbox();



        void render(SDL_Renderer* renderer);


        void switch_used_asset();

        // ===== Methods =====


    private:

        // ===== Methods =====

        void reset_hitbox();
        void reset_surface();

        // ===== Methods =====



        // ===== Data =====

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


        std::array<std::array<handle_ctx, 4>, 2> floor_instances;
        

        unsigned int current_instance;



        SDL_Surface* surface_to_use;

        // ===== Data =====
};



// =========================================================================================== FLOOR CLASS