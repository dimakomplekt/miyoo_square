// instance.cpp


// =========================================================================================== IMPORT

#include "instance.h"

#include "instance_manager.h"

#include <cmath>

#include <algorithm>

// =========================================================================================== IMPORT


// =========================================================================================== HELPERS

// ===== HITBOX and COLLISION =====


collision_result check_collision(

    const hitbox_points& penetrating,
    const hitbox_points& penetrated

)
{

    collision_result result{};


    const int penetrating_left      = penetrating.top_left.x;
    const int penetrating_top       = penetrating.top_left.y;
    const int penetrating_right     = penetrating.bottom_right.x;
    const int penetrating_bottom    = penetrating.bottom_right.y;

    const int penetrated_left       = penetrated.top_left.x;
    const int penetrated_top        = penetrated.top_left.y;
    const int penetrated_right      = penetrated.bottom_right.x;
    const int penetrated_bottom     = penetrated.bottom_right.y;


    // =========================================================================
    // CHECK COLLISION
    // =========================================================================

    const int overlap_x =
        std::min(penetrating_right, penetrated_right)
        - std::max(penetrating_left, penetrated_left);

    const int overlap_y =
        std::min(penetrating_bottom, penetrated_bottom)
        - std::max(penetrating_top, penetrated_top);


    if (overlap_x <= 0 || overlap_y <= 0)
    {
        return {};
    }


    // =========================================================================
    // COLLISION DETECTED
    // =========================================================================

    result.has_collision = true;
    const int penetrating_center_x = penetrating_left + penetrating_right;
    const int penetrated_center_x = penetrated_left + penetrated_right;
    const int penetrating_center_y = penetrating_top + penetrating_bottom;
    const int penetrated_center_y = penetrated_top + penetrated_bottom;

    // Positive means that the penetrating body is on the negative side
    // of the obstacle and must move negatively to separate.
    result.penetration_x =
        penetrating_center_x < penetrated_center_x ? overlap_x : -overlap_x;
    result.penetration_y =
        penetrating_center_y < penetrated_center_y ? overlap_y : -overlap_y;


    return result;

}

// ===== HITBOX and COLLISION =====




// ===== CUSTOM SURFACE GENERATION =====

void custom_surface_generation(SDL_Surface* basic_surface, SDL_Surface* target_surface, custom_surface_gen_mode mode)
{
    // Pointers validation
    if (basic_surface == nullptr || target_surface == nullptr) {
        return;
    }

    // Clear basic surface
    SDL_FillRect(target_surface, nullptr, 0x00000000);

    // Generate custom surface
    if (mode == RESCALE_CSGM) 
    {
        // Rescale
        SDL_BlitScaled(basic_surface, nullptr, target_surface, nullptr);
    } 
    else if (mode == PATTERN_CSGM) 
    {
        int src_w = basic_surface->w;  // 200
        int src_h = basic_surface->h;  // 200
        int dst_w = target_surface->w; // 300
        int dst_h = target_surface->h; // 200

        // Цикл по вертикали. Идем с шагом в высоту плитки (200).
        // Выполнится всего 1 раз (для current_y = 0), так как при следующем шаге 
        // current_y станет 200, а это не меньше dst_h (200).
        for (int current_y = 0; current_y < dst_h; current_y += src_h) 
        {
            // Цикл по горизонтали. Идем с шагом в ширину плитки (200).
            // Сначала current_x = 0. На следующем шаге current_x = 200.
            for (int current_x = 0; current_x < dst_w; current_x += src_w) 
            {
                /*
                ИТЕРАЦИЯ 1 (current_x = 0):
                dst_w - current_x  =>  300 - 0 = 300 (столько места осталось до правого края)
                std::min(200, 300)  =>  выбирает 200. Значит fill_w = 200.
                
                ИТЕРАЦИЯ 2 (current_x = 200):
                dst_w - current_x  =>  300 - 200 = 100 (остался хвостик в 100 пикселей до края!)
                std::min(200, 100)  =>  выбирает 100. Значит fill_w = 100! Вот он, наш КРОП!
                */
                int fill_w = std::min(src_w, dst_w - current_x);
                int fill_h = std::min(src_h, dst_h - current_y);

                /*
                src_rect указывает, КАКОЙ кусок мы вырезаем ИЗ оригинальной плитки.
                ИТЕРАЦИЯ 1: {0, 0, 200, 200} -> берем всю плитку целиком.
                ИТЕРАЦИЯ 2: {0, 0, 100, 200} -> берем только левую половинку плитки!
                */
                SDL_Rect src_rect = { 0, 0, fill_w, fill_h };

                /*
                dst_rect указывает, КУДА на финальном бэкграунде мы этот кусок вставляем.
                ИТЕРАЦИЯ 1: {0, 0, 200, 200} -> вставляем в самое начало.
                ИТЕРАЦИЯ 2: {200, 0, 100, 200} -> вставляем со смещением 200, 
                            как раз туда, где закончилась первая плитка.
                */
                SDL_Rect dst_rect = { current_x, current_y, fill_w, fill_h };

                // SDL копирует строго указанный в src_rect кусочек в указанное место dst_rect.
                // Всё, что не влезло (правая половина второй плитки), просто игнорируется.
                SDL_BlitSurface(basic_surface, &src_rect, target_surface, &dst_rect);
            }
        }
    }
}

