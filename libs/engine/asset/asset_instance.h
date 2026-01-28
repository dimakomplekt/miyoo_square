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


        /**
         * @brief Change image scale.
         *
         * Recomputes current width and height based on the original size.
         *
         * @param x_scaler Scale factor x-axes (1.0 = original size).
         * @param y_scaler Scale factor y-axes (1.0 = original size).
         */
        void set_scaler(float x_scaler, float y_scaler);

    protected:

        Image_asset* get_main_asset_link() const
        {
            return static_cast<Image_asset*>(Asset_instance::get_main_asset_link());
        }


    private:

        // Current image scale factor x-axes
        float x_scaler;

        // Current image scale factor y-axes
        float y_scaler;


        // Scaled w-dimension
        unsigned int current_width;

        // Scaled h-dimension
        unsigned int current_height;


        /**
         * @brief Nine key anchor points of the image for the initial scale.
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

        // Recalculate the anchor points, based on the current width and height
        // Calls at the constructor and inside the set_scaler() method;
        void get_new_anchor_points();
}

// =========================================================================================== INSTANCE SUBCLASSES
