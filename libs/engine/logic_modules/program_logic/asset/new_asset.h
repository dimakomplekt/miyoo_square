// asset.h

#pragma once


// =========================================================================================== IMPORT

#include <string>
#include <vector>
#include <unordered_set>

#include "../platform/platform.h"

// =========================================================================================== IMPORT


// =========================================================================================== ASSET TYPES


/**
 * @brief Kinds of assets supported by the engine.
 *
 * This enum class categorizes all loadable resources in the game:
 * textures, audios, fonts, video, etc.
 *
 * It allows the engine to reason about what kind of resource it is working with
 * without knowing the concrete implementation.
 * 
 */
enum class Asset_type {

    IMAGE,      // 2D texture rendered via SDL
    VIDEO,      // Animated or streamed media
    AUDIO,      // Audio resource
    FONT,       // Bitmap or vector font
    UNKNOWN     // Placeholder for invalid or not-yet-loaded assets

};

// =========================================================================================== ASSET_TYPES


// =========================================================================================== ASSET BASE CLASS


/**
 * @brief Abstract base class for all assets in the engine.
 *
 * An Asset represents any external resource used by the game:
 * images, audios, fonts, video, etc.
 *
 * The base class intentionally contains only generic information
 * that applies to all asset types:
 *
 * - What kind of asset it is
 * 
 * - Where it was loaded from
 * 
 * - A virtual interface for "using" the asset
 *
 * Concrete behavior is implemented in derived classes.
 * 
 */
class Asset
{

    public:

        /**
         * @brief Construct a generic asset.
         *
         * @param type Type of the asset (image, audio, etc.)
         * @param path File path to the asset on disk.
         * 
         */
        Asset(Asset_type type, const std::string& path);


        // Virtual destructor.
        // Ensures proper basic class data cleanup with subclasses ~ calls.
        virtual ~Asset();


        // Asset type getter
        Asset_type get_type() const;

        // Asset path getter
        const std::string& get_path() const;


    protected:

        // Kind of this asset
        Asset_type type;    

        // Path to the file on disk
        std::string source_path;

};


// =========================================================================================== ASSET BASE CLASS


// =========================================================================================== ASSETS SUBCLASSES

// === Image asset ===

// Predeclare for friendship without linking recursion
class Image_instance;


// Image format logic enum
enum image_format 
{

    PNG,
    JPG,
    BMP
    
};


/**
 * @brief Concrete asset representing a 2D image (texture).
 *
 * Images are expected to be used primarily by image_renderer
 * 
 * Holds the methods for basic image asset and instance creation / delete
 * 
 */
class Image_asset : public Asset {

    friend class Image_instance;

    public:

        /**
         * @brief Constructor - load an image asset.
         *
         * @param renderer SDL renderer used to create the texture.
         * @param path Path to the image file.
         * 
         */
        explicit Image_asset(const std::string& path);

        // Destructor - delete the asset and all instances.
        ~Image_asset();
             

        // Get initial image format
        image_format get_format() const;


        // Get initial width of the image
        unsigned int get_width() const;

        // Get initial height of the image
        unsigned int get_height() const;


        // === INSTANCE WORKFLOW === 

        /**
         * @brief Create and register an asset instance.
         *
         * This method creates a new Image_instance (by Image_instance constructor)
         * and registers it inside the asset's internal instance set - std::unordered_set<Image_instance*> instances.
         *
         * Asset fully owns the lifetime of created instances.
         * 
         */
        Image_instance* add_instance();

        /**
         * @brief Unregister an asset instance.
         *
         * Calls the Image_instance destructor and removes the given Image_instance pointer
         * from the internal list of active instances associated with this asset.
         *
         * This method is called automatically and iterative by Image_asset destructor
         *
         * @param instance Pointer to the asset instance to unregister.
         * 
         */
        void delete_instance(Image_instance* instance);

        // === INSTANCE WORKFLOW


