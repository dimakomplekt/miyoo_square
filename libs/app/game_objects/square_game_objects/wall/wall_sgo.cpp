// wall_sgo.cpp


// =========================================================================================== IMPORT

#include "wall_sgo.h"

// =========================================================================================== IMPORT






// =========================================================================================== CLASS ASSET's


// Predeclare (realized inside app.cpp)

extern Asset_manager* App_asset_manager;
extern Instance_manager* App_instance_manager;


handle_ctx wall_1_ah;

handle_ctx wall_2_ah;



void wall_assets_init()
{
    if (App_asset_manager == nullptr || App_instance_manager == nullptr)
    {
        std::cout << "[ASSET TEST] managers are not initialized\n";
        return;
    }

    const std::string asset_1_path = absolute_by_relative_from_exe("app_content/images/wall_1.png");

    const std::string asset_2_path = absolute_by_relative_from_exe("app_content/images/wall_2.png");



    wall_1_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_path);

    wall_2_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_path);

}


void wall_assets_clear()
{
    if (App_instance_manager == nullptr || App_asset_manager == nullptr)
    {
        return;
    }


    
    // No check at this stage
    const bool asset_1_deleted =
        App_asset_manager->delete_asset_request(wall_1_ah);

    const bool asset_2_deleted =
        App_asset_manager->delete_asset_request(wall_2_ah);



    // Clear 

    wall_1_ah = {};
    wall_2_ah = {};
}


// =========================================================================================== CLASS ASSET's


// =========================================================================================== WALL CLASS


// ===== Lifetime =====

Wall_sgo::Wall_sgo()
{
    // Ordinary constructor - must create instanses

    this->wall_instances = {};

    wall_instances[0][0] = App_instance_manager->add_instance(IMAGE_AT, wall_1_ah);

    wall_instances[1][0] = App_instance_manager->add_instance(IMAGE_AT, wall_2_ah);


    // Reinit data by base value
    // We know that asset 1 is "equal" to asset 2

    this->width = App_instance_manager->get_image_instance(this->wall_instances[0][0])->get_width();
    this->height = MAIN_WINDOW_V_SIZE - App_instance_manager->get_image_instance(this->wall_instances[0][0])->get_height();


    this->custom_surface_now = true;
    this->custom_surface_prev = true;

    this->custom_mode = PATTERN_CSGM;

    this->current_instance = 0;
    this->surface_to_use = nullptr;


    this->reset_hitbox();

    this->reset_surface();
}


Wall_sgo::Wall_sgo(const Wall_sgo& other)
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

    this->wall_instances = other.wall_instances;


    App_instance_manager->sub(
        this->wall_instances[0][0]
    );

    App_instance_manager->sub(
        this->wall_instances[1][0]
    );


    // Surface is owned independently by this object

    this->surface_to_use = nullptr;

    this->reset_surface();
}


Wall_sgo::~Wall_sgo()
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
        this->wall_instances[0][0]
    );

    App_instance_manager->unsub(
        this->wall_instances[1][0]
    );



    App_instance_manager->delete_instance_request(this->wall_instances[0][0]);

    App_instance_manager->delete_instance_request(this->wall_instances[1][0]);


    // Nullptr everything



    // After that delet itself


}

// ===== Lifetime =====


// ===== Methods =====

void Wall_sgo::set_render_point(int x, int y)
{
    this->x_render_point = x;
    this->y_render_point = y;
}


void Wall_sgo::set_size(unsigned int height)
{

    unsigned int basic_height = 
        App_instance_manager->get_image_instance(this->wall_instances[this->current_instance][0])->get_height();

    if (height % basic_height != 0) return;


    this->height = height;


    this->reset_hitbox();
    this->reset_surface();
}



unsigned int Wall_sgo::get_width()
{
    return this->width;
}


unsigned int Wall_sgo::get_height()
{
    return this->height;
}



const hitbox_points Wall_sgo::get_hitbox()
{
    return this->hitbox;
}


void Wall_sgo::render(SDL_Renderer* renderer)
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


void Wall_sgo::switch_used_asset()
{
    if (this->current_instance == 0) this->current_instance = 1;
    else this->current_instance = 0;

    reset_surface();
}


void Wall_sgo::reset_hitbox()
{
    this->hitbox.top_left.x = this->x_render_point - 0.5 * this->width;
    this->hitbox.top_left.y = this->y_render_point - 0.5 * this->height;      // SDL
    
    this->hitbox.bottom_right.x = this->x_render_point + 0.5 * this->width;
    this->hitbox.bottom_right.y = this->y_render_point + 0.5 * this->height;  // SDL
}


void Wall_sgo::reset_surface()
{

    unsigned int basic_cell_height = 
        App_instance_manager->get_image_instance(this->wall_instances[this->current_instance][0])->get_height();
    

    if (basic_cell_height == 0) return;


    unsigned int cells_number = this->height / basic_cell_height;

    if (cells_number == 0) return;
    if (cells_number == 1)
    {
        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = false;
    }
    else
    {
        this->custom_surface_prev = this->custom_surface_now;
        this->custom_surface_now = true;
    }


    SDL_Surface* base_surface_1 = App_instance_manager->get_image_instance(
        this->wall_instances[this->current_instance][0])->get_surface();

    
        
    if (this->custom_surface_prev && this->surface_to_use != nullptr)
    {
        SDL_FreeSurface(this->surface_to_use);
    }
    if (this->custom_surface_now && this->surface_to_use != nullptr)
    {
        this->surface_to_use = nullptr;
    }

    // One cell case

    if (!this->custom_surface_now)
    {
        this->surface_to_use = base_surface_1;
        return;
    }

    if (this->custom_surface_now)
    {
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

        if (new_surface == nullptr)
        {
            return;
        }

        custom_surface_generation(
            base_surface_1,
            new_surface,
            this->custom_mode
        );

        this->surface_to_use = new_surface;

    }
}


// ===== Methods =====


// =========================================================================================== WALL CLASS