// floor_sgo.cpp


// =========================================================================================== IMPORT

#include "floor_sgo.h"

#include "../../../global_data/global_data.h"

// =========================================================================================== IMPORT



// =========================================================================================== CLASS ASSET's


// Predeclare (realized inside app.cpp)

extern Asset_manager* App_asset_manager;
extern Instance_manager* App_instance_manager;


handle_ctx floor_1_1_ah;
handle_ctx floor_1_2_ah;
handle_ctx floor_1_3_ah;
handle_ctx floor_1_4_ah;

handle_ctx floor_2_1_ah;
handle_ctx floor_2_2_ah;
handle_ctx floor_2_3_ah;
handle_ctx floor_2_4_ah;


void floor_assets_init()
{
    if (App_asset_manager == nullptr || App_instance_manager == nullptr)
    {
        std::cout << "[ASSET TEST] managers are not initialized\n";
        return;
    }

    const std::string asset_1_1_path = absolute_by_relative_from_exe("app_content/images/floor_1.1.png");
    const std::string asset_1_2_path = absolute_by_relative_from_exe("app_content/images/floor_1.2.png");
    const std::string asset_1_3_path = absolute_by_relative_from_exe("app_content/images/floor_1.3.png");
    const std::string asset_1_4_path = absolute_by_relative_from_exe("app_content/images/floor_1.4.png");

    const std::string asset_2_1_path = absolute_by_relative_from_exe("app_content/images/floor_2.1.png");
    const std::string asset_2_2_path = absolute_by_relative_from_exe("app_content/images/floor_2.2.png");
    const std::string asset_2_3_path = absolute_by_relative_from_exe("app_content/images/floor_2.3.png");
    const std::string asset_2_4_path = absolute_by_relative_from_exe("app_content/images/floor_2.4.png");


    floor_1_1_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_1_path);
    floor_1_2_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_2_path);
    floor_1_3_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_3_path);
    floor_1_4_ah = App_asset_manager->add_asset(IMAGE_AT, asset_1_4_path);

    floor_2_1_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_1_path);
    floor_2_2_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_2_path);
    floor_2_3_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_3_path);
    floor_2_4_ah = App_asset_manager->add_asset(IMAGE_AT, asset_2_4_path);
}


void floor_assets_clear()
{
    if (App_instance_manager == nullptr || App_asset_manager == nullptr)
    {
        return;
    }


    
    // No check at this stage
    const bool asset_1_1_deleted =
        App_asset_manager->delete_asset_request(floor_1_1_ah);

    const bool asset_1_2_deleted =
        App_asset_manager->delete_asset_request(floor_1_2_ah);

    const bool asset_1_3_deleted =
        App_asset_manager->delete_asset_request(floor_1_3_ah);

    const bool asset_1_4_deleted =
        App_asset_manager->delete_asset_request(floor_1_4_ah);


    const bool asset_2_1_deleted =
        App_asset_manager->delete_asset_request(floor_2_1_ah);

    const bool asset_2_2_deleted =
        App_asset_manager->delete_asset_request(floor_2_2_ah);

    const bool asset_2_3_deleted =
        App_asset_manager->delete_asset_request(floor_2_3_ah);

    const bool asset_2_4_deleted =
        App_asset_manager->delete_asset_request(floor_2_4_ah);

    // Clear 

    floor_1_1_ah = {};
    floor_1_2_ah = {};
    floor_1_3_ah = {};
    floor_1_4_ah = {};

    floor_2_1_ah = {};
    floor_2_2_ah = {};
    floor_2_3_ah = {};
    floor_2_4_ah = {};
}


// =========================================================================================== CLASS ASSET's


// =========================================================================================== FLOOR CLASS


// ===== Lifetime =====

Floor_sgo::Floor_sgo()
{
    // Ordinary constructor - must create instanses

    this->floor_instances = {};

    floor_instances[0][0] = App_instance_manager->add_instance(IMAGE_AT, floor_1_1_ah);
    floor_instances[0][1] = App_instance_manager->add_instance(IMAGE_AT, floor_1_2_ah);
    floor_instances[0][2] = App_instance_manager->add_instance(IMAGE_AT, floor_1_3_ah);
    floor_instances[0][3] = App_instance_manager->add_instance(IMAGE_AT, floor_1_4_ah);


    floor_instances[1][0] = App_instance_manager->add_instance(IMAGE_AT, floor_2_1_ah);
    floor_instances[1][1] = App_instance_manager->add_instance(IMAGE_AT, floor_2_2_ah);
    floor_instances[1][2] = App_instance_manager->add_instance(IMAGE_AT, floor_2_3_ah);
    floor_instances[1][3] = App_instance_manager->add_instance(IMAGE_AT, floor_2_4_ah);

    // Reinit data by base value
    // We know that asset 1 is "equal" to asset 2

    this->width = MAIN_WINDOW_H_SIZE;
    this->height = App_instance_manager->get_image_instance(this->floor_instances[0][0])->get_height();


    this->custom_surface_now = true;
    this->custom_surface_prev = true;

    this->custom_mode = PATTERN_CSGM;

    this->current_instance = 0;
    this->surface_to_use = nullptr;


    this->reset_hitbox();

    this->reset_surface();
}


Floor_sgo::Floor_sgo(const Floor_sgo& other)
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

    this->floor_instances = other.floor_instances;

    App_instance_manager->sub(
        this->floor_instances[0][0]
    );

    App_instance_manager->sub(
        this->floor_instances[0][1]
    );

    App_instance_manager->sub(
        this->floor_instances[0][2]
    );

    App_instance_manager->sub(
        this->floor_instances[0][3]
    );

    App_instance_manager->sub(
        this->floor_instances[1][0]
    );

    App_instance_manager->sub(
        this->floor_instances[1][1]
    );

    App_instance_manager->sub(
        this->floor_instances[1][2]
    );

    App_instance_manager->sub(
        this->floor_instances[1][3]
    );

    // Surface is owned independently by this object

    this->surface_to_use = nullptr;

    this->reset_surface();
}


