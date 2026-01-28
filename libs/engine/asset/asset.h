// asset.h

#pragma once


// =========================================================================================== IMPORT

#include <string>
#include <vector>
#include <unordered_set>

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



// Asset instance class forward declaration
// (realized inside the asset_instance.h and asset_instance.cpp)

class Asset_instance;


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
 */
class Asset
{

    // Instance is the fried-class for the asset class for the protected methods 
    // permission - "add_instance(Asset_instance* instance)" and "delete_instance(Asset_instance* instance)"
    friend class Asset_instance;


    public:

        /**
         * @brief Construct a generic asset.
         *
         * @param type Type of the asset (image, audio, etc.)
         * @param path File path to the asset on disk.
         */
        Asset(Asset_type type, const std::string& path);


        // Virtual destructor.
        // Ensures proper cleanup when deleting derived assets through a base pointer.
        virtual ~Asset();


        /**
         * @brief Use method for all assets.
         *
         * Returns the type and path for players by the
         * get_type() and get_path() calls.
         *
         * This method defines a common interface for the engine.
         */
        void use() const;


        // Asset type getter
        Asset_type get_type() const;

        // Asset path getter
        const std::string& get_path() const;


    protected:

        // Kind of this asset
        Asset_type type;    

        // Path to the file on disk
        std::string source_path;

        
        /**
         * @brief Create and register an asset instance.
         *
         * This method creates a new Asset_instance and registers it
         * inside the asset's internal instance list.
         *
         * Asset fully owns the lifetime of created instances.
         */
        Asset_instance* add_instance();

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
        void delete_instance(Asset_instance* instance);


    private:
        
        // List of active asset instance addresses.
        // The container is empty on asset creation and fully owned by Asset.
        std::unordered_set<Asset_instance*> instances;
};


// =========================================================================================== ASSET BASE CLASS


// =========================================================================================== ASSETS SUBCLASSES


/**
 * @brief Concrete asset representing a 2D image (texture).
 *
 * Images are expected to be used primarily by image_renderer
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
         */
        Image_asset(const std::string& path);

        // Destructor.
        ~Image_asset() override;
             

        // Get initial width of the image.
        unsigned int get_width() const;

        // Get initial height of the image.
        unsigned int get_height() const;


    private:

        // Original image w-dimension
        unsigned int initial_width;
        // Original image h-dimension
        unsigned int initial_height;
};


/**
 * @brief Concrete asset representing a audio.
 *
 * Audio are expected to be used primarily by audio_player
 * 
 */
class Audio_asset : public Asset {

    public:

        /**
         * @brief Constructor - load an audio asset.
         * It could be audio or music with different input and output
         * bitrate and sample rate.
         *
         * @param path Path to the audio file.
         */
        Audio_asset(const std::string& path);

        // Destructor.
        // Stop playing, deallocate and nullptr 
        ~Audio_asset() override;


        // Sample rate getter - returns the current sample rate
        unsigned int get_sample_rate() const;


        // Bitrate getter - returns the current bitrate
        unsigned int get_bitrate() const;


        // Audio length getter - returns the audio length (by link without copy),
        // as a vector with size of [4] and format: <h, m, s, ms>
        const std::vector<unsigned int>& get_length() const;


    private:

        unsigned int initial_sample_rate;
        unsigned int initial_bitrate;

        std::vector<unsigned int> initial_audio_length; // <h, m, s, ms>
};

// =========================================================================================== ASSETS SUBCLASSES
