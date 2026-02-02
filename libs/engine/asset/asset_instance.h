// asset_instance.h

#pragma once

// =========================================================================================== IMPORT

#include "asset.h"

// =========================================================================================== IMPORT


// =========================================================================================== INSTANCE CLASS

// Instance is the copy of the asset, which used for implementing multiple actions with multiple assets 
// copies, without the storage of the repetitive data (like links, default settings and ect.)
//
// For every asset use we should create an asset instance and work with it, as we want,
// by the asset player using.
//
// Instance is the fried-class for the asset class for the protected methods 
// permission - "add_instance(Asset_instance* instance)" and "delete_instance(Asset_instance* instance)"
class Asset_instance
{   

    friend class Asset; // Methods providing


    // Call only by Asset class
    protected:

           /**
         * @brief Construct an asset instance.
         *
         * This constructor calls from the instance_type constructors.
         * It creates a new instance of an asset and automatically
         * associates it with its parent Asset. The instance stores a pointer
         * to the main Asset (`main_asset`) and immediately registers itself
         * in the asset's internal list of active instances by calling:
         *
         *      type_asset->add_instance(this);
         *
         * This ensures that the Asset is aware of all its instances and can
         * properly manage their lifetimes, including destruction when the
         * Asset itself is deleted.
         *
         * @param asset Pointer to the Asset this instance is derived from.
         */    
        explicit Asset_instance(Asset* asset);


        /* Basic asset instance destructor.
         * Called automatically by the childrens destructors 
         * (which called by the Asset::delete_instance(instance);)
         */
        virtual ~Asset_instance();


    public:

        // Main asset link getter
        const Asset* get_main_asset_link() const;

        
    private:

        // Main asset pointer for instance-to-asset association and parameter access 
        const Asset* main_asset;
};



// =========================================================================================== INSTANCE CLASS


// =========================================================================================== INSTANCE SUBCLASSES


// =========================================================================================== IMAGE INSTANCE


// Descartes coordinate for 2D space.
struct desc_c_2D
{

    float x;    // Coordinate by x-axes (width).
    float y;    // Coordinate by y-axes (height).

};


// Rectangle size for 2D space.
struct size_2D
{

    unsigned int w;
    unsigned int h;

};

// Crop map for 2D space
struct crop_map_2D {

    // Points choosen for width - height / x - y values coincidence

    desc_c_2D bottom_left;
    desc_c_2D top_right;

};


/*
 * Image instance subclass for copies of image assets
 * This class could work with Image_asset specific parameters and methods
 * It stores main_asset pointer by the heritage from Asset_instance base class
 * by the protected getter get_main_asset_link()
 * 
 * Image instance contains the most common parameters
 * for image refactoring - scale, flip, rotation angle, crop map.
 * 
 * Image instance data uses for quick access to the common image asset parameters
 * without calculation on every render step. 
 * 
 * Asset player works faster with using of these parameters. 
 * 
 * More memory, but less CPU usage. 
 * 
 */ 
class Image_instance : public Asset_instance
{

    friend class Image_asset;   // Methods providing

    // Call only by Image_asset class
    protected:

        /**
         * Image_instance constructor, which calls the Asset_instance constructor and 
         * pass the Image_asset pointer to the main_asset link, then registers itself 
         * in the asset's internal list of active instances.
         *
         * After that it initializes the scale factors to 1.0 (original size) and calculates
         * the current_width, current_height and anchor points
         * 
         * Calls from:
         * 
         *      Image_asset::add_instance();
         * 
         */
        explicit Image_instance(Image_asset* asset);

        /*
         * Image_instance destructor which called by the Assets class instance indestructor method.
         * Delete the object data and unregister instance from the asset's internal list of active asset
         * instances
         * 
         * Calls from:
         * 
         *      Image_asset::delete_instance(instance);
         * 
         * or
         * 
         *      Asset::~Asset();
         * 
         */
        ~Image_instance() override;

    public:

        // Basic asset link getter override.
        // Returns the link to the main Image_asset
        const Image_asset* get_main_asset_link() const;


        // === SCALER METHODS ===

        /**
         * @brief Change image scale.
         *
         * Recomputes current width and height based on the original size.
         *
         * @param x_scaler Scale factor x-axes (1.0 = original size).
         * @param y_scaler Scale factor y-axes (1.0 = original size).
         * 
         */
        void set_scaler(float x_scaler, float y_scaler);


        /**
         * @brief Change image width size.
         *
         * Recalculates the instance parameters by the new width data.
         *
         * @param new_width New width value
         * 
         */
        void set_width(unsigned int new_width);

        
        /**
         * @brief Change image width size.
         *
         * Recalculates the instance parameters by the new height data.
         *
         * @param new_height New height value
         * 
         */
        void set_height(unsigned int new_height);


