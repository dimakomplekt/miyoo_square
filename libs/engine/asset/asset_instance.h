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

        
    private:

        // Main asset pointer for instance-to-asset association and parameter access 
        const Asset* main_asset;
};



// =========================================================================================== INSTANCE CLASS


// =========================================================================================== INSTANCE SUBCLASSES



// =========================================================================================== INSTANCE SUBCLASSES
