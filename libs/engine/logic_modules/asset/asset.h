// asset.h

#pragma once


// =========================================================================================== IMPORT

#include <string>
#include <vector>
#include <unordered_set>

#include "../../base_modules/platform/platform.h"

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
enum asset_type {

    IMAGE_AT,      // 2D texture rendered via SDL
    AUDIO_AT,      // Audio resource
    VIDEO_AT,      // Animated or streamed media
    FONT_AT,       // Bitmap or vector font

    UNKNOWN_AT     // Placeholder for invalid or not-yet-loaded assets

};

// =========================================================================================== ASSET TYPES


// =========================================================================================== HANDLE STRUCT

// For both asset manager and asset instance manager

// Handle struct, which serves asset manager and asset instances manager
// clients for the lifetime check functional. For example: managers subscribers 
// should check both idx and gen before further actions with their current asset or
// instance
struct handle_ctx
{
    // Index of handle
    int index;

    // Generation of handle
    int generation;
    
};


// =========================================================================================== ASSET BASE CLASS


// Predeclare for friendship
class Asset_manager;


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
    friend Asset_manager;


    public:

        // ===== METHODS =====

        // Asset type getter - uses by Asset manager
        const asset_type get_type() const;

        // Asset path getter - uses by Asset manager
        const std::string& get_path() const;

        // ===== METHODS =====

    protected:

        // ===== LIFETIME =====

        /**
         * @brief Construct a generic asset.
         * 
         */
        Asset();


        // Virtual destructor.
        // Ensures proper basic class data cleanup with subclasses ~ calls.
        virtual ~Asset();

        // ===== LIFETIME =====


        // ===== DATA =====

        // Kind of this asset
        asset_type type;    


        // Path to the file on disk
        std::string source_path;

        // ===== DATA =====

};


// =========================================================================================== ASSET BASE CLASS



// =========================================================================================== IMAGE ASSET

// Image format logic enum
enum image_format 
{

    PNG_IF,
    JPG_IF,
    BMP_IF
    
};


/**
 * @brief Concrete asset representing a 2D image (texture).
 *
 * Images are expected to be used primarily by image_renderer
 * 
 * Holds the methods for basic image asset and instance creation / delete
 * 
 */
class Image_asset : public Asset 
{
    friend Asset_manager;


    public:

        // ===== METHODS =====

        // Get initial image format
        image_format get_format() const;


        // Get initial width of the image
        unsigned int get_width() const;

        // Get initial height of the image
        unsigned int get_height() const;

        // ===== METHODS =====


    protected:

        // ===== LIFETIME =====

        /**
         * @brief Constructor - load an image asset.
         *
         * @param path Path to the image file.
         * 
         */
        explicit Image_asset(const std::string& path);

        // Destructor - delete the asset and all instances.
        ~Image_asset();
             
        // ===== LIFETIME =====




    private:

        // ===== METHODS =====

        // Set initial image format - calls inside constructor
        void set_format();


        // Set initial width and height of the image - calls in constructor
        void set_sizes();

        // ===== METHODS =====


        // ===== DATA =====

        image_format initial_format;


        // Original image w-dimension
        unsigned int initial_width;

        // Original image h-dimension
        unsigned int initial_height;

        // ===== DATA =====
};



// =========================================================================================== IMAGE ASSET