        // === SCALER METHODS ===


        // === FLIP METHODS ===

        /**
         * @brief Set horizontal flip for the image.
         * 
         * @param h_f_enable true to flip horizontally, false to disable
         */
        void set_horizontal_flip(bool h_f_enable);

        /**
         * @brief Set vertical flip for the image.
         * 
         * @param v_f_enable true to flip vertically, false to disable
         */
        void set_vertical_flip(bool v_f_enable);

        /**
         * @brief Set both flips at once.
         *
         * @param h_f_enable true to flip horizontally, false to disable
         * @param v_f_enable true to flip vertically, false to disable
         * 
         */
        void set_flip(bool h_f_enable, bool v_f_enable);
        
        // === FLIP METHODS ===


        // === ROTATION METHODS ===

        /**
         * @brief Set image rotation angle.
         *
         * @param angle_deg Rotation angle in degrees.
         */
        void set_angle(float angle_deg);

        /**
         * @brief Add rotation to current angle.
         *
         * @param delta_angle_deg Angle delta in degrees.
         */
        void add_angle(float delta_angle_deg);

        // Current rotation angle getter
        float get_angle() const;

        // === ROTATION METHODS ===


        // === CROP METHODS ===

        /**
         * @brief Image crop map setter 1 
         * 
         * Setup the image crop map by crop_map link.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * @param new_crop_map Crop map by the crop_map_2D link
         * 
         * Use like:
         * 
         * image_instance.set_new_crop_map(crop);
         * 
         *      !!!  CROP MAP CONSTRAINS  !!!
         * 
         *         x_1 >= 0;   x_2 >= x_1;
         * 
         *         y_1 >= 0;   y_2 >= y_1;
         * 
         *      !!!  CROP MAP CONSTRAINS  !!!
         * 
         */
        void set_new_crop_map(const crop_map_2D& new_crop_map);

        /**
         * @brief Image crop_map setter 2 
         * 
         * Setup the image crop_map by 2 points.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * @param bottom_left Top left crop point by the desc_c_2D link
         * @param top_right Bottom right crop point by the desc_c_2D link
         * 
         * Use like:
         * 
         * set_new_crop_map({{x_1, y_1}, {x_2, y_2}});
         * 
         *      !!!  CROP MAP CONSTRAINS  !!!
         * 
         *         x_1 >= 0;   x_2 >= x_1;
         * 
         *         y_1 >= 0;   y_2 >= y_1;
         * 
         *      !!!  CROP MAP CONSTRAINS  !!!
         * 
         */
        void set_new_crop_map(const desc_c_2D& bottom_left, const desc_c_2D& top_right);


        // Current crop width getter
        unsigned int get_crop_width() const;

        // Current crop height getter
        unsigned int get_crop_height() const;

        // === CROP METHODS ===


    private:

        // Current image scale factor x-axes
        float x_scaler;

        // Current image scale factor y-axes
        float y_scaler;

        // Scalers cache for crop reset
        float last_x_scaler;
        float last_y_scaler; 


        // Scaled w-dimension
        unsigned int current_width;

        // Scaled h-dimension
        unsigned int current_height;

        
        // Image flip flags

        // Horizontal flip (relative to the center point)
        bool horizontal_flip;

        // Vertical flip (relative to the center point)
        bool vertical_flip;


        // Asset instance rotation

        // Rotation angle in degrees (clockwise)
        float rotation_angle;


        // Current crop map by 2 points
        crop_map_2D crop_map;

                                                    
        /**
         * @brief Nine key anchor points of the image in local (not rotated) space.
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
         * 
         * This is useful for positioning sprites relative to
         * characters, physics bodies, or UI layout.
         */
        struct Anchor_points {

            desc_c_2D top_left;
            desc_c_2D top_center;
            desc_c_2D top_right;
            desc_c_2D center_left;
            desc_c_2D center_center;
            desc_c_2D center_right;
            desc_c_2D bottom_left;
            desc_c_2D bottom_center;
            desc_c_2D bottom_right;

        } anchors;


        // Crop horizontal dimension
        unsigned int crop_width;

        // Crop vertical dimension
        unsigned int crop_height;

        // Inner recalculation


        // NOTE:
        // geometry-related setters currently call recalculation methods manually.
        // Can be unified later into a single rebuild step if dependencies grow.

        // Recalculate the current_width and current_height
        void reset_size();

        // Recalculate the crop map by the old crop map and dependent parameters
        void reset_crop_map();

        // Recalculate the anchor points, based on the current width and height
        // Calls at the constructor and inside the set_scaler() method;
        void reset_anchor_points();
};


