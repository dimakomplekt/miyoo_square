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

    // Initialize object data members default values

    // Default sizes (current_width and current_height by the original asset sizes)
    this->current_width = asset->get_width();
    this->current_height = asset->get_height();

    // Default scalers 
    this->x_scaler = 1.0f;
    this->y_scaler = 1.0f;

    // Initial scalers cache
    this->last_x_scaler = 1.0f;
    this->last_y_scaler = 1.0f;

    
    // Default flip flags
    this->horizontal_flip = false;
    this->vertical_flip = false;

    // Default rotation angle
    this->rotation_angle = 0.0f;

    // Default crop map by the current sizes and 0 points
    this->crop_map = crop_map_2D {

        {0.0f, 0.0f},   // Default bottom left  
        {static_cast<float>(this->current_width), static_cast<float>(this->current_height)} // Default top right

    };


    // Set the new anchors map
    this->reset_anchor_points();
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

    // Cache the last scalers for crop map recalculation
    this->last_x_scaler = this->x_scaler;
    this->last_y_scaler = this->y_scaler;

    // Set axes scalers

    this->x_scaler = x_scaler;
    this->y_scaler = y_scaler;


    // Set the new sizes (current_width and current_height)
    this->reset_size();

    // Set the new crop map
    this->reset_crop_map();

    // Set the new anchors map
    this->reset_anchor_points();
}


// Size changers

void Image_instance::set_width(unsigned int new_width)
{
    // Data error handling On the development stage
    assert(new_width > 0);

    // Cache the last scalers for crop map recalculation
    this->last_x_scaler = this->x_scaler;

    // X-scaler recalculation and setting by the old current_width value
    this->x_scaler = static_cast<float>(new_width) / static_cast<float>(this->get_main_asset_link()->get_width());


    // Current width reset by the new_width value
    this->current_width = new_width;


    // No reason to call this->reset_size();

    // Set the new crop map
    this->reset_crop_map();


    // Set the new anchors map
    this->reset_anchor_points();
}


void Image_instance::set_height(unsigned int new_height)
{
    // Data error handling On the development stage
    assert(new_height > 0);

    // Cache the last scalers for crop map recalculation
    this->last_y_scaler = this->y_scaler;

    // Y-scaler recalculation and setting by the old current_width value
    this->y_scaler = static_cast<float>(new_height) / static_cast<float>(this->get_main_asset_link()->get_height());

    
    // Current width reset by the new_width value
    this->current_height = new_height;


    // No reason to call this->reset_size();


    // Set the new crop map 
    this->reset_crop_map();

    // Set the new anchors map
    this->reset_anchor_points();
}


// Crop map setters with data error handling ()


// === Inner recalculation ===

// Sizes recalculation

void Image_instance::reset_size()
{
    // Values change by the scalers with type handling

    this->current_width = static_cast<unsigned int>(this->get_main_asset_link()->get_width() * this->x_scaler);
    this->current_height = static_cast<unsigned int>(this->get_main_asset_link()->get_height() * this->y_scaler);

}


// Crop map recalculation

void Image_instance::reset_crop_map()
{
    /** 
     * 
     * Recalculate by the old crop map coordinates and new / cached scalers delta 
     * 
     * If we have proportion as:
     * 
     * OLD_SCALE -> OLD_COORDINATE
     * NEW_SCALE -> NEW_COORDINATE
     * 
     * we could calculate the NEW_COORDINATE by cross-multiplication formula:
     * 
     * NEW_COORDINATE = (NEW_SCALE / OLD_SCALE) * OLD_COORDINATE
     * 
     * 
     */


    // Delta-coefficient

    float scaler_delta_x = this->x_scaler / this->last_x_scaler;
    float scaler_delta_y = this->y_scaler / this->last_y_scaler;


    // New crop points

    desc_c_2D new_bottom_left = {

        this->crop_map.bottom_left.x * scaler_delta_x,
        this->crop_map.bottom_left.y * scaler_delta_y,

    };

    desc_c_2D new_top_right = {

        this->crop_map.top_right.x * scaler_delta_x,
        this->crop_map.top_right.y * scaler_delta_y,
        
    };


    // Horizontal crop dimension recalculation 
    unsigned int new_crop_width = new_top_right.x - new_bottom_left.x;

    // Vertical crop dimension recalculation
    unsigned int new_crop_height = new_top_right.y - new_bottom_left.y;

    // New crop map
    // NOTE: BE SURE THAT reset_crop_map() ain't called inside set_new_crop_map()
    this->set_new_crop_map(new_bottom_left, new_top_right);
}


unsigned int Image_instance::get_crop_width() const
{
    return this->crop_width;
}

unsigned int Image_instance::get_crop_height() const
{
    return this->crop_height;
}


void Image_instance::reset_anchor_points()
{
    // Uses current crop to set the current anchor points

    // Crop width and height scalers
    float w = static_cast<float>(this->crop_width);
    float h = static_cast<float>(this->crop_height);

    // Anchors reset

    this->anchors.top_left = {0.0f * w, 1.0f * h};
    this->anchors.top_center = {0.5f * w, 1.0f * h};
    this->anchors.top_right = {1.0f * w, 1.0f * h};

    this->anchors.center_left = {0.0f * w, 0.5f * h};
    this->anchors.center_center = {0.5f * w, 0.5f * h};
    this->anchors.center_right = {1.0f * w, 0.5f * h};

    this->anchors.bottom_left = {0.0f * w, 0.0f * h};
    this->anchors.bottom_center = {0.5f * w, 0.0f * h};
    this->anchors.bottom_right = {1.0f * w, 0.0f * h};
}

// === Inner recalculation ===

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