// ===== CUSTOM SURFACE GENERATION =====

// =========================================================================================== HELPERS




// =========================================================================================== INSTANCE BASIC CLASS

// ===== LIFETIME =====

Instance::Instance(handle_ctx asset_handle, Instance_manager* r_instance_manager, Asset_manager* r_asset_manager)
: 
asset_handle(asset_handle), instance_manager(r_instance_manager), asset_manager(r_asset_manager) 
{

};


Instance::~Instance() {};

// ===== LIFETIME =====

// =========================================================================================== INSTANCE BASIC CLASS


// =========================================================================================== IMAGE INSTANCE 

// ===== LIFETIME =====

Image_instance::Image_instance(handle_ctx asset_handle, Instance_manager* r_instance_manager, Asset_manager* r_asset_manager) 
: 
Instance(asset_handle, r_instance_manager, r_asset_manager) 
{

    auto* image_asset = this->get_main_asset();


    unsigned int basic_width = image_asset->get_width();
    unsigned int basic_height = image_asset->get_height();
    

    // Basic settings

    this->x_scaler = 1.0f;
    this->y_scaler = 1.0f;


    // Basic crop, sizes and surface prepare

    this->set_new_crop_map(
    
        0, 0,
        basic_width, basic_height
    
    );

};


Image_instance::~Image_instance() 
{
    instance_manager = nullptr;
    asset_manager = nullptr;

    if (this->surface != nullptr)
    {
        SDL_FreeSurface(this->surface);
        this->surface = nullptr;
    }
};

// ===== LIFETIME =====


// ===== METHODS =====


void Image_instance::set_new_crop_map(unsigned int x_1, unsigned int y_1, unsigned int x_2, unsigned int y_2)
{
    if (x_1 == x_2 && y_1 == y_2) return;


    // Calculate scaler
    auto* image_asset = this->get_main_asset();


    unsigned int basic_width = image_asset->get_width();
    unsigned int basic_height = image_asset->get_height();


    if (x_1 > basic_width || x_2 > basic_width) return;
    if (y_1 > basic_height || y_2 > basic_height) return;


    unsigned int cropped_width = abs(static_cast<int>(x_2) - static_cast<int>(x_1));
    unsigned int cropped_height = abs(static_cast<int>(y_2) - static_cast<int>(y_1));

    if (cropped_width == 0) return;
    if (cropped_height == 0) return;


    if (cropped_width > basic_width || cropped_height > basic_height) return;


    // Set crop map by basic scaler of the asset

    this->crop_map.point_1.x = x_1;
    this->crop_map.point_1.y = y_1;
    this->crop_map.point_2.x = x_2;
    this->crop_map.point_2.y = y_2;


    this->crop_map.crop_center.x = (x_1 + x_2) * 0.5;
    this->crop_map.crop_center.y = (y_1 + y_2) * 0.5;


    // We work by the previous scaler here, 
    // because orientation on the both
    // scaled and cropped image seems more
    // heavy and dangerous

    // Check current scalers difference

    float curr_x_scaler = this->x_scaler;
    float curr_y_scaler = this->y_scaler;


    // Set width and height

    this->current_width = cropped_width * curr_x_scaler;
    this->current_height = cropped_height * curr_y_scaler;


    // Set anchors
    this->reset_anchor_points();


    // Renew surface
    this->renew_surface();

}


