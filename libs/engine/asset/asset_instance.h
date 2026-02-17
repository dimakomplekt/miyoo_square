// asset_instance.h

#pragma once

// =========================================================================================== IMPORT

#include "asset.h"
#include <vector>

#include <unordered_map>

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



// Image_player class predeclaration
// (realized inside the asset_player.h and asset_player.cpp)

class Image_player;


// Descartes coordinate for 2D space.
struct desc_c_2D
{

    unsigned int x;    // Coordinate by x-axes (width).
    unsigned int y;    // Coordinate by y-axes (height).

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

    desc_c_2D point_1;
    desc_c_2D point_2;

};


/*
 * Image instance subclass for copies of image assets
 * This class could work with Image_asset specific parameters and methods
 * It stores main_asset pointer by the heritage from Asset_instance base class
 * by the protected getter get_main_asset_link()
 * 
 * Image instance contains the most common parameters
 * for image refactoring - scale, mirror, rotation angle, crop map.
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



        // === IMAGE PLAYERS LIST === 

        /**
         * 
         * @brief player_registry
         * 
         * Writes an "Image_player*" to the image_instance_users map  
         * 
         * @param Image_player* image player pointer
         * 
         */
        void player_registry(Image_player*);
        

        /**
         * 
         * @brief player_removal
         * 
         * Deletes the "Image_player* from the image_instance_users map  
         * 
         * @param Image_player* image player pointer
         * 
         */
        void player_removal(Image_player*);

        // === IMAGE PLAYERS LIST === 

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


        // Current x-axes scaler getter
        float get_x_scaler() const;

        // Current y-axes scaler getter
        float get_y_scaler() const;


        /**
         * @brief Change image width size.
         *
         * Recalculates the instance parameters by the new width data.
         *
         * @param new_width New width value
         * 
         */
        void set_width(unsigned int new_width);

        // No need for getter - player works only with cropped size
        
        /**
         * @brief Change image width size.
         *
         * Recalculates the instance parameters by the new height data.
         *
         * @param new_height New height value
         * 
         */
        void set_height(unsigned int new_height);

        // No need for getter - player works only with cropped siz

        // === SCALER METHODS ===


        // === FLIP METHODS ===

        /**
         * @brief Set horizontal mirror for the image.
         * 
         * @param h_m_enable true to mirror horizontally, false to disable
         */
        void set_horizontal_mirror(bool h_m_enable);

        /**
         * @brief Set vertical mirror for the image.
         * 
         * @param v_m_enable true to mirror vertically, false to disable
         */
        void set_vertical_mirror(bool v_m_enable);

        /**
         * @brief Set both mirrors at once.
         *
         * @param h_m_enable true to mirror horizontally, false to disable
         * @param v_m_enable true to mirror vertically, false to disable
         * 
         */
        void set_mirror(bool h_m_enable, bool v_m_enable);


        // Current horizontal mirror status-flag getter
        bool get_horizontal_mirror() const;

        // Current vertical mirror status-flag getter
        bool get_vertical_mirror() const;
        
        
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
         * !!! Always setup by the initial asset sizes (scaled size drifting prevention) !!!
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
         */
        void set_new_crop_map(const crop_map_2D& new_crop_map);

        /**
         * @brief Image crop_map setter 2 
         * 
         * !!! Always setup by the initial asset sizes (scaled size drifting prevention) !!!
         * 
         * Setup the image crop_map by 2 points.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * @param point_1 Bottom left crop point by the desc_c_2D link
         * @param point_2 Top right crop point by the desc_c_2D link
         * 
         * Use like:
         * 
         * set_new_crop_map({{x_1, y_1}, {x_2, y_2}});
         * 
         */
        void set_new_crop_map(const desc_c_2D& point_1, const desc_c_2D& point_2);


        /**
         * @brief Image crop_map setter 2 
         * 
         * !!! Always setup by the initial asset sizes (scaled size drifting prevention) !!!
         * 
         * Setup the image crop_map by 2 points.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * Any points order!
         * 
         * @param x_1 1st crop point x
         * @param y_1 1st crop point y
         * @param x_2 2nd crop point x
         * @param y_2 2nd crop point y
         * 
         * Use like:
         * 
         * set_new_crop_map(x_1, y_1, x_2, y_2);
         * 
         */
        void set_new_crop_map(unsigned int x_1, unsigned int y_1, unsigned int x_2, unsigned int y_2);


        // Current crop width getter
        unsigned int get_crop_width() const;

        // Current crop height getter
        unsigned int get_crop_height() const;


        // Current scaled crop width getter
        unsigned int get_scaled_crop_width() const;

        // Current scaled crop height getter
        unsigned int get_scaled_crop_height() const;


        // === CROP METHODS ===


    private:

        const Image_asset* main_asset;

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

        
        // Image mirror flags

        // Horizontal mirror (relative to the center point)
        bool horizontal_mirror;

        // Vertical mirror (relative to the center point)
        bool vertical_mirror;


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


        // Crop horizontal dimension after scaling
        unsigned int scaled_crop_width;

        // Crop vertical dimension after scaling
        unsigned int scaled_crop_height;


        // Inner recalculation

        // NOTE:

        // Geometry-related setters currently call recalculation methods manually.
        // Can be unified later into a single rebuild step if dependencies grow.

        // Recalculate the current_width and current_height
        void reset_size();

        // Recalculate the anchor points, based on the current width and height
        // Calls at the constructor and inside the set_scaler() method;
        void reset_anchor_points();


        // === IMAGE PLAYERS LIST === 

        /**
         * 
         * Map of Image_players, which uses current asset_instance
         * 
         * Uses Image_player pointer for quick access.
         *
         */
        std::unordered_set<Image_player*> image_instance_users;

        // === IMAGE PLAYERS LIST === 
};


