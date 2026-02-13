// asset_instance.cpp


// =========================================================================================== IMPORT

#include "asset_instance.h"

#include <cassert> // assert include
#include <cmath> // Devision and rounding operations

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

    
    // Default mirror flags
    this->horizontal_mirror = false;
    this->vertical_mirror = false;

    // Default rotation angle
    this->rotation_angle = 0.0f;

    // Default crop map by the current sizes and 0 points
    this->crop_map = crop_map_2D {

        {0, 0},   // Default bottom left  
        {this->current_width, this->current_height} // Default top right

    };

    this->crop_width = this->current_width;
    this->crop_height = this->current_height;

    this->scaled_crop_width = this->crop_width;
    this->scaled_crop_height = this->crop_height;

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


// === Size control ===

// Scaler setter

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
    // here could be fluctuations of the values, because of rounding 
    this->reset_size();

    // Set the new anchors map
    this->reset_anchor_points();
}


// Scaler getters

float Image_instance::get_x_scaler() const
{
    return this->x_scaler;
}

float Image_instance::get_y_scaler() const
{
    return this->y_scaler;
}


// Size setters

void Image_instance::set_width(unsigned int new_width)
{

    // Cache the last scalers for crop map recalculation
    this->last_x_scaler = this->x_scaler;

    // X-scaler recalculation and setting by the old current_width value
    // Float cast for right precision
    this->x_scaler = static_cast<float>(new_width) / static_cast<float>(this->get_main_asset_link()->get_width());


    // Current width reset by the new_width value
    this->current_width = new_width;

    // No reason to call this->reset_size();

    // Set the new anchors map
    this->reset_anchor_points();
}


void Image_instance::set_height(unsigned int new_height)
{

    // Cache the last scalers for crop map recalculation
    this->last_y_scaler = this->y_scaler;

    // Y-scaler recalculation and setting by the old current_width value
    this->y_scaler = static_cast<float>(new_height) / static_cast<float>(this->get_main_asset_link()->get_height());

    
    // Current width reset by the new_width value
    this->current_height = new_height;

    // No reason to call this->reset_size();

    // Set the new anchors map
    this->reset_anchor_points();
}


// NOTE: For size geting used methods get_crop_width() and get_crop_heigh() 


// === Size control ===


// === Rotation and mirror ===

// Mirror setters 

void Image_instance::set_horizontal_mirror(bool h_m_enable)
{
    this->horizontal_mirror = h_m_enable;
}


void Image_instance::set_vertical_mirror(bool v_m_enable)
{
    this->vertical_mirror = v_m_enable;
}


void Image_instance::set_mirror(bool h_m_enable, bool v_m_enable)
{
    this->horizontal_mirror = h_m_enable;
    this->vertical_mirror = v_m_enable;
}

// Mirror getters 

bool Image_instance::get_horizontal_mirror() const
{
    return this->horizontal_mirror;
}

bool Image_instance::get_vertical_mirror() const
{
    return this->vertical_mirror;
}

// Angle setters 

void Image_instance::set_angle(float angle_deg)
{
    this->rotation_angle = angle_deg;
}


void Image_instance::add_angle(float delta_angle_deg)
{
    this->rotation_angle += delta_angle_deg;
}


// Angle getter

float Image_instance::get_angle() const
{
    return this->rotation_angle;
}

// === Rotation and mirror ===


// === Crop map ===


// Crop map setters with data error handling ()

void Image_instance::set_new_crop_map(const crop_map_2D& new_crop_map)
{
    unsigned int x_1 = new_crop_map.point_1.x;
    unsigned int y_1 = new_crop_map.point_1.y;
    unsigned int x_2 = new_crop_map.point_2.x;
    unsigned int y_2 = new_crop_map.point_2.y;


    // Basic setter call
    this->set_new_crop_map(x_1, y_1, x_2, y_2);
}



void Image_instance::set_new_crop_map(const desc_c_2D& point_1, const desc_c_2D& point_2)
{
    unsigned int x_1 = point_1.x;
    unsigned int y_1 = point_1.y;
    unsigned int x_2 = point_2.x;
    unsigned int y_2 = point_2.y;


    // Basic setter call
    this->set_new_crop_map(x_1, y_1, x_2, y_2);
}


// Crop map setting is not super often operation, so we could afford some extra logic for 
// programming comfort (with any point pass order handling)
void Image_instance::set_new_crop_map(unsigned int x_1, unsigned int y_1, unsigned int x_2, unsigned int y_2)
{
    // Bottom left and top right points definition

    bool x_diff_flag = (x_1 <= x_2);
    bool y_diff_flag = (y_1 <= y_2);

    // Points initialization
    desc_c_2D point_1;
    desc_c_2D point_2;


    /**
     * 
     *    ---------P2
     *    ----------
     *    P1--------
     * 
     *    All pass order cases transforms to be like this one 
     *    at the end of the if-else block.
     */
    if (x_diff_flag && y_diff_flag)
    {
        point_1 = {x_1, y_1};
        point_2 = {x_2, y_2};
    }

    /**
     * 
     *    P1--------
     *    ----------
     *    ---------P2
     * 
     */
    else if (x_diff_flag && !y_diff_flag)
    {
        point_1 = {x_1, y_2};
        point_2 = {x_2, y_1};
    }

    /**
     * 
     *    P2--------
     *    ----------
     *    ---------P1
     * 
     */
    else if (!x_diff_flag && y_diff_flag)
    {
        point_1 = {x_2, y_1};
        point_2 = {x_1, y_2};
    }

    /**
     * 
     *    ---------P1
     *    ----------
     *    P2--------
     * 
     */
    else
    {
        point_1 = {x_2, y_2};
        point_2 = {x_1, y_1};
    }


    // Data error handling
    // Assert if crop map range out of the initial asset sizes
    assert(

        point_2.x <= this->get_main_asset_link()->get_width() &&
        point_2.y <= this->get_main_asset_link()->get_height() &&

        "Crop map exceeds image bounds"

    );


    // Crop map setting
    this->crop_map.point_1 = point_1;
    this->crop_map.point_2 = point_2;

    
    // Sizes recalculation
    this->crop_width = point_2.x - point_1.x;
    this->crop_height = point_2.y - point_1.y;


    // Reset anchor points
    this->reset_anchor_points();
}