// =========================================================================================== IMAGE INSTANCE


// =========================================================================================== AUDIO INSTANCE


/**
 * @brief Convert timecode to samples
 * 
 * @param current_timecode Current timecode number
 * @param sample_rate Current sample rate 
 * 
 */
uint64_t time_to_samples(timecode current_timecode, unsigned int sample_rate);

/**
 * @brief Convert samples to timecode
 * 
 * @param sample Current sample number
 * @param sample_rate Current sample rate 
 * 
 */
timecode samples_to_time(uint64_t sample, unsigned int sample_rate);


// Audio instance subclass for copies of audio assets
// This class could work with Audio_asset specific parameters and methods
// It stores main_asset pointer by the heritage from Asset_instance base class -
// by the protected getter get_main_asset_link()
class Audio_instance : public Asset_instance
{
    // To use the Audio_asset protected methods and parameters
    friend class Audio_asset;

    protected:

        
        // Audio_instance constructor, which calls the Asset_instance constructor and 
        // pass the Audio_asset pointer to the main_asset link, then registers itself 
        // in the asset's internal list of active instances.
        //
        // After that it initializes the current_sample_rate and current_bitrate (with original size)
        // and calculates the current_start, current_end and current_length, then set the current_playtime
        // to {0, 0, 0, 0}.
        Audio_instance(Audio_asset* asset);

        // Audio_instance destructor which calls the Asset_instance destructor - delete
        // the object data and unregister itself from the asset's internal list of active asset
        // instances
        ~Audio_instance() override;


    public:

        const Audio_asset* get_main_asset_link() const;

        // === MAIN SETTINGS ===

        /**
         * @brief Sample rate setter.
         * 
         * Switches the current_sample_rate value
         * 
         * @param sample_rate New sample rate value
         * 
         */
        void set_sample_rate(unsigned int sample_rate);

        // Current sample_rate getter
        unsigned int get_sample_rate() const;


        /**
         * @brief Bitrate setter.
         * 
         * Switches the current_bitrate value
         * 
         * @param bitrate New sample rate value
         * 
         */
        void set_bitrate(unsigned int bitrate);

        // Current bitrate getter
        unsigned int get_bitrate() const;

        // === MAIN SETTINGS ===


        // === TRIM METHODS ===


        /**
         * @brief Start sample value setter 1
         * 
         * Setup the start_sample by the sample or time-sample translator.
         * 
         * Use like:
         * 
         * set_start_sample(current_sample);
         * 
         * set_start_sample(time_to_samples(current_timecode, current_instance.current_sample_rate));
         * 
         */
        void set_start_sample(uint64_t sample);

        
        /**
         * @brief Start sample value setter 2
         * 
         * Setup the start_sample by the timecode and current sample rate.
         * 
         * Use like:
         * 
         * set_start_sample(timecode);
         * 
         */
        void set_start_sample(timecode current_timecode);


        // Start sample value getter
        uint64_t get_start_sample() const;


        /**
         * @brief End sample value setter 1
         * 
         * Setup the end_sample by the sample or time-sample translator.
         * 
         * Use like:
         * 
         * set_end_sample(current_sample);
         * 
         * set_end_sample(time_to_samples(current_timecode, current_instance.current_sample_rate));
         * 
         */
        void set_end_sample(uint64_t sample);

        
        /**
         * @brief End sample value setter 2
         * 
         * Setup the end_sample by the timecode and current sample rate.
         * 
         * Use like:
         * 
         * set_end_sample(timecode);
         * 
         */
        void set_end_sample(timecode current_timecode);


        // End sample value getter
        uint64_t get_end_sample() const;


        // === TRIM METHODS ===


        // === UPDATE ===

        /**
         * @brief Update playback cursor by the sample delta 
         * 
         * Called by the audio player to update the current playback
         * position, while playing the audio instance
         * 
         * @param sample_delta Number of samples to advance the playback cursor
         * 
         */
        uint64_t playback_update(uint64_t sample_delta);

        // === UPDATE ===


    private:

        // Current sample rate value
        unsigned int current_sample_rate;

        // Current bitrate rate value
        unsigned int current_bitrate;


        // Start trim in samples
        uint64_t start_sample;

        // End trim in samples
        uint64_t end_sample;

        // Cached length
        uint64_t length_samples;

        // Last known playback cursor
        uint64_t current_playtime_sample;


        // Length calculator
        // Called automatically inside the start sample 
        // and end sample setters
        uint64_t calculate_length();

};

// =========================================================================================== AUDIO INSTANCE

// =========================================================================================== INSTANCE SUBCLASSES