Floor_sgo::~Floor_sgo()
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
        this->floor_instances[0][0]
    );

    App_instance_manager->unsub(
        this->floor_instances[0][1]
    );

    App_instance_manager->unsub(
        this->floor_instances[0][2]
    );


    App_instance_manager->unsub(
        this->floor_instances[0][3]
    );

    App_instance_manager->unsub(
        this->floor_instances[1][0]
    );

    App_instance_manager->unsub(
        this->floor_instances[1][1]
    );

    App_instance_manager->unsub(
        this->floor_instances[1][2]
    );

    App_instance_manager->unsub(
        this->floor_instances[1][3]
    );


    App_instance_manager->delete_instance_request(this->floor_instances[0][0]);
    App_instance_manager->delete_instance_request(this->floor_instances[0][1]);
    App_instance_manager->delete_instance_request(this->floor_instances[0][2]);
    App_instance_manager->delete_instance_request(this->floor_instances[0][3]);

    App_instance_manager->delete_instance_request(this->floor_instances[1][0]);
    App_instance_manager->delete_instance_request(this->floor_instances[1][1]);
    App_instance_manager->delete_instance_request(this->floor_instances[1][2]);
    App_instance_manager->delete_instance_request(this->floor_instances[1][3]);

    // Nullptr everything



    // After that delet itself


}

// ===== Lifetime =====


// ===== Methods =====

void Floor_sgo::set_render_point(int x, int y)
{
    this->x_render_point = x;
    this->y_render_point = y;
    this->reset_hitbox();
}


void Floor_sgo::set_size(unsigned int width)
{

    unsigned int basic_width = 
        App_instance_manager->get_image_instance(this->floor_instances[this->current_instance][0])->get_width();


    if (width % basic_width != 0) return;


    this->width = width;


    this->reset_hitbox();
    this->reset_surface();
}



unsigned int Floor_sgo::get_width()
{
    return this->width;
}


unsigned int Floor_sgo::get_height()
{
    return this->height;
}



const hitbox_points Floor_sgo::get_hitbox()
{
    return this->hitbox;
}


void Floor_sgo::render(SDL_Renderer* renderer)
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


void Floor_sgo::switch_used_asset()
{
    if (this->current_instance == 0) this->current_instance = 1;
    else this->current_instance = 0;

    reset_surface();
}


void Floor_sgo::reset_hitbox()
{
    this->hitbox.top_left.x = this->x_render_point - 0.5 * this->width;
    this->hitbox.top_left.y = this->y_render_point - 0.5 * this->height;      // SDL
    
    this->hitbox.bottom_right.x = this->x_render_point + 0.5 * this->width;
    this->hitbox.bottom_right.y = this->y_render_point + 0.5 * this->height;  // SDL
}


void Floor_sgo::reset_surface()
{

    unsigned int basic_cell_width = 
        App_instance_manager->get_image_instance(this->floor_instances[this->current_instance][0])->get_width();

    unsigned int cells_number = this->width / basic_cell_width;


    SDL_Surface* base_surface_1 = App_instance_manager->get_image_instance(
        this->floor_instances[this->current_instance][0])->get_surface();
    
    SDL_Surface* base_surface_2 = App_instance_manager->get_image_instance(
        this->floor_instances[this->current_instance][1])->get_surface();
    
    SDL_Surface* base_surface_3 = App_instance_manager->get_image_instance(
        this->floor_instances[this->current_instance][2])->get_surface();
    
    SDL_Surface* base_surface_4 = App_instance_manager->get_image_instance(
        this->floor_instances[this->current_instance][3])->get_surface();

    
    if (this->custom_surface_now && this->custom_surface_prev && this->surface_to_use != nullptr)
    {
        SDL_FreeSurface(this->surface_to_use);
    }

    this->surface_to_use = nullptr;


    SDL_Surface* new_surface;

    new_surface = SDL_CreateRGBSurface(
        0,                             
        this->width,                   
        this->height,                  
        32,                            
        0x00ff0000,                    
        0x0000ff00,                    
        0x000000ff,                    
        0xff000000                     
    );


    if (!new_surface) return;

    SDL_Rect dest_rect;



    // A one-cell floor uses the dedicated compact tile.
    if (cells_number == 1)
    {
        SDL_BlitSurface(base_surface_4, nullptr, new_surface, nullptr);
    }
    else if (cells_number >= 2)
    {
        // Requested layout: [asset 2] [asset 1] ... [asset 1] [asset 3].
        dest_rect = { 0, 0, 0, 0 };
        SDL_BlitSurface(base_surface_2, nullptr, new_surface, &dest_rect);

        for (int i = 1; i < cells_number - 1; ++i)
        {
            dest_rect = { static_cast<Sint16>(i * basic_cell_width), 0, 0, 0 };
            SDL_BlitSurface(base_surface_1, nullptr, new_surface, &dest_rect);
        }

        dest_rect = { static_cast<Sint16>((cells_number - 1) * basic_cell_width), 0, 0, 0 };
        SDL_BlitSurface(base_surface_3, nullptr, new_surface, &dest_rect);
    }


    this->surface_to_use = new_surface;

    this->custom_surface_now = true;
}


// ===== Methods =====


// =========================================================================================== FLOOR CLASS