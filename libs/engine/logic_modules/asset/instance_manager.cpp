// instance_manager.cpp

// =========================================================================================== IMPORT

#include "instance_manager.h"

// =========================================================================================== IMPORT



// =========================================================================================== INSTANCE MANAGER CLASS

// ===== LIFETIME =====

// Instance manager constructor
// Calles once per app cycle 
Instance_manager::Instance_manager(Asset_manager* used_asset_manager)
{
    this->asset_manager = used_asset_manager;
}


void Instance_manager::instance_manager_delete()
{
    bool slots_list_free = false;

    if (this->slots.empty())
    {
        slots_list_free = true;
    }
    else
    {
        slots_list_free = true;

        for (size_t i = 0; i < this->slots.size(); i++)
        {
            instance_slot_ctx* slot = &this->slots[i];

            if (slot->instance != nullptr)
            {
                slots_list_free = false;

                break;
            }
        }
    }

    // Stop delete operation if there is some slots 
    if (!slots_list_free) return;

    // The caller owns the manager object and performs the final delete.
    if (slots_list_free)
    {
        std::cout << "Instance_manager is ready for deletion\n";
    }
}


// Instance manager destructor
// Calles once per app cycle 
Instance_manager::~Instance_manager() {}

// ===== LIFETIME =====


// ===== METHODS =====

const handle_ctx Instance_manager::add_instance(asset_type type, handle_ctx asset_handle)
{

    if (type < IMAGE_AT || type >= UNKNOWN_AT)
    {
        std::cout << "\nWrong asset type pass!\n" << std::endl;

        return {};
    }


    asset_slot_ctx* asset_slot = asset_manager->get_asset_slot(asset_handle);

    if (asset_slot == nullptr || asset_slot->asset == nullptr)
    {
        std::cout << "\nEmpty asset link!\n" << std::endl;

        return {};
    }

    


    // Prepare handle for new asset
    handle_ctx handle_to_use = this->get_free_handle();


    // Add asset
    switch (type)
    {
        case IMAGE_AT:
        {
            if (asset_slot->asset->get_type() != IMAGE_AT)
            {
                std::cout << "\nAsset type does not match requested instance type!\n" << std::endl;
                return {};
            }

            // Create instance
            Image_instance* new_instance = new Image_instance(handle_to_use, this, this->asset_manager);

            
            // Image not passed handler
            if (new_instance->surface == nullptr)
            {
                std::cout << "\nFailed to create image asset instance!\n" << std::endl;

                delete new_instance;

                return {};
            }
            else
            {
                // Add instance to instances list
                this->slots[handle_to_use.index].instance = new_instance;
            }

            break;
        }

        case AUDIO_AT:
        {
            std::cout << "\nAudio instances are not implemented yet!\n" << std::endl;
            return {};
        }


        case VIDEO_AT:
        {
            std::cout << "\nVideo instances are not implemented yet!\n" << std::endl;
            return {};
        }


        case FONT_AT:
        {
            std::cout << "\nFont instances are not implemented yet!\n" << std::endl;
            return {};
        }


        default:
            return {};
    }


    // The caller that creates an instance owns its first subscription.
    // Additional owners must call sub(), and every owner must call unsub().
    this->slots[handle_to_use.index].subscribers_count = 1;

    // Increment subscribed instances count inside asset slot
    asset_slot->instance_count += 1;


    return handle_to_use;
}



