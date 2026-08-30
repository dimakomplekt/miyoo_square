// asset_manager.h

#pragma once


// =========================================================================================== IMPORT


#include "asset.h"

// =========================================================================================== IMPORT


// =========================================================================================== HELPERS

// Asset slot (asset + handle)
struct asset_slot_ctx {

    Asset* asset;               // Asset himself

    std::string asset_name;     // Asset name
    handle_ctx handle;          // Asset handle (index and generation)

};


// List of asset slots in asset manager 
struct images_assets_list 
{
    // Whatever the storage logic
};


// =========================================================================================== HELPERS


// =========================================================================================== FRIEND CLASSES PREDECLARE

// Predeclaration for friendship setting

class Asset_instance;


// =========================================================================================== FRIEND CLASSES PREDECLARE





// =========================================================================================== ASSET MANAGER CLASS

class Asset_manager
{
    friend Asset_instance;


    public:

        // Manager constructor
        // Calles once per app cycle 
        Asset_manager();

        // Manager destructor
        // Calles once per app cycle 
        ~Asset_manager();


        // ===== Asset workflow =====

        /**
         * @brief Asset adder (+ creator)
         * 
         * Serves to create asset add add it inside asset manager
         * 
         * @param type Asset type
         * @param asset_name Asset name in string format. Should be unique. 
         * @param asset_link Link to the asset file inside build directory
         * 
         * @return handle_ctx constant copy to use 
         * 
         */
        const handle_ctx add_asset(asset_type type, std::string asset_name, std::string asset_link);


        /**
         * @brief Asset delete from assets list method (+ asset destructor call)
         * 
         * Serves to delete asset
         * 
         * @param asset Asset to delete
         * 
         */
        void delete_asset(Asset* asset);


        /**
         * @brief Asset getter
         * 
         * Serves to get the asset link by type and handle
         * 
         * @param type Asset type
         * @param asset_handle Asset handle
         * 
         * @return Asset link
         * 
         */
        Asset* get_asset(asset_type type, handle_ctx asset_handle) const;

        // ===== Asset workflow =====

    protected:

        /**
         * @brief Asset handle getter
         * 
         * Serves to get the asset handle by asset name.
         * Could be called only by asset instance manager
         * 
         * Uses inside add_instance() method. During the call
         * function will get the current instance handle. So in further
         * handle changes would provide controlled behavior of the program
         * cycle - returns with std::cout << of treatment errors
         * 
         * @param name Asset name
         * 
         * @return Asset handle constant copy
         * 
         */
        handle_ctx get_asset_handle_by_name(std::string asset_name) const;


    private:

        images_assets_list images_assets; 
}

// =========================================================================================== ASSET MANAGER CLASS