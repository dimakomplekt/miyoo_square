// asset.h

#pragma once


// =========================================================================================== IMPORT

#include <iostream>
#include <filesystem>

#include <string>
#include <vector>
#include <unordered_set>

// Platform
#include "../../base_modules/platform/platform.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// =========================================================================================== IMPORT


/*
    PARADIGM:
    
    Asset:
        owns the source resource loaded from disk and is shared by instances

    Instance:
        references an Asset by a generation-checked handle and owns a
        calculated/runtime representation

    Object:
        references an Instance and owns gameplay/component data

    Intended pipeline:
        1. Asset_manager::add_asset() loads one source resource.
        2. Instance_manager::add_instance() creates a typed representation.
        3. A game object subscribes to the instance and reads its runtime data
           (Image_instance::get_surface(), for example).
        4. The object unsubscribes, then the instance is deleted.
        5. The asset is deleted after its last instance is gone.

    Handles contain a slot index and generation. Always pass the complete
    handle back to the owning manager; never retain raw Asset or Instance
    pointers after the corresponding delete request.
*/


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
    int index = -1;                 // Non valid by default

    // Generation of handle
    int generation = 0;             // No generation by default
    
};


// =========================================================================================== ASSET BASE CLASS


// Predeclare for friendship
class Asset_manager;
class Instance;
class Image_instance;

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
    // ===== Friendship ===== 

    friend Asset_manager;

    // ===== Friendship ===== 


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
         * @param r_asset_manager Responsible asset manager  
         * 
         */
        Asset(Asset_manager* r_asset_manager);


        // Virtual destructor.
        // Ensures proper basic class data cleanup with subclasses ~ calls.
        // Called only by the asset manager
        virtual ~Asset();

        // ===== LIFETIME =====


        // ===== DATA =====

        // Kind of this asset
        asset_type type = UNKNOWN_AT;


        // Path to the file on disk
        std::string source_path;


        // Responsible asset manager link
        const Asset_manager* asset_manager;

        // ===== DATA =====

};


// =========================================================================================== ASSET BASE CLASS



// =========================================================================================== IMAGE ASSET


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
    friend Instance;
    friend Image_instance;

    
    public:

        // ===== METHODS =====

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
         * Called only by the asset manager.
         * 
         * @param path Path to the image file.
         * @param r_asset_manager Responsible asset manager
         * 
         */
        explicit Image_asset(const std::string& path, Asset_manager* r_asset_manager);

        // Destructor - delete the asset.
        // Called only by the asset manager.
        ~Image_asset();
            
        // ===== LIFETIME =====


        // ===== METHODS =====

        /**
         * @brief Surface provider
         * 
         * Provides ready-to-use surface to image assets instances
         * through the instance manager request
         * 
         * @return Pointer to basic image asset in SDL surface format
         *  
         */
        const SDL_Surface* provide_surface() const;

        // ===== METHODS =====


    private:

        // ===== METHODS =====

        // Set initial width and height of the image - calls ones inside constructor
        void image_load();

        // ===== METHODS =====


        // ===== DATA =====

        // Loaded image, translated into surface
        // Surface uses here because textures needs
        // link to renderer
        SDL_Surface* surface = nullptr;


        // Original image w-dimension
        unsigned int initial_width = 0;

        // Original image h-dimension
        unsigned int initial_height = 0;

        // ===== DATA =====
};



// =========================================================================================== IMAGE ASSET