bool Instance_manager::delete_instance_request(handle_ctx instance_handle)
{
    if (instance_handle.index < 0 ||
        instance_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed instance\n" << std::endl;

        return false;
    }


    if (instance_handle.generation != this->slots[instance_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation instance\n" << std::endl;

        return false;
    }


    if (this->slots[instance_handle.index].instance == nullptr)
    {
        std::cout << "\nInstance does not exist already\n" << std::endl;

        return false;
    }


    if (this->slots[instance_handle.index].subscribers_count == 0)
    {
        // Delete
        this->delete_instance(this->slots[instance_handle.index].instance);

        // Nullptr
        this->slots[instance_handle.index].instance = nullptr;


        // Switch the generation number to
        // block the operations with unsubscribed old
        // subs (error case 2nd stage preventing)
        this->slots[instance_handle.index].handle.generation += 1;


        return true;
    }


    return false;
}


void Instance_manager::unsub(handle_ctx instance_handle)
{
    if (instance_handle.index < 0 ||
    instance_handle.index >= this->slots.size())
    {
        std::cout << "\nRequest to non existed instance\n" << std::endl;

        return;
    }


    if (instance_handle.generation != this->slots[instance_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation instance\n" << std::endl;

        return;
    }


    if (this->slots[instance_handle.index].instance == nullptr)
    {
        std::cout << "\nInstance does not exist already\n" << std::endl;

        return;
    }


    if (this->slots[instance_handle.index].subscribers_count == 0)
    {
        std::cout << "\nInstance subscribers counter underflow\n" << std::endl;

        return;
    }


    this->slots[instance_handle.index].subscribers_count -= 1;
}

void Instance_manager::sub(handle_ctx instance_handle)
{
    if (instance_handle.index < 0 ||
        instance_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed instance\n" << std::endl;
        return;
    }

    instance_slot_ctx& slot = this->slots[instance_handle.index];

    if (instance_handle.generation != slot.handle.generation)
    {
        std::cout << "\nRequest to old generation instance\n" << std::endl;
        return;
    }

    if (slot.instance == nullptr)
    {
        std::cout << "\nInstance does not exist already\n" << std::endl;
        return;
    }

    slot.subscribers_count += 1;
}

bool Instance_manager::is_instance_alive(handle_ctx instance_handle) const
{
    return get_instance(instance_handle) != nullptr;
}

int Instance_manager::get_instance_generation(int index) const
{
    if (index < 0 || index >= static_cast<int>(this->slots.size()))
    {
        return 0;
    }

    return this->slots[index].handle.generation;
}


Instance* Instance_manager::get_instance(handle_ctx instance_handle)
{
    if (
        
        instance_handle.index < 0 ||

        instance_handle.index >= static_cast<int>(this->slots.size())
    )
    {
        std::cout << "\nRequest to non existed instance\n" << std::endl;

        return nullptr;
    }

    if (instance_handle.generation != this->slots[instance_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation asset\n" << std::endl;

        return nullptr;
    }


    return this->slots[instance_handle.index].instance;
}


const Instance* Instance_manager::get_instance(handle_ctx instance_handle) const
{
    if (instance_handle.index < 0 || instance_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed instance\n" << std::endl;
        return nullptr;
    }

    if (instance_handle.generation != this->slots[instance_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation asset\n" << std::endl;
        return nullptr;
    }

    return this->slots[instance_handle.index].instance;
}


Image_instance* Instance_manager::get_image_instance(handle_ctx instance_handle)
{

    Instance* instance = this->get_instance(instance_handle); 
    
    if (!instance) {
        return nullptr;
    }

    return static_cast<Image_instance*>(instance);
}


void Instance_manager::delete_instance(Instance* instance)
{
    if (instance != nullptr)
    {
        this->asset_manager->unsub_operation(instance->asset_handle);
    }

    delete instance;
}


const handle_ctx Instance_manager::get_free_handle()
{   
    bool handle_found = false;
    handle_ctx handle_for_return;

    // First Instance case
    if (this->slots.empty())
    {
        this->slots.push_back({});

        this->slots[0].instance = nullptr;

        this->slots[0].handle.index = 0;

        // Need to switch gen at slot init
        this->slots[0].handle.generation = 1;

        this->slots[0].subscribers_count = 0;

        handle_found = true;
        handle_for_return = this->slots[0].handle;
    }
    else
    {
        for (size_t i = 0; i < this->slots.size(); i++)
        {
            instance_slot_ctx* slot = &this->slots[i];

            if (slot->instance == nullptr)
            {
                // No need to switch slot gen here
                // it will be reset at the instance delete stage

                slot->subscribers_count = 0;


                handle_found = true;
                handle_for_return = slot->handle;
            }

            if (handle_found) break;
        }

        // All existed slots reserved
        if (!handle_found)
        {
            int new_idx = this->slots.size();

            this->slots.push_back({});

            this->slots[new_idx].instance = nullptr;

            this->slots[new_idx].handle.index = new_idx;

            // Need to switch gen at slot init
            this->slots[new_idx].handle.generation = 1;

            this->slots[new_idx].subscribers_count = 0;


            handle_found = true;
            handle_for_return = this->slots[new_idx].handle;
        }
    }


    if(handle_found) return handle_for_return;


    else
    {
        std::cout << "\nERROR DURING HANDLE PROVIDE TO THE NEW INSTANCE\n";
        return {};
    }
}


// ===== METHODS =====



// =========================================================================================== INSTANCE MANAGER CLASS