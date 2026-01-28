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

    friend class Asset; // Asset could call the ~Asset_instance()

    protected:

           /**
         * @brief Construct an asset instance.
         *
         * This constructor creates a new instance of an asset and automatically
         * associates it with its parent Asset. The instance stores a pointer
         * to the main Asset (`main_asset`) and immediately registers itself
         * in the asset's internal list of active instances by calling:
         *
         *     main_asset->add_instance(this);
         *
         * This ensures that the Asset is aware of all its instances and can
         * properly manage their lifetimes, including destruction when the
         * Asset itself is deleted.
         *
         * @param asset Pointer to the Asset this instance is derived from.
         */    
        explicit Asset_instance(Asset* asset);


        virtual ~Asset_instance(); // Only called by the Asset::delete_instance(instance);

        // Main asset link getter
        Asset* get_main_asset_link() const;

        
    private:

        // Main asset pointer for instance-to-asset association and parameter access 
        const Asset* main_asset;
};



// =========================================================================================== INSTANCE CLASS


// =========================================================================================== INSTANCE SUBCLASSES


// =========================================================================================== IMAGE INSTANCE


// Decart coordinate for 2D space.
struct dec_c_2D
{

    float x;    // Coordinate by x-axes (width).
    float y;    //Coordinate by y-axes (height).

};


// Rectangle size for 2D space.
struct size_2D
{

    unsigned int w;
    unsigned int h;

};

// Crop map for 2D space
struct crop_map_2D {

    dec_c_2D top_left;
    dec_c_2D bottom_right;

};


// Image instance subclass for copies of image assets
// This class could work with Image_asset specific parameters and methods
// It stores main_asset pointer by the heritage from Asset_instance base class
// by the protected getter get_main_asset_link()
class Image_instance : public Asset_instance
{

    friend class Image_asset;

    public:

        // Image_instance constructor, which calls the Asset_instance constructor and 
        // pass the Image_asset pointer to the main_asset link, then registers itself 
        // in the asset's internal list of active instances.
        //
        // After that it initializes the scale factors to 1.0 (original size) and calculates
        // the current_width, current_height and anchor points
        Image_instance(Image_asset* asset);

        // Image_instance destructor which calls the Asset_instance destructor - delete
        // the object data and unregister itself from the asset's internal list of active asset
        // instances
        ~Image_instance() override;


        // === CROP METHODS ===

        /**
         * @brief Setup the image cropmap by cropmap link.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * @param new_crop_map Crop map by the crop_map_2D link
         * 
         * Use like:
         * 
         * image_instance.set_crop_map(crop);
         */
        void set_crop_map(const crop_map_2D& new_crop_map);

        /**
         * @brief Setup the image cropmap by 2 points.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * @param top_left Top left crop point by the dec_c_2D link
         * @param bottom_right Bottom rigth crop point by the dec_c_2D link
         * 
         * Use like:
         * 
         * set_crop_map({{x_1, y_1}, {x_2, y_2}});
         * 
         */
        void set_crop_map(const dec_c_2D& top_left, const dec_c_2D& bottom_right);

        // === CROP METHODS ===


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


    protected:

        Image_asset* get_main_asset_link() const
        {
            return static_cast<Image_asset*>(Asset_instance::get_main_asset_link());
        }


    private:

        // Current crop map by 2 points
        crop_map_2D crop_map;

        // Current image scale factor x-axes
        float x_scaler;

        // Current image scale factor y-axes
        float y_scaler;

        // Scaled w-dimension
        unsigned int current_width;

        // Scaled h-dimension
        unsigned int current_height;


        /**
         * @brief Nine key anchor points of the image in local (unrotated) space.
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

            dec_c_2D top_left;
            dec_c_2D top_center;
            dec_c_2D top_right;
            dec_c_2D center_left;
            dec_c_2D center_center;
            dec_c_2D center_right;
            dec_c_2D bottom_left;
            dec_c_2D bottom_center;
            dec_c_2D bottom_right;

        } anchors;

        // Recalculate the anchor points, based on the current width and height
        // Calls at the constructor and inside the set_scaler() method;
        void get_new_anchor_points();


        // Image flip flags

        // Horizontal flip (relative to the center point)
        bool horizontal_flip;

        // Vertical flip (relative to the center point)
        bool vertical_flip;


        // Asset instance rotation

        // Rotation angle in degrees (clockwise)
        float rotation_angle;
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



class Audio_instance : public Asset_instance
{

    friend class Audio_asset;

    public:

        
        // Audio_instance constructor, which calls the Asset_instance constructor and 
        // pass the Audio_asset pointer to the main_asset link, then registers itself 
        // in the asset's internal list of active instances.
        //
        // After that it initializes the current_sample_rate and current_bitrate (with original size)
        // and calculates the current_start, current_end and current_length, then set the urrent_playtime
        // to {0, 0, 0, 0}.
        Audio_instance(Audio_asset* asset);

        // Audio_instance destructor which calls the Asset_instance destructor - delete
        // the object data and unregister itself from the asset's internal list of active asset
        // instances
        ~Audio_instance() override;


        // === TRIM METHODS ===


        /**
         * @brief Start sample value setter
         * 
         * Setup the start_sample by the time-sample translator.
         * 
         * Use like:
         * 
         * set_start_sample(time_to_samples(timecode, sample_rate));
         * 
         */
        void set_start_sample(uint64_t sample);

        uint64_t get_start_sample() const;

        


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
};

// =========================================================================================== AUDIO INSTANCE

// =========================================================================================== INSTANCE SUBCLASSES
