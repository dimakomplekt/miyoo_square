// square_sgo.cpp


// =========================================================================================== IMPORT

#include "square_sgo.h"

#include <algorithm>
#include <cmath>

// =========================================================================================== IMPORT


// =========================================================================================== CLASS ASSET's


// Predeclare (realized inside app.cpp)

extern Asset_manager* App_asset_manager;
extern Instance_manager* App_instance_manager;


handle_ctx square_1_ah;

handle_ctx square_2_ah;


void square_assets_init()
{
    if (App_asset_manager == nullptr || App_instance_manager == nullptr)
    {
        std::cout << "[ASSET TEST] managers are not initialized\n";
        return;
    }

    const std::string asset_1_path = absolute_by_relative_from_exe("app_content/images/square_1.png");

    const std::string asset_2_path = absolute_by_relative_from_exe("app_content/images/square_2.png");



    square_1_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_path);

    square_2_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_path);

}


void square_assets_clear()
{
    if (App_instance_manager == nullptr || App_asset_manager == nullptr)
    {
        return;
    }


    
    // No check at this stage
    const bool asset_1_deleted =
        App_asset_manager->delete_asset_request(square_1_ah);

    const bool asset_2_deleted =
        App_asset_manager->delete_asset_request(square_2_ah);



    // Clear 

    square_1_ah = {};
    square_2_ah = {};
}


// =========================================================================================== CLASS ASSET's


// =========================================================================================== SQUARE CLASS


// ===== Lifetime =====

Square_sgo::Square_sgo()
:
movement(

    this->m_settings,
    App_timer_1,
    [this](float dx, float dy)
    {
        this->set_render_point_by_delta(dx, dy);
    }

)
{
    // Ordinary constructor - must create instanses

    this->square_instances = {};

    square_instances[0][0] = App_instance_manager->add_instance(IMAGE_AT, square_1_ah);

    square_instances[1][0] = App_instance_manager->add_instance(IMAGE_AT, square_2_ah);


    // Reinit data by base value
    // We know that asset 1 is "equal" to asset 2

    this->width = App_instance_manager->get_image_instance(this->square_instances[0][0])->get_width();
    this->height = App_instance_manager->get_image_instance(this->square_instances[0][0])->get_height();


    this->custom_surface_now = false;
    this->custom_surface_prev = false;

    this->custom_mode = RESCALE_CSGM;

    this->current_instance = 0;
    this->surface_to_use = nullptr;

    this->motion_state = MOVING_SMS;

    this->m_settings.max_speed = 200.0f;
    this->m_settings.acceleration = 50.0f;
    this->m_settings.deceleration = 25.0f;
    this->m_settings.jump_max_speed = 400.0f;
    this->m_settings.jump_acceleration = 200.0f;
    this->m_settings.jump_deceleration = 200.0f;


    this->movement.switch_settings(this->m_settings);
    

    this->reset_hitbox();

    this->reset_surface();
}


Square_sgo::Square_sgo(const Square_sgo& other)
:

m_settings(other.m_settings),

movement(
    this->m_settings,
    App_timer_1,
    [this](float dx, float dy)
    {
        this->set_render_point_by_delta(dx, dy);
    }
)

{
    // Copy ordinary data

    this->width = other.width;
    this->height = other.height;

    this->x_render_point = other.x_render_point;
    this->y_render_point = other.y_render_point;

    this->hitbox = other.hitbox;

    this->custom_surface_now = other.custom_surface_now;
    this->custom_surface_prev = other.custom_surface_prev;

    this->custom_mode = other.custom_mode;

    this->current_instance = other.current_instance;


    // Copy instance handles and subscribe as a new owner

    this->square_instances = other.square_instances;

    App_instance_manager->sub(
        this->square_instances[0][0]
    );

    App_instance_manager->sub(
        this->square_instances[1][0]
    );


    // Surface is owned independently by this object

    this->surface_to_use = nullptr;

    this->reset_surface();
}


Square_sgo::~Square_sgo()
{
    // With unsub operation we request to delete instance
    // it will be accept if this object was the last 
    // subscriber of instance and declined otherwise
    
    if (this->custom_surface_now && this->surface_to_use != nullptr)
    {
        SDL_FreeSurface(this->surface_to_use);
    }

    this->surface_to_use = nullptr;


    App_instance_manager->unsub(
        this->square_instances[0][0]
    );

    App_instance_manager->unsub(
        this->square_instances[1][0]
    );


    App_instance_manager->delete_instance_request(this->square_instances[0][0]);

    App_instance_manager->delete_instance_request(this->square_instances[1][0]);


    // Nullptr everything


    // After that delet itself

}