// Size getters
unsigned int Image_instance::get_crop_width() const
{
    return this->crop_width;
}

unsigned int Image_instance::get_crop_height() const
{
    return this->crop_height;
}


// === Crop map ===



// === Inner recalculation ===

// Sizes recalculation

void Image_instance::reset_size()
{
    // Values change by the scalers with rounding and type handling

    this->current_width = static_cast<unsigned int>(

        // int * float with autocast by compiler
        // Always same rounding accuracy result, cause we work with initial width and new scaler
        std::round(this->get_main_asset_link()->get_width() * this->x_scaler)

    );


    this->current_height = static_cast<unsigned int>(

        // Always same rounding accuracy result, cause we work with initial width and new scaler
        std::round(this->get_main_asset_link()->get_height() * this->y_scaler)

    );
}


void Image_instance::reset_anchor_points()
{
    // Uses current crop to set the current anchor points

    // Crop width and height scalers
    // Always the same rounding accuracy, because we work with crop map in initial scale and new scalers
    unsigned int w = static_cast<unsigned int>(std::round(this->crop_width * this->get_x_scaler()));
    unsigned int h = static_cast<unsigned int>(std::round(this->crop_height * this->get_y_scaler()));


    // Anchors reset
    
    /**
     * 
     * P P P P P
     * 1 2 3 4 5
     * 
     * center is 3 -> (n + 1) / 2
     *
     */

    unsigned int c_w = (w + 1) / 2;     // Horizontal center
    unsigned int c_h = (h + 1) / 2;     // Vertical center

    this->anchors.top_left              = { 0,  h };
    this->anchors.top_center            = { c_w, h };
    this->anchors.top_right             = { w,  h };

    this->anchors.center_left           = { 0,  c_h };
    this->anchors.center_center         = { c_w, c_h };
    this->anchors.center_right          = { w,  c_h };

    this->anchors.bottom_left           = { 0,  0 };
    this->anchors.bottom_center         = { c_w, 0 };
    this->anchors.bottom_right          = { w,  0 };

    // Anchors reset with rounding and type handling

    // New sizes of scaled crop
    this->scaled_crop_width = w;
    this->scaled_crop_height = h;
}

// === Inner recalculation ===

// =========================================================================================== IMAGE INSTANCE CLASS



// =========================================================================================== AUDIO INSTANCE CLASS


// Type translators

uint64_t time_to_samples(timecode current_timecode, unsigned int sample_rate)
{
    /** 
     * We convert the entire timecode to milliseconds and perform the calculation 
     * with using integer arithmetic to avoid floating-point precision errors.
     * 
     * Using double would introduce small rounding inaccuracies because values like
     * 1/1000 cannot be represented exactly in binary form. In audio systems,
     * even tiny timing errors may accumulate or cause misalignment over long
     * durations.
     * 
     * By multiplying first and dividing at the end:
     * 
     * samples = (total_ms * sample_rate) / 1000
     * 
     * We preserve full integer precision and ensure deterministic,
     * sample-accurate result
     * 
     */


    // With use of unsigned long long
    uint64_t total_ms =
    
        static_cast<uint64_t>(current_timecode.h) * 3600000ULL +
        static_cast<uint64_t>(current_timecode.m) * 60000ULL +
        static_cast<uint64_t>(current_timecode.s) * 1000ULL +
        static_cast<uint64_t>(current_timecode.ms);


    // Returns the sample value by timecode and sample rate
    return (total_ms * sample_rate) / 1000ULL;
}


timecode samples_to_time(uint64_t sample, unsigned int sample_rate)
{
    timecode sample_time; // Return variable initialization

    // Total sample time
    uint64_t total_ms = (sample * 1000ULL) / sample_rate;

    
    // Time values calculation by the total sample time division
    // Integer division and decrementation combination used to find time values


    // 3600000 = 1000 * 60 * 60 - ms in one hour
    // Integer division to find hours value
    sample_time.h = static_cast<uint8_t>(total_ms / 3600000ULL);

    // Reminder by decrementation of the hours value, translated to the ms
    total_ms -= sample_time.h * 3600000ULL;

    // Same logic
    sample_time.m = static_cast<uint8_t>(total_ms / 60000ULL);
    total_ms -= sample_time.m * 60000ULL;

    // Seconds value by the same logic
    sample_time.s = static_cast<uint8_t>(total_ms / 1000ULL);
    total_ms -= sample_time.s * 1000ULL;

    // Ms setter by the remainder
    sample_time.ms = static_cast<uint16_t>(total_ms);

    // Timecode return 
    return sample_time;
}



Audio_instance::Audio_instance(Audio_asset* asset) : Asset_instance(asset)
{
    // Error handler
    assert(asset != nullptr);
};


Audio_instance::~Audio_instance() {

    // Clears an player_playback_map and delete the instanc e data from the players
}


const Audio_asset* Audio_instance::get_main_asset_link() const
{
    return static_cast<const Audio_asset*>(Asset_instance::get_main_asset_link());
}


// Player registration insoode the player_playback_map 
void Audio_instance::player_registry(Audio_player*)
{

}

// =========================================================================================== AUDIO INSTANCE CLASS
