// instance_manager.h

#pragma once


// =========================================================================================== IMPORT


#include "instance.h"

// =========================================================================================== IMPORT


// =========================================================================================== NOTE

/*
    So, the logic here:

    Instance users must do IM.unsub(used_instance_handle) in their destructors (which will decrement subscribers_count inside
    the instance slot)
    
    After that IM.delete_instance_request(used_instance_handle) (which will check subscribers_count == 0 and delete the
    instance if its true). With instance delete, the instance destructor will be called, which must call 
    AM.unsub(used_instance_handle), which will decrement instance_count inside the asset slot.

    If we want to delete the asset, we must call AM.delete_asset_request(used_asset_handle),
    which will check instance_count == 0 inside the asset slot and delete the asset itself if its true. 

    So we got the 2-stage subscription system, which allows us to delete the asset only if there is no active instances
    of it, and delete the instance.

    So with correct classes or correct use inside non-OOP code, we will not have any memory leaks or dangling pointers.


*/

// =========================================================================================== NOTE


// =========================================================================================== HELPERS


/*

    I choose not to hold asset handle inside the instance slot and pass it to the instance private data
    because the instance only needs it on the construction and destruction stages, but instance slot 
    uses it for the whole lifetime of the instance. So, I decided to hold asset handle inside the instance slot
    and pass it to the instance constructor, which will hold it inside the instance private data.

*/

// Instance slot ctx
struct instance_slot_ctx {

    Instance* instance;                    // Instance

    handle_ctx handle;                     // Instance handle (index and generation)

    unsigned int subscribers_count;        // Instance subscriber count (serves to understand could we delete instance or not)

};


// =========================================================================================== HELPERS


// =========================================================================================== FRIEND CLASSES PREDECLARE

// Predeclaration for friendship setting

class Asset_instance;

// =========================================================================================== FRIEND CLASSES PREDECLARE


// =========================================================================================== INSTANCE MANAGER CLASS

class Instance_manager 
{

    // ===== Friendship ===== 

    friend Asset_manager;

    // ===== Friendship ===== 


    public:

        // ===== LIFETIME =====

        // Instance manager constructor
        // Calles once per app cycle 
        Instance_manager();

        // Instance manager destructor
        // Calles once per app cycle 
        ~Instance_manager();

        // ===== LIFETIME =====
    

        // ===== METHODS =====


        /**
         * @brief Asset instance adder (+ creator)
         * 
         * Serves to create asset instance and add it inside instance manager
         * 
         * @param asset_handle Link to the asset file inside build directory
         * 
         * @return handle_ctx of the instance constant copy to use 
         * 
         */
        const handle_ctx add_instance(handle_ctx asset_handle);


        /**
         * @brief Asset instance delete from instances list method (+ instance destructor call)
         * 
         * Serves to delete asset instance. Checks the asset instance counter and deletes
         * asset instance if there is no active instances.  
         * 
         * @param instance_handle Handle of instance to delete
         * 
         */
        bool delete_instance_request(handle_ctx instance_handle);


        /**
         * @brief Asset instance unsubscribing method
         * 
         * Serves to unsubscribe the instance from the instance manager.
         * Decrements the subscribers_count inside the instance slot.
         * 
         * @param instance_handle Handle of instance to unsubscribe
         * 
         */
        void unsub(handle_ctx instance_handle);


        /**
         * @brief Asset instance getter
         * 
         * Serves to get the asset instance by handle
         * 
         * @return Pointer to the reqested Asset_instance
         * 
         */
        const Asset_instance* get_instance(handle_ctx instance_handle) const;

        // ===== METHODS =====


        // ===== METHODS =====



    private:

        // ===== METHODS =====

        /**
         * @brief Asset instance delete from instances list method (+ instance destructor call)
         * 
         * Inner instance manager function.
         * 
         * Serves to delete asset instance after nullptr of instance link and generation incrementation
         * inside the instance list
         * 
         * @param instance Instance to delete
         * 
         */
        void delete_instance(Asset_instance* instance);
        
        // ===== METHODS =====

        
        // ===== Data =====

        /**
         * 
         * @brief Asset instances slots list.
         *
         * The vector index is directly associated with the asset instance handle index:
         *     slots[index] <-> handle_ctx.index
         *
         * This provides O(1) asset instance slot access by handle without any additional
         * lookup or search operation.
         *
         * Each vector element represents a stable logical slot. Instance deletion
         * does not remove the slot from the vector, because erasing an element
         * would shift subsequent elements and invalidate their handle indices.
         *
         * Instead, an unused slot remains inside the vector and may be reused
         * for a future asset. When a slot is reused, its generation is incremented
         * to invalidate handles referring to the previous asset. At the add operation
         * we check if the slot is free (instance == nullptr) and if it is, we use it for 
         * the new instance.
         *
         * Therefore the following invariant must always be preserved:
         *
         *     slots[N].handle.index == N
         *
         * The handle index identifies the slot, while the handle generation
         * identifies the current lifetime of the asset occupying that slot.
         * 
        */
        std::vector<instance_slot_ctx> slots;
        
        // ===== Data =====
        
};

// =========================================================================================== INSTANCE MANAGER CLASS