void Image_instance::set_width(unsigned int new_width)
{
    if (new_width == 0) return;


    // Calculate scaler

    // Crop map always in basic scale
    unsigned int cropped_width = abs(static_cast<int>(this->crop_map.point_2.x) - static_cast<int>(this->crop_map.point_1.x));


    // We work by the previous scaler here, 
    // because orientation on the both
    // scaled and cropped image seems more
    // heavy and dangerous

    float scaler = static_cast<float>(new_width) / cropped_width;

    if (scaler <= 0) return;


    // Set new width and scaler, than recalculate other metadata

    this->current_width = new_width;

    this->x_scaler = scaler; 


    this->reset_anchor_points();

    this->renew_surface();
}


void Image_instance::set_height(unsigned int new_height)
{
    if (new_height == 0) return;


    // Calculate scaler

    // Crop map always in basic scale
    unsigned int cropped_height = abs(static_cast<int>(this->crop_map.point_2.y) - static_cast<int>(this->crop_map.point_1.y));


    // We work by the previous scaler here, 
    // because orientation on the both
    // scaled and cropped image seems more
    // heavy and dangerous

    float scaler = static_cast<float>(new_height) / cropped_height;

    if (scaler <= 0) return;


    // Set new width and scaler, than recalculate other metadata

    this->current_height = new_height;

    this->y_scaler = scaler; 


    this->reset_anchor_points();

    this->renew_surface();
}





unsigned int Image_instance::get_width() const
{
    return this->current_width;
}


unsigned int Image_instance::get_height() const
{
    return this->current_height;
}


void Image_instance::set_scaler(float new_x_scaler, float new_y_scaler)
{
    if (new_x_scaler <= 0) return;
    if (new_y_scaler <= 0) return;


    // Check width and height difference 

    // Crop map always in basic scale
    unsigned int basic_width = abs(static_cast<int>(this->crop_map.point_1.x) - static_cast<int>(this->crop_map.point_2.x));
    unsigned int basic_height = abs(static_cast<int>(this->crop_map.point_1.y) - static_cast<int>(this->crop_map.point_2.y));

    unsigned int new_width = std::round(basic_width * new_x_scaler);
    unsigned int new_height = std::round(basic_height * new_y_scaler);


    if (new_width == 0 || new_height == 0) return;


    this->current_width = new_width;
    this->current_height = new_height;

    this->x_scaler = new_x_scaler;
    this->y_scaler = new_y_scaler;


    this->reset_anchor_points();

    this->renew_surface();
}



float Image_instance::get_x_scaler() const
{
    return this->x_scaler;
}


float Image_instance::get_y_scaler() const
{
    return this->y_scaler;
}




const Image_asset* Image_instance::get_main_asset() const
{
    if (this->asset_manager == nullptr)
    {
        return nullptr;
    }

    const Asset* asset = this->asset_manager->get_asset(this->asset_handle);
    if (asset == nullptr || asset->get_type() != IMAGE_AT)
    {
        return nullptr;
    }

    auto* image_asset = static_cast<const Image_asset*>(asset);

    return image_asset;
}



SDL_Surface* Image_instance::get_surface() const
{   
    return this->surface;
}



