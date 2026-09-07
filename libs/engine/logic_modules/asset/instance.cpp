// instance.cpp


// =========================================================================================== IMPORT

#include "instance.h"

#include "instance_manager.h"

#include <cmath>

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

void Image_instance::set_width(unsigned int new_width)
{
    if (new_width == 0) return;

    // Set new width    
    this->current_width = new_width;

    // Calculate scaler
    auto* image_asset = static_cast<const Image_asset*>(this->asset_manager->get_asset(this->asset_handle));

    unsigned int basic_width = image_asset->get_width();

    float scaler = new_width / basic_width;

    if (scaler <= 0) return;

    // Set new width    
    this->current_width = new_width;

    this->x_scaler = scaler; 
}


// TODO: REDO

void Image_instance::set_height(unsigned int new_height)
{
    if (new_height == 0) return;


    // Calculate scaler
    auto* image_asset = static_cast<const Image_asset*>(this->asset_manager->get_asset(this->asset_handle));

    unsigned int basic_height = image_asset->get_height();

    float scaler = new_height / basic_height;

    if (scaler <= 0) return;

    // Set new height   
    this->current_height = new_height;

    this->y_scaler = scaler; 
}


void Image_instance::set_scaler(float new_x_scaler, float new_y_scaler)
{
    if (new_x_scaler <= 0) return;
    if (new_y_scaler <= 0) return;

    auto* image_asset = static_cast<const Image_asset*>(this->asset_manager->get_asset(this->asset_handle));
    
    unsigned int basic_width = image_asset->get_width();
    unsigned int basic_height = image_asset->get_height();


    unsigned int new_width = std::round(basic_width * new_x_scaler);
    unsigned int new_height = std::round(basic_height * new_y_scaler);

    if (new_width == 0 || new_height == 0) return;


    this->current_width = new_width;
    this->crop_height = new_height;
    this->x_scaler = new_x_scaler;
    this->y_scaler = new_y_scaler;
}



float Image_instance::get_x_scaler() const
{
    return this->x_scaler;
}


float Image_instance::get_y_scaler() const
{
    return this->y_scaler;
}



// ===== METHODS =====



// =========================================================================================== IMAGE INSTANCE 