// =========================================================================================== IMAGE INSTANCE


// =========================================================================================== AUDIO INSTANCE



// Audio_player class predeclaration
// (realized inside the asset_player.h and asset_player.cpp)

class Audio_player;


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
    /*
    AUDIO INSTANCE DESIGN NOTES

    1) Bitrate as a base asset property
    - Bitrate is a fundamental property of an audio track.
    - It may change due to processing or platform limitations.
    - Downscaling is possible dynamically; duplicating assets is unnecessary.

    2) No playback state inside Audio_instance
    - Audio_instance does NOT track playback state (stopped/playing/paused).
    - It only stores reference points for playback (start/end samples).
    - Playback is fully controlled by Audio_player; multiple players can
        use the same Audio_instance independently.

    3) Player-owned playback modifiers
    - Parameters like pitch, speed, time stretching are owned by Audio_player.
    - Audio_instance remains lightweight and data-oriented.

    4) Tracking users
    - Audio_instance keeps track of which Audio_players currently use it
        via `audio_instance_users`.
    - When the instance is destroyed, all registered players are notified
        to clear their reference.
    - When a player is destroyed, it unregisters itself from any Audio_instance.
    */

    // To use the Audio_asset protected methods and parameters
    friend class Audio_asset;

    protected:

        /**
         * 
         * @brief Audio_instance constructor
         * 
         * Calls the Asset_instance constructor and pass the Audio_asset pointer
         * to the main_asset link, then registers itself in the asset's internal
         * list of active instances.
         * 
         * After that it initializes the current_sample_rate and current_bitrate
         * (with original size) and calculates the current_start, current_end 
         * and current_length, then set the current_playtime to {0, 0, 0, 0}.
         * 
         * @param asset Main Audio_asset object pointer 
         * 
         */
        Audio_instance(Audio_asset* asset);


        /**
         * 
         * Audio_instance destructor which calls the Asset_instance destructor - 
         * delete the object data and unregister itself from the asset's internal
         * list of active asset instances
         *
         */
        ~Audio_instance() override;
        

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

        /**
         * @brief Bitrate setter.
         * 
         * Switches the current_bitrate value
         * 
         * @param bitrate New sample rate value
         * 
         */
        void set_bitrate(unsigned int bitrate);

        /**
         * @brief Channel mode setter.
         * 
         * Switches the current_chanel_mode value
         * 
         * @param new_mode New sample rate value
         * 
         */
        void set_channel_mode(channel_mode new_mode);

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
         * @param sample Starting point in sample-count value
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
         * @param current_timecode Starting point in timecode struct value
         * 
         */
        void set_start_sample(timecode current_timecode);


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
         * @param sample Ending point in sample-count value
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
         * @param current_timecode Ending point in timecode struct value
         * 
         */
        void set_end_sample(timecode current_timecode);
        
        // === TRIM METHODS ===


        // === AUDIO PLAYERS LIST === 

        /**
         * 
         * @brief player_registry
         * 
         * Writes an "Audio_player* inside the audio_instance_users map  
         * 
         * @param Audio_player* audio player pointer
         * 
         */
        void player_registry(Audio_player*);
        

        /**
         * 
         * @brief player_removal
         * 
         * Deletes the "Audio_player* from the audio_instance_users map  
         * 
         * @param Audio_player* audio player pointer
         * 
         */
        void player_removal(Audio_player*);

        // === AUDIO PLAYERS LIST === 


    public:

        const Audio_asset* get_main_asset_link() const;

        
        // === MAIN SETTINGS ===


        // Current sample_rate getter
        unsigned int get_sample_rate() const;

        // Current bitrate getter
        unsigned int get_bitrate() const;

        // Current channel mode getter
        channel_mode get_channel_mode() const;

        // === MAIN SETTINGS ===


        // === TRIM METHODS ===

        // Start sample value getter
        uint64_t get_start_sample() const;

        // End sample value getter
        uint64_t get_end_sample() const;


        // === TRIM METHODS ===

    private:

        const Audio_asset* main_asset;


        // Current sample rate value
        unsigned int current_sample_rate;

        // Current bitrate rate value
        unsigned int current_bitrate;

        // Current channel mode 
        channel_mode current_channel_mode;

        // Start trim in samples
        uint64_t current_start_sample;

        // End trim in samples
        uint64_t current_end_sample;

        // Cached length
        uint64_t current_length_samples;


        // Length calculator
        // Called automatically inside the start sample 
        // and end sample setters
        uint64_t calculate_length();


        // === AUDIO PLAYERS LIST === 

        /**
         * 
         * Map of Audio_players which uses current asset_instance/
         * 
         * Uses Audio_player pointer for quick access.
         *
         */
        std::unordered_set<Audio_player*> audio_instance_users;

        // === AUDIO PLAYERS LIST === 

};

// =========================================================================================== AUDIO INSTANCE

// =========================================================================================== INSTANCE SUBCLASSES
