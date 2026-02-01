// asset_instance.cpp


// =========================================================================================== IMPORT

#include "asset_instance.h"

#include <cassert> // assert include

// =========================================================================================== IMPORT



// =========================================================================================== ASSET INSTANCE CLASS


// Basic constructor which initializes the main_asset link (called by main asset methods)

Asset_instance::Asset_instance(Asset* asset) : main_asset(asset)
{
    // Error handler
    assert(asset != nullptr);
};


// Basic destructor, which calls automatically from Asset::delete_instance(instance);

Asset_instance::~Asset_instance() = default;


const Asset* Asset_instance::get_main_asset_link() const
{
    return main_asset;
}


// =========================================================================================== ASSET INSTANCE CLASS


// =========================================================================================== IMAGE INSTANCE CLASS


// Basic constructor which initializes the main_asset link (called by main asset methods)

explicit Image_instance::Image_instance(Image_asset* asset) : Asset_instance(asset)
{
    // Error handler
    assert(asset != nullptr);
};


/* Basic destructor, which calls automatically from:
 * 
 *      Image_asset::delete_instance(instance);
 * 
 * or
 * 
 *      Asset::~Asset();
 */

Image_instance::~Image_instance() {};


// Basic asset link getter override

const Image_asset* Image_instance::get_main_asset_link() const
{
    // Type handling
    return static_cast<const Image_asset*>(Asset_instance::get_main_asset_link());
}


// Scaler

void Image_instance::set_scaler(float x_scaler, float y_scaler)
{
    // Data error handling On the development stage
    assert(x_scaler > 0.0f && y_scaler > 0.0f);


    // Set axes scalers

    this->x_scaler = x_scaler;
    this->y_scaler = y_scaler;


    // Set the new sizes (current_width and current_height)
    this->set_new_size();

    // Set the new anchors map
    this->set_new_anchor_points();

    // Set the new crop map
    this->set_new_crop_map();
}


// Size changers

void Image_instance::set_width(unsigned int new_width)
{
    // Data error handling On the development stage
    assert(new_width > 0);

    // X-scaler recalculation and setting by the old current_width value
    this->x_scaler = static_cast<float>(new_width) / static_cast<float>(this->current_width);

    // Current width reset by the new_width value
    this->current_width = new_width;


    // No reason to call this->set_new_size();

    // Set the new anchors map
    this->set_new_anchor_points();

    // Set the new crop map
    this->set_new_crop_map();
}


void Image_instance::set_height(unsigned int new_height)
{
    // Data error handling On the development stage
    assert(new_height > 0);

    // X-scaler recalculation and setting by the old current_width value
    this->y_scaler = static_cast<float>(new_height) / static_cast<float>(this->current_height);

    // Current width reset by the new_width value
    this->current_height = new_height;


    // No reason to call this->set_new_size();

    // Set the new anchors map
    this->set_new_anchor_points();

    // Set the new crop map
    this->set_new_crop_map();
}



// =========================================================================================== IMAGE INSTANCE CLASS



// =========================================================================================== AUDIO INSTANCE CLASS


Audio_instance::Audio_instance(Audio_asset* asset) : Asset_instance(asset)
{
    // Error handler
    assert(asset != nullptr);
};


Audio_instance::~Audio_instance() {}


const Audio_asset* Audio_instance::get_main_asset_link() const
{
    return static_cast<const Audio_asset*>(Asset_instance::get_main_asset_link());
}


// =========================================================================================== AUDIO INSTANCE CLASS