// ===== Lifetime =====


// ===== Methods =====



void Square_sgo::motion_processing(collision_result outer_check)
{
    Image_instance* controlled_instance =
        App_instance_manager->get_image_instance(
            this->square_instances[this->current_instance][0]);
    if (controlled_instance == nullptr)
    {
        return;
    }

    const float delta_time = std::clamp(App_timer_1.get_delta_time(), 0.0f, 0.1f);
    const int requested_direction = this->movement.get_x_request().direction;

    if (this->motion_state == BOUNCING_SMS)
    {
        const float bounce_delta =
            this->motion_ctx.bounce_speed * delta_time +
            this->motion_ctx.bounce_sub_pixel;
        const int bounce_pixels = std::min(
            static_cast<int>(bounce_delta),
            static_cast<int>(this->motion_ctx.bounce_remaining));
        this->motion_ctx.bounce_sub_pixel =
            bounce_delta - static_cast<float>(bounce_pixels);

        if (bounce_pixels > 0)
        {
            this->set_render_point_by_delta(
                this->motion_ctx.compressed_direction > 0
                    ? -bounce_pixels
                    : bounce_pixels,
                0);
            this->motion_ctx.bounce_remaining -= bounce_pixels;
        }

        if (this->motion_ctx.bounce_remaining <= 0.0f)
        {
            this->motion_ctx.bounce_remaining = 0.0f;
            this->motion_state = MOVING_SMS;
            this->motion_ctx.compressed_direction = 0;
        }

        return;
    }

    if (this->motion_state == MOVING_SMS && outer_check.has_collision &&
        requested_direction != 0)
    {
        this->motion_state = COMPRESSING_SMS;
        this->motion_ctx.compressed_direction = requested_direction;
        this->motion_ctx.contact_edge_x =
            requested_direction > 0
                ? this->hitbox.bottom_right.x - outer_check.penetration_x
                : this->hitbox.top_left.x - outer_check.penetration_x;

        // Remove the overlap before changing the width.
        this->set_render_point_by_delta(-outer_check.penetration_x, 0);
    }

    if (this->motion_state == COMPRESSING_SMS)
    {
        if (requested_direction == 0)
        {
            this->motion_state = RECOVERING_SMS;
        }

        if (this->motion_state == COMPRESSING_SMS)
        {
            this->motion_ctx.curr_size_scaler = std::max(
                this->motion_ctx.min_size_scaler,
                this->motion_ctx.curr_size_scaler -
                    this->motion_ctx.compression_speed * delta_time);

            controlled_instance->set_scaler(
                this->motion_ctx.curr_size_scaler, 1.0f);
            this->set_size(
                controlled_instance->get_width(),
                controlled_instance->get_height());

            this->set_render_point(
                this->motion_ctx.compressed_direction > 0
                    ? this->motion_ctx.contact_edge_x -
                          static_cast<int>(this->width / 2)
                    : this->motion_ctx.contact_edge_x +
                          static_cast<int>(this->width / 2),
                this->y_render_point);
        }
    }

    if (this->motion_state == RECOVERING_SMS)
    {
        this->motion_ctx.curr_size_scaler = std::min(
            this->motion_ctx.max_size_scaler,
            this->motion_ctx.curr_size_scaler +
                this->motion_ctx.recovery_speed * delta_time);

        controlled_instance->set_scaler(
            this->motion_ctx.curr_size_scaler, 1.0f);
        this->set_size(
            controlled_instance->get_width(),
            controlled_instance->get_height());
        this->set_render_point(
            this->motion_ctx.compressed_direction > 0
                ? this->motion_ctx.contact_edge_x -
                      static_cast<int>(this->width / 2)
                : this->motion_ctx.contact_edge_x +
                      static_cast<int>(this->width / 2),
            this->y_render_point);

        if (this->motion_ctx.curr_size_scaler >=
            this->motion_ctx.max_size_scaler)
        {
            this->motion_ctx.curr_size_scaler =
                this->motion_ctx.max_size_scaler;
            this->motion_ctx.bounce_remaining =
                this->motion_ctx.bounce_distance;
            this->motion_ctx.bounce_sub_pixel = 0.0f;
            this->motion_state = BOUNCING_SMS;
        }
    }
}


square_motion_state Square_sgo::get_motion_state() const
{
    return this->motion_state;
}



void Square_sgo::set_render_point(int x, int y)
{
    this->x_render_point = x;
    this->y_render_point = y;
    this->reset_hitbox();
}


void Square_sgo::set_render_point_by_delta(int dx, int dy)
{
    this->x_render_point += dx;
    this->y_render_point += dy;
    this->reset_hitbox();
}