void Image_instance::reset_anchor_points()
{

    /**
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
     */


    this->anchors.center_center.x = 0;
    this->anchors.center_center.y = 0;

    this->anchors.center_left.x = -this->current_width * 0.5;  
    this->anchors.center_left.y = 0;  

    this->anchors.center_right.x = this->current_width * 0.5;
    this->anchors.center_right.y = 0;  

    this->anchors.top_center.x = 0;
    this->anchors.top_center.y = -this->current_height * 0.5;            // SDL orientation

    this->anchors.bottom_center.x = 0;
    this->anchors.bottom_center.y = this->current_height * 0.5;          // SDL orientation

    this->anchors.top_left.x = this->anchors.center_left.x;
    this->anchors.top_left.y = this->anchors.top_center.y;

    this->anchors.top_right.x = this->anchors.center_right.x;
    this->anchors.top_right.y = this->anchors.top_center.y;

    this->anchors.bottom_left.x = this->anchors.center_left.x;
    this->anchors.bottom_left.y = this->anchors.bottom_center.y;

    this->anchors.bottom_right.x = this->anchors.center_right.x;
    this->anchors.bottom_right.y = this->anchors.bottom_center.y;

}


void Image_instance::renew_surface()
{
    // Basic asset

    auto* image_asset = this->get_main_asset();

    if (image_asset == nullptr) return;


    const SDL_Surface* source_surface = image_asset->provide_surface();

    if (source_surface == nullptr)
    {
        return;
    }


    // ===== Get crop data =====

    const unsigned int crop_x_1 = this->crop_map.point_1.x;
    const unsigned int crop_y_1 = this->crop_map.point_1.y;

    const unsigned int crop_x_2 = this->crop_map.point_2.x;
    const unsigned int crop_y_2 = this->crop_map.point_2.y;


    // Crop points may be specified in either direction.
    // Normalize them before creating SDL_Rect.

    const unsigned int crop_left   = std::min(crop_x_1, crop_x_2);
    const unsigned int crop_top    = std::min(crop_y_1, crop_y_2);
    const unsigned int crop_right  = std::max(crop_x_1, crop_x_2);
    const unsigned int crop_bottom = std::max(crop_y_1, crop_y_2);


    const unsigned int crop_width  = crop_right - crop_left;
    const unsigned int crop_height = crop_bottom - crop_top;


    if (crop_width == 0 || crop_height == 0)
    {
        return;
    }


    // ===== Get current scaled dimensions =====

    const unsigned int scaled_width  = this->current_width;
    const unsigned int scaled_height = this->current_height;


    if (scaled_width == 0 || scaled_height == 0)
    {
        return;
    }


    // ===== Validate crop against source surface =====

    if (crop_right > static_cast<unsigned int>(source_surface->w))
    {
        return;
    }

    if (crop_bottom > static_cast<unsigned int>(source_surface->h))
    {
        return;
    }


    // ===== Prepare source crop rectangle =====

    SDL_Rect source_rect;

    source_rect.x = static_cast<int>(crop_left);
    source_rect.y = static_cast<int>(crop_top);
    source_rect.w = static_cast<int>(crop_width);
    source_rect.h = static_cast<int>(crop_height);


    // ===== Prepare destination surface =====

    SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        static_cast<int>(scaled_width),
        static_cast<int>(scaled_height),
        source_surface->format->BitsPerPixel,
        source_surface->format->format
    );


    if (new_surface == nullptr)
    {
        return;
    }


    // ===== Crop + scale =====

    SDL_Rect destination_rect;

    destination_rect.x = 0;
    destination_rect.y = 0;
    destination_rect.w = static_cast<int>(scaled_width);
    destination_rect.h = static_cast<int>(scaled_height);


    if (SDL_BlitScaled(
        const_cast<SDL_Surface*>(source_surface),
        &source_rect,
        new_surface,
        &destination_rect
    ) != 0)
    {
        SDL_FreeSurface(new_surface);
        return;
    }


    // ===== Replace old surface =====

    if (this->surface != nullptr)
    {
        SDL_FreeSurface(this->surface);
    }


    this->surface = new_surface;
}


// ===== METHODS =====


// =========================================================================================== IMAGE INSTANCE 