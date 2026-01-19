// asset.h

#pragma once


// =========================================================================================== IMPORT

#include <string>

#include "../../engine/platform/platform.h" // SDL linkin

// =========================================================================================== IMPORT


// =========================================================================================== ASSET TYPES


/**
 * @brief Kinds of assets supported by the engine.
 *
 * This enum class categorizes all loadable resources in the game:
 * textures, sounds, fonts, video, etc.
 *
 * It allows the engine to reason about what kind of resource it is working with
 * without knowing the concrete implementation.
 */
enum class Asset_type {

    IMAGE,      // 2D texture rendered via SDL
    VIDEO,      // Animated or streamed media
    SOUND,      // Audio resource
    FONT,       // Bitmap or vector font
    UNKNOWN     // Placeholder for invalid or not-yet-loaded assets

};

// =========================================================================================== ASSET_TYPES


// =========================================================================================== ASSET BASE CLASS


/**
 * @brief Abstract base class for all assets in the engine.
 *
 * An Asset represents any external resource used by the game:
 * images, sounds, fonts, video, etc.
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
    public:

        /**
         * @brief Construct a generic asset.
         *
         * @param type Type of the asset (image, sound, etc.)
         * @param path File path to the asset on disk.
         */
        Asset(Asset_type type, const std::string& path);


        // Virtual destructor.
        // Ensures proper cleanup when deleting derived assets through a base pointer.
        virtual ~Asset();


        /**
         * @brief Abstract "use" method for all assets.
         *
         * Each concrete asset decides what "using" means:
         *
         * - Image: render to screen
         * 
         * - Sound: play audio
         * 
         * - Font: prepare glyphs
         * 
         * - Video: start playback
         *
         * This method defines a common interface for the engine.
         */
        virtual void use() = 0;


        // Asset type getter
        Asset_type get_type();

        // Asset path getter
        std::string get_path();


    protected:

        // Kind of this asset
        Asset_type type;    

        // Path to the file on disk
        std::string source_path;
};


// =========================================================================================== ASSET BASE CLASS


// =========================================================================================== ASSETS SUBCLASSES


/**
 * @brief Concrete asset representing a 2D image (texture).
 *
 * This class wraps an SDL_Texture and adds:
 *
 * - Scaling support
 * 
 * - Automatic size tracking
 * 
 * - Anchor points for positioning
 * 
 * - A rendering helper method
 *
 * Images are expected to be used primarily by game objects,
 * such as characters, UI elements, or tiles.
 */
class Image_asset : public Asset {

    public:

        /**
         * @brief Constructor - load an image asset.
         *
         * @param renderer SDL renderer used to create the texture.
         * @param path Path to the image file.
         */
        Image_asset(SDL_Renderer* renderer, const std::string& path);

        // Destructor.
        // Releases the SDL texture if it exists.
        ~Image_asset();
             

        // Default override for image asset type
        void use() override {
            // Render with default settings 
            render_image_at_point({0,0}, this->anchors.center_center);
        }
    
        /**
         * @brief Override for image asset type with parameters 
         * 
         * Render the image at a given point.
         * 
         * The point is interpreted as a base reference location
         * (for example, center, top-left, etc., depending on future design).
         *
         * @param drawing_point Base position in screen space.
         * @param asset_basic_point Base asset anchor.
         */
        void use(SDL_FPoint drawing_point, SDL_FPoint asset_anchor) {

            render_image_at_point(drawing_point, asset_anchor);

        }

        // Get current scaled width of the image.
        int get_width();

        // Get current scaled height of the image.
        int get_height();


        /**
         * @brief Change image scale.
         *
         * Recomputes current width and height based on the original size.
         *
         * @param new_scaler Scale factor (1.0 = original size).
         */
        void set_scaler(float new_scaler);


        // Access raw SDL texture. Useful for advanced rendering operations.
        SDL_Texture* get_texture();


    private:

        // Underlying SDL texture
        SDL_Texture* texture = nullptr;

        // Original image w-dimension
        int initial_width;
        // Original image h-dimension
        int initial_height;

        // Current image scale factor
        float scaler = 1.0f;
        
        // Scaled w-dimension
        int current_width;

        // Scaled h-dimension
        int current_height;


        /**
         * @brief Nine key anchor points of the image.
         *
         * These points allow flexible alignment:
         *
         *  [TL]---[TC]---[TR]
         *   |      |      |
         *  [CL]---[CC]---[CR]
         *   |      |      |
         *  [BL]---[BC]---[BR]
         *
         * This is useful for positioning sprites relative to
         * characters, physics bodies, or UI layout.
         */
        struct Anchor_points {

            SDL_FPoint top_left;
            SDL_FPoint top_center;
            SDL_FPoint top_right;
            SDL_FPoint center_left;
            SDL_FPoint center_center;
            SDL_FPoint center_right;
            SDL_FPoint bottom_left;
            SDL_FPoint bottom_center;
            SDL_FPoint bottom_right;

        } anchors;


        /**
         * @brief Render the image at a given point.
         * 
         * The point is interpreted as a base reference location
         * (for example, center, top-left, etc., depending on future design).
         *
         * @param drawing_point Base position in screen space.
         * @param asset_basic_point Base asset anchor.
         */
        void render_image_at_point(SDL_FPoint drawing_point, SDL_FPoint asset_basic_point);
};


// =========================================================================================== ASSETS SUBCLASSES
