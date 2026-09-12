// background_sgo.cpp


// =========================================================================================== IMPORT

#include "background_sgo.h"

// =========================================================================================== IMPORT



// =========================================================================================== CLASS ASSET's


// Predeclare (realized inside app.cpp)

extern Asset_manager* App_asset_manager;
extern Instance_manager* App_instance_manager;


handle_ctx background_1_ah;
handle_ctx background_2_ah;


void background_assets_init()
{
    if (App_asset_manager == nullptr || App_instance_manager == nullptr)
    {
        std::cout << "[ASSET TEST] managers are not initialized\n";
        return;
    }

    const std::string asset_1_path = absolute_by_relative_from_exe("app_content/images/background_1.png");
    const std::string asset_2_path = absolute_by_relative_from_exe("app_content/images/background_2.png");


    background_1_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_path);
    background_2_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_path);
}


void background_assets_clear()
{
    if (App_instance_manager == nullptr || App_asset_manager == nullptr)
    {
        return;
    }

    const bool asset_1_deleted =
        App_asset_manager->delete_asset_request(background_1_ah);


    const bool asset_2_deleted =
        App_asset_manager->delete_asset_request(background_2_ah);

    
    // No check at this stage


    // Clear 

    background_1_ah = {};
    background_2_ah = {};
}


// =========================================================================================== CLASS ASSET's


// =========================================================================================== BACKGROUND CLASS


// ===== Lifetime =====

Background_sgo::Background_sgo()
{
    // Ordinary constructor - must create instanses

    background_instances.push_back(std::array<handle_ctx, 2>()); 

    background_instances[0][0] = App_instance_manager->add_instance(IMAGE_AT, background_1_ah);
    background_instances[0][1] = App_instance_manager->add_instance(IMAGE_AT, background_1_ah);


    // Reinit data by base value
    // We know that asset 1 is "equal" to asset 2

    this->width = 
        App_instance_manager->get_image_instance(this->background_instances[0][0])->get_width();

    this->height = 
        App_instance_manager->get_image_instance(this->background_instances[0][0])->get_width();


    this->custom_surface_now = false;
    this->custom_surface_prev = false;

    this->custom_mode = RESCALE_CSGM;

    this->current_instance = 0;

    this->reset_hitbox();

    this->reset_surface();
}


Background_sgo::Background_sgo(const Background_sgo& other)
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

    this->background_instances = other.background_instances;

    App_instance_manager->sub(
        this->background_instances[0][0]
    );

    App_instance_manager->sub(
        this->background_instances[0][1]
    );


    // Surface is owned independently by this object

    this->surface_to_use = nullptr;

    this->reset_surface();
}


Background_sgo::~Background_sgo()
{
    // With unsub operation we request to delete instance
    // it will be accept if this object was the last 
    // subscriber of instance and declined otherwise
    
    if (!this->custom_surface_now) this->surface_to_use = nullptr;


    App_instance_manager->unsub(
        this->background_instances[0][0]
    );

    App_instance_manager->unsub(
        this->background_instances[0][1]
    );

    App_instance_manager->delete_instance_request(this->background_instances[0][0]);
    App_instance_manager->delete_instance_request(this->background_instances[0][1]);


    // Nullptr everything



    // After that delet itself


}

// ===== Lifetime =====


// ===== Methods =====

void Background_sgo::set_render_point(int x, int y)
{
    this->x_render_point = x;
    this->y_render_point = y;
}


void Background_sgo::set_size(unsigned int width, unsigned int height)
{

    unsigned int basic_width = 
        App_instance_manager->get_image_instance(this->background_instances[0][0])->get_width();

    unsigned int basic_height = 
        App_instance_manager->get_image_instance(this->background_instances[0][0])->get_width();

    if (basic_width != width || basic_height != height)
    {
        // Custom surface need to be generated

        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = true;
    }
    else
    {
        // Custom surface don't need to be generated

        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = false;
    }


    this->reset_hitbox();
    this->reset_surface();
}


const hitbox_points Background_sgo::get_hitbox()
{
    return this->hitbox;
}


void Background_sgo::render(SDL_Renderer* renderer)
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

            this->x_render_point - 0.5 * this->width, // SDL LOGIC - to center-center render
            this->y_render_point - 0.5 * this->width, // SDL LOGIC - to center-center render
            static_cast<int>(this->width), 
            static_cast<int>(this->height) 

        };

        // 4. Render
        SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);

        // 5. Destroy texture after render
        SDL_DestroyTexture(texture);
    }
}


void Background_sgo::switch_used_asset()
{
    if (this->current_instance == 0) this->current_instance = 1;
    else this->current_instance = 0;

    // Don't change sizes if 2nd instance was not the same as it
    // represent's now by class, just check

    unsigned int basic_width = 
        App_instance_manager->get_image_instance(this->background_instances[0][this->current_instance])->get_width();

    unsigned int basic_height = 
        App_instance_manager->get_image_instance(this->background_instances[0][this->current_instance])->get_width();


    if (basic_width != this->width || basic_height != this->height)
    {
        // Custom surface need to be generated

        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = true;
    }
    else
    {
        // Custom surface don't need to be generated

        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = false;
    }


    reset_surface();
}


void Background_sgo::reset_hitbox()
{
    this->hitbox.top_left.x = this->x_render_point - 0.5 * this->width;
    this->hitbox.top_left.y = this->y_render_point - 0.5 * this->width;      // SDL
    
    this->hitbox.bottom_right.x = this->x_render_point + 0.5 * this->width;
    this->hitbox.bottom_right.y = this->y_render_point + 0.5 * this->width;  // SDL
}


void Background_sgo::reset_surface()
{
    // Get the clean initial asset from the manager for generation
    SDL_Surface* base_surface = App_instance_manager->get_image_instance(
        this->background_instances[0][this->current_instance])->get_surface();

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


// =========================================================================================== BACKGROUND CLASS