    private:

        image_format initial_format;


        // Original image w-dimension
        unsigned int initial_width;

        // Original image h-dimension
        unsigned int initial_height;


        // Set initial image format - calls in constructor
        void set_format();


        // Set initial width and height of the image - calls in constructor
        void set_sizes();


        // === INSTANCE WORKFLOW

        // Registration of asset instance inside the instances list
        void register_instance(Image_instance* instance);

        // Delete instance from instances list
        void unregister_instance(Image_instance* instance);

        // List of active asset instance addresses.
        // The container is empty on asset creation and fully owned by Asset.
        std::unordered_set<Image_instance*> instances;

        // === INSTANCE WORKFLOW
};

// === Image asset ===


// === Audio asset ===

// Timecode structure for audio time representation
struct timecode
{

    uint8_t h;
    uint8_t m;
    uint8_t s;
    uint16_t ms;

};


// Audio channel mode logic enum
enum channel_mode
{

    MONO,
    STEREO

};


// Audio format logic enum
enum audio_format 
{

    MP3,
    WMA,
    OGG,
    WAW,
    FLAC

};


// Predeclare for friendship without linking recursion
class Audio_instance;

/**
 * @brief Concrete asset representing a audio.
 *
 * Audio are expected to be used primarily by audio_player
 * 
 */
class Audio_asset : public Asset {

    friend class Audio_instance;

    public:

        /**
         * @brief Constructor - load an audio asset.
         * It could be audio or music with different input and output
         * bitrate and sample rate.
         *
         * @param path Path to the audio file.
         */
        explicit Audio_asset(const std::string& path);

        // Destructor.
        // Stop playing, deallocate and nullptr 
        ~Audio_asset() override;


        // Sample rate getter - returns the current sample rate
        unsigned int get_sample_rate() const;


        // Bitrate getter - returns the current bitrate
        unsigned int get_bitrate() const;


        // Audio length getter - returns the audio length (by link without copy),
        // as a timecode struct: h, m, s, ms
        const timecode& get_length() const;


        // Initial channel mode getter
        channel_mode get_channel_mode() const;


        // Initial format getter
        audio_format get_format() const;


        // === INSTANCE WORKFLOW === 

        /**
         * @brief Create and register an asset instance.
         *
         * This method creates a new Asset_instance and registers it
         * inside the asset's internal instance list.
         *
         * Asset fully owns the lifetime of created instances.
         */
        Audio_instance* add_instance();

        /**
         * @brief Unregister an asset instance.
         *
         * Removes the given Asset_instance pointer from the internal list
         * of active instances associated with this asset.
         *
         * This method is called automatically by the Asset_instance
         * destructor and should never be called manually.
         *
         * @param instance Pointer to the asset instance to unregister.
         */
        void delete_instance(Audio_instance* instance);

        // === INSTANCE WORKFLOW


    private:

        unsigned int initial_sample_rate;

        unsigned int initial_bitrate;


        timecode initial_audio_length;          // h, m, s, ms


        channel_mode initial_channel_mode;      // Mono, stereo...


        audio_format initial_format;            // WAW, MP3...


        // Initial sample rate inner setter
        void set_sample_rate();

        // Initial bitrate inner setter
        void set_bitrate();


        // Initial audio length inner setter
        void set_length();


        // Channel mode initial setter
        void set_channel_mode();

        // Format initial setter
        void set_audio_format();


        // === INSTANCE WORKFLOW

        // Registration of asset instance inside the instances list
        void register_instance(Audio_instance* instance);

        // Delete instance from instances list
        void unregister_instance(Audio_instance* instance);

        // List of active asset instance addresses.
        // The container is empty on asset creation and fully owned by Asset.
        std::unordered_set<Audio_instance*> instances;

        // === INSTANCE WORKFLOW
};

// === Audio asset ===


// =========================================================================================== ASSETS SUBCLASSES
