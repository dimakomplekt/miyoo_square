// background_sgo.h


#pragma once

// =========================================================================================== IMPORT


#include "../../../../engine/engine.h"

#include "../../../global_data/global_data.h"


#include <array>
#include <vector>

// =========================================================================================== IMPORT


/*
    NOTE

        Call background_assets_init() before any background objects creation

        Call background_assets_clear(); after background objects using end

        If you called background_assets_clear(); and need objects once again -
        return to the step 1

*/


// =========================================================================================== CLASS ASSET's

void background_assets_init();
void background_assets_clear();

extern handle_ctx background_1_ah;
extern handle_ctx background_2_ah;

// =========================================================================================== CLASS ASSET's


// =========================================================================================== BACKGROUND CLASS


class Background_sgo
{
    public:

        // ===== Lifetime =====

        Background_sgo();

        Background_sgo(const Background_sgo& other); 

        ~Background_sgo();

        // ===== Lifetime =====


        // ===== Methods =====

        void set_render_point(int x, int y);

        void set_size(unsigned int width, unsigned int height);


        

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


        std::vector<std::array<handle_ctx, 2>> background_instances;

        unsigned int current_instance;



        SDL_Surface* surface_to_use;

        // ===== Data =====
};



// =========================================================================================== BACKGROUND CLASS