int Square_sgo::get_x_render_point() const
{
    return this->x_render_point;
}


int Square_sgo::get_y_render_point() const
{
    return this->y_render_point;
}



void Square_sgo::set_size(unsigned int width, unsigned int height)
{

    unsigned int basic_width =
        App_instance_manager->get_image_instance(
            this->square_instances[this->current_instance][0])->get_width();
    unsigned int basic_height = 
        App_instance_manager->get_image_instance(this->square_instances[this->current_instance][0])->get_height();

    this->width = width;
    this->height = height;

    
    if (width != basic_width || height != basic_height)
    {   
        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = true;
    }
    else
    {
        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = false;
    }

    this->reset_hitbox();
    this->reset_surface();
}



unsigned int Square_sgo::get_width()
{
    return this->width;
}


unsigned int Square_sgo::get_height()
{
    return this->height;
}



const hitbox_points Square_sgo::get_hitbox()
{
    return this->hitbox;
}


void Square_sgo::render(SDL_Renderer* renderer)
{
    // 1. Check
    if (renderer == nullptr || this->surface_to_use == nullptr) {
        return;
    }

    // 2. Create texture (download pixels in GPU)
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, this->surface_to_use);
    
    if (texture != nullptr) 
    {
        // 3. Set coordinates and sizes
        SDL_Rect dst_rect = 
        { 

            this->x_render_point - static_cast<int>(this->width / 2), // SDL LOGIC - to center-center render
            this->y_render_point - static_cast<int>(this->height / 2), // SDL LOGIC - to center-center render

            static_cast<int>(this->width), 
            static_cast<int>(this->height) 

        };

        // 4. Render
        SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);

        // 5. Destroy texture after render
        SDL_DestroyTexture(texture);
    }
}


void Square_sgo::switch_used_asset()
{
    if (this->current_instance == 0) this->current_instance = 1;
    else this->current_instance = 0;

    reset_surface();
}


void Square_sgo::reset_hitbox()
{
    this->hitbox.top_left.x = this->x_render_point - 0.5 * this->width;
    this->hitbox.top_left.y = this->y_render_point - 0.5 * this->height;      // SDL
    
    this->hitbox.bottom_right.x = this->x_render_point + 0.5 * this->width;
    this->hitbox.bottom_right.y = this->y_render_point + 0.5 * this->height;  // SDL
}



void Square_sgo::reset_surface()
{
    // Get the clean initial asset from the manager for generation
    SDL_Surface* base_surface = App_instance_manager->get_image_instance(
        this->square_instances[this->current_instance][0])->get_surface();



    // Clear case
    if (!custom_surface_now && custom_surface_prev)
    {
        // FIX: Replaced delete with SDL_FreeSurface to avoid heap corruption
        if (this->surface_to_use != nullptr) 
        {
            SDL_FreeSurface(this->surface_to_use);
        }

        this->surface_to_use = nullptr;

        // Use basic surface
        this->surface_to_use = base_surface;
    }

    if (!custom_surface_now && !custom_surface_prev)
    {
        // Use basic surface
        this->surface_to_use = base_surface;
    }


    if (custom_surface_now && !custom_surface_prev)
    {
        // Just reset pointer to the new surface 
        // obtained by generation without any clear

        SDL_Surface* new_surface = SDL_CreateRGBSurface(
            0,                             
            this->width,                   
            this->height,                  
            32,                            
            0x00ff0000,                    
            0x0000ff00,                    
            0x000000ff,                    
            0xff000000                     
        );

        // FIX: Pass base_surface instead of tmp to get perfect source pixel data
        custom_surface_generation(base_surface, new_surface, this->custom_mode);

        this->surface_to_use = new_surface;
    }


    if (custom_surface_now && custom_surface_prev)
    {
        // Just reset pointer to the new surface 
        // obtained by generation without any clear
        SDL_Surface* tmp = surface_to_use;

        SDL_Surface* new_surface = SDL_CreateRGBSurface(
            0,                             
            this->width,                   
            this->height,                  
            32,                            
            0x00ff0000,                    
            0x0000ff00,                    
            0x000000ff,                    
            0xff000000                     
        );


        // FIX: Pass base_surface to prevent cumulative compression artifact blurring
        custom_surface_generation(base_surface, new_surface, this->custom_mode);

        // FIX: Replaced delete with SDL_FreeSurface for internal allocation cleanup
        if (tmp != nullptr) 
        {
            SDL_FreeSurface(tmp);
        }

        surface_to_use = new_surface;
    }

    // Sync state for the next size/state change call
    custom_surface_prev = custom_surface_now;
}

// ===== Methods =====

// =========================================================================================== SQUARE CLASS