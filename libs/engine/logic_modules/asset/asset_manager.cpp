// asset_manager.cpp

// =========================================================================================== IMPORT

#include "asset_manager.h"

// =========================================================================================== IMPORT


// =========================================================================================== NOTE

/*
                  Asset_manager
                       │
                       ▼
              ┌─────────────────┐
              │  asset_slot_ctx  │
              ├─────────────────┤
              │ Asset* asset    │
              │ handle           │
              │ instance_count  │
              └─────────────────┘
                       │
                 generation
                       │
          ┌────────────┴────────────┐
          ▼                         ▼
    handle {index, gen}       asset == nullptr
          │                         │
          │                    slot reusable
          ▼
   validate handle
          │
    index + generation
          │
          ▼
       Asset*

*/

// =========================================================================================== NOTE



// =========================================================================================== ASSET MANAGER CLASS



// ===== Lifetime ===== 

Asset_manager::Asset_manager() {};


// Manager destructor
// Calles once per app cycle 
void Asset_manager::asset_manager_delete()
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
            asset_slot_ctx* slot = &this->slots[i];

            if (slot->asset != nullptr)
            {
                slots_list_free = false;

                break;
            }
        }
    }

    // Stop delete operation
    if (!slots_list_free) return;

    // The caller owns the manager object and performs the final delete.
    if (slots_list_free)
    {
        std::cout << "Asset_manager is ready for deletion\n";
    }
}

Asset_manager::~Asset_manager() {};

// ===== Lifetime =====


// ===== METHODS =====


const handle_ctx Asset_manager::add_asset(asset_type type, std::string asset_link)
{
    if (type < IMAGE_AT || type >= UNKNOWN_AT)
    {
        std::cout << "\nWrong asset type pass!\n" << std::endl;

        return {};
    }

    if (asset_link.empty())
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
            Image_asset* new_asset = new Image_asset(asset_link, this);
            
            // Image not passed handler
            if (new_asset->provide_surface() == nullptr)
            {
                std::cout << "\nFailed to load image asset!\n" << std::endl;

                delete new_asset;

                return {};
            }
            
            this->slots[handle_to_use.index].asset = new_asset;

            break;
        }

        case AUDIO_AT:
        {
            std::cout << "\nAudio assets are not implemented yet!\n" << std::endl;
            return {};
        }


        case VIDEO_AT:
        {
            std::cout << "\nVideo assets are not implemented yet!\n" << std::endl;
            return {};
        }


        case FONT_AT:
        {
            std::cout << "\nFont assets are not implemented yet!\n" << std::endl;
            return {};
        }


        default:
            return {};
    }


    return handle_to_use;

}


bool Asset_manager::delete_asset_request(handle_ctx asset_handle)
{

    if (asset_handle.index < 0 ||
        asset_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed asset\n" << std::endl;

        return false;
    }

    if (asset_handle.generation != this->slots[asset_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation asset\n" << std::endl;

        return false;
    }


    if (this->slots[asset_handle.index].asset == nullptr)
    {
        std::cout << "\nAsset does not exist already\n" << std::endl;

        return false;
    }


    if (this->slots[asset_handle.index].instance_count == 0)
    {
        // Delete
        this->delete_asset(this->slots[asset_handle.index].asset);

        // Nullptr
        this->slots[asset_handle.index].asset = nullptr;


        // Switch the generation number to
        // block the operations with unsubscribed old
        // subs (error case 2nd stage preventing)
        this->slots[asset_handle.index].handle.generation += 1;


        return true;
    }


    return false;
}

bool Asset_manager::is_asset_alive(handle_ctx asset_handle) const
{
    return get_asset(asset_handle) != nullptr;
}

int Asset_manager::get_asset_generation(int index) const
{
    if (index < 0 || index >= static_cast<int>(this->slots.size()))
    {
        return 0;
    }

    return this->slots[index].handle.generation;
}


void Asset_manager::unsub_operation(handle_ctx asset_handle)
{

    if (asset_handle.index < 0 ||
        asset_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed asset\n" << std::endl;

        return;
    }


    if (asset_handle.generation != this->slots[asset_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation asset\n" << std::endl;

        return;
    }


    if (this->slots[asset_handle.index].instance_count == 0)
    {
        std::cout << "\nAsset instance counter underflow\n" << std::endl;

        return;
    }

    this->slots[asset_handle.index].instance_count -= 1;
}



asset_slot_ctx* Asset_manager::get_asset_slot(handle_ctx asset_handle)
{
    if (asset_handle.index < 0 ||
        asset_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed asset\n" << std::endl;

        return nullptr;
    }


    if (asset_handle.generation != this->slots[asset_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation asset\n" << std::endl;

        return nullptr;
    }


    return &this->slots[asset_handle.index];
}


const Asset* Asset_manager::get_asset(handle_ctx asset_handle) const
{
    if (asset_handle.index < 0 ||
        asset_handle.index >= static_cast<int>(this->slots.size()))
    {
        std::cout << "\nRequest to non existed asset\n" << std::endl;

        return nullptr;
    }


    if (asset_handle.generation != this->slots[asset_handle.index].handle.generation)
    {
        std::cout << "\nRequest to old generation asset\n" << std::endl;

        return nullptr;
    }


    return this->slots[asset_handle.index].asset;
}



void Asset_manager::delete_asset(Asset* asset)
{
    delete asset;
}


const handle_ctx Asset_manager::get_free_handle()
{
    bool handle_found = false;
    handle_ctx handle_for_return;

    // First asset case
    if (this->slots.empty())
    {
        this->slots.push_back({});

        this->slots[0].asset = nullptr;

        this->slots[0].handle.index = 0;

        // Need to switch gen at slot init
        this->slots[0].handle.generation = 1;

        this->slots[0].instance_count = 0;

        handle_found = true;
        handle_for_return = this->slots[0].handle;
    }
    else
    {
        for (size_t i = 0; i < this->slots.size(); i++)
        {
            asset_slot_ctx* slot = &this->slots[i];

            if (slot->asset == nullptr)
            {
                // No need to switch slot gen here
                // it will be reset at the asset delete stage

                slot->instance_count = 0;


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

            this->slots[new_idx].asset = nullptr;

            this->slots[new_idx].handle.index = new_idx;

            // Need to switch gen at slot init
            this->slots[new_idx].handle.generation = 1;

            this->slots[new_idx].instance_count = 0;


            handle_found = true;
            handle_for_return = this->slots[new_idx].handle;
        }
    }


    if(handle_found) return handle_for_return;


    else
    {
        std::cout << "\nERROR DURING HANDLE PROVIDE TO THE NEW ASSET\n";
        return {};
    }
}


// ===== METHODS =====


// =========================================================================================== ASSET MANAGER CLASS
