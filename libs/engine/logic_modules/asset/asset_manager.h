// asset_manager.h

#pragma once


// =========================================================================================== IMPORT


#include "asset.h"

// =========================================================================================== IMPORT


// =========================================================================================== HELPERS

// Asset slot (asset + handle)
struct asset_slot_ctx {

    Asset* asset;                       // Asset himself

    handle_ctx handle;                  // Asset handle (index and generation)

    unsigned int instance_count;        // Instance counter (serves to understand could we delete asset or not)

};


// =========================================================================================== HELPERS


// =========================================================================================== FRIEND CLASSES PREDECLARE

// Predeclaration for friendship setting

class Asset_instance;
class Image_instance;
class Audio_instance;

// =========================================================================================== FRIEND CLASSES PREDECLARE





// =========================================================================================== ASSET MANAGER CLASS

class Asset_manager
{
    // ===== Friendship ===== 

    friend Asset_instance;
    friend Image_instance;
    friend Audio_instance;

    // ===== Friendship ===== 

    public:

        // Manager constructor
        // Calles once per app cycle 
        Asset_manager();

        // Manager destructor
        // Calles once per app cycle 
        ~Asset_manager();


        // ===== METHODS =====

        /**
         * @brief Asset adder (+ creator)
         * 
         * Serves to create asset add add it inside asset manager
         * 
         * @param type Asset type
         * @param asset_link Link to the asset file inside build directory
         * 
         * @return handle_ctx constant copy to use 
         * 
         */
        const handle_ctx add_asset(asset_type type, std::string asset_link);


        /**
         * @brief Asset delete from assets list method (+ asset destructor call)
         * 
         * Serves to delete asset. Checks the asset instance counter and deletes
         * asset if there is no active instances.  
         * 
         * @param asset_handle Handle of asset to delete
         * 
         */
        bool delete_asset_request(handle_ctx asset_handle);

        // ===== METHODS =====


    protected:

        // ===== METHODS =====

        /**
         * @brief Asset getter
         * 
         * Serves to get the asset link by handle
         * 
         * Could be called only by INSTANCE MANAGERS (during
         * the reason of "at least one instance to work with 
         * asset" workflow).
         * 
         * @return Pointer to the requested Asset
         * 
         * @return Asset link
         * 
         */
        Asset* get_asset(handle_ctx asset_handle) const;

        // ===== METHODS =====

    private:

        // ===== METHODS =====

        /**
         * @brief Asset delete from assets list method (+ asset destructor call)
         * 
         * Inner asset manager function.
         * 
         * Serves to delete asset after nullptr of asset link and generation incrementation
         * inside the asset list
         * 
         * @param asset Asset to delete
         * 
         */
        void delete_asset(Asset* asset);
        
        // ===== METHODS =====


        // ===== Data =====

        /**
         * 
         * @brief Asset slots list.
         *
         * The vector index is directly associated with the asset handle index:
         *     slots[index] <-> handle_ctx.index
         *
         * This provides O(1) asset slot access by handle without any additional
         * lookup or search operation.
         *
         * Each vector element represents a stable logical slot. Asset deletion
         * does not remove the slot from the vector, because erasing an element
         * would shift subsequent elements and invalidate their handle indices.
         *
         * Instead, an unused slot remains inside the vector and may be reused
         * for a future asset. When a slot is reused, its generation is incremented
         * to invalidate handles referring to the previous asset.
         *
         * Therefore the following invariant must always be preserved:
         *
         *     slots[N].handle.index == N
         *
         * The handle index identifies the slot, while the handle generation
         * identifies the current lifetime of the asset occupying that slot.
        */
        std::vector<asset_slot_ctx> slots;
        
        // ===== Data =====
};

// =========================================================================================== ASSET MANAGER CLASS
