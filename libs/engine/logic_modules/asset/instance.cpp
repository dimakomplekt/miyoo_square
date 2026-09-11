// instance.cpp


// =========================================================================================== IMPORT

#include "instance.h"

#include "instance_manager.h"

#include <cmath>

#include <algorithm>

// =========================================================================================== IMPORT


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


    unsigned int cropped_width = abs(x_2 - x_1);
    unsigned int cropped_height = abs(y_2 - y_1);

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
    unsigned int cropped_width = abs(this->crop_map.point_2.x - this->crop_map.point_1.x);


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
    unsigned int cropped_height = abs(this->crop_map.point_2.y - this->crop_map.point_1.y);


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


void Image_instance::set_scaler(float new_x_scaler, float new_y_scaler)
{
    if (new_x_scaler <= 0) return;
    if (new_y_scaler <= 0) return;


    // Check width and height difference 

    // Crop map always in basic scale
    unsigned int basic_width = abs(this->crop_map.point_1.x - this->crop_map.point_2.x);
    unsigned int basic_height = abs(this->crop_map.point_1.y - this->crop_map.point_2.y);

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