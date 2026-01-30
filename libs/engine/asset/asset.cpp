// asset.cpp


// =========================================================================================== IMPORT

#include <iostream>

#include "asset_instance.h"         // Automatically #include "asset.h"
#include <algorithm>                // For std::remove


#include <cassert>                  // assert include

// =========================================================================================== IMPORT


// =========================================================================================== ASSET CLASS

// Constructor realization 

Asset::Asset(Asset_type type, const std::string& path) : type(type), source_path(path) {};


// Destructor - deletes class data and all active instances

Asset::~Asset()
{
    while (!instances.empty())
    {
        auto* instance = *instances.begin();

        // Switch the logic for different asset types
        // This logic choosed for the precise control of the destructors selection,
        // by the reason, that Asset_instance could have some exlusive destructors logic
        switch (type)
        {
            case Asset_type::IMAGE:

                static_cast<Image_asset*>(this)->delete_instance(static_cast<Image_instance*>(instance));
                break;

            case Asset_type::AUDIO:

                static_cast<Audio_asset*>(this)->delete_instance(static_cast<Audio_instance*>(instance));
                break;

            default:

                delete instance;
                break;
        }
    }

    assert(instances.empty());
}


void Asset::use() const {};             // Empty basic use


// Asset type getter

Asset_type Asset::get_type() const
{
    // Returns the current asset type
    return type;
}


// Asset path getter

const std::string& Asset::get_path() const 
{
    // Returns the asset path
    return source_path;
}


// Instances workflow by the unordered_set methods


void Asset::register_instance(Asset_instance* instance)
{
    instances.insert(instance);
}

void Asset::unregister_instance(Asset_instance* instance)
{
    instances.erase(instance);
}


// Instances workflow by the unordered_set methods


// =========================================================================================== ASSET CLASS


// =========================================================================================== IMAGE ASSET CLASS

// Image asset constructor

Image_asset::Image_asset(const std::string& path) :

    // Default values
    Asset(Asset_type::IMAGE, path), 

    initial_width(0), 
    initial_height(0)

{
    // TODO:
    // 1. LOAD IMAGE
    // 2. GET width / height
    // 3. SAVE PARAMETERS IN initial_width and initial_height
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Image_asset::~Image_asset()
{
    // TODO: 
    // 1) Instances free logic
    // 2) Asset free logic
}


// Initial width getter

unsigned int Image_asset::get_width() const
{
    return initial_width;
}


// Initial height getter

unsigned int Image_asset::get_height() const
{
    return initial_height;
}


// Instance create and delete methods


Image_instance* Image_asset::add_instance()
{
    // Create an instance by the friendly class constructor
    auto* instance = new Image_instance(this);

    // Insert the instance inside the instances list
    register_instance(instance);

    return instance; // Return and adress for the class-object (variable) to use
}

void Image_asset::delete_instance(Image_instance* instance)
{
    // Error handler
    if (!instance) return;

    assert(instance->get_main_asset_link() == this);

    // Remove the instance from the instances list
    unregister_instance(instance);

    // Delete from the memory - CALL ONLY BY delete_instance()
    delete instance;
}


// =========================================================================================== IMAGE ASSET CLASS


// =========================================================================================== AUDIO ASSET CLASS

// Audio asset constructor 

Audio_asset::Audio_asset(const std::string& path) :

    // Default values
    Asset(Asset_type::AUDIO, path),

    initial_sample_rate(0),
    initial_bitrate(0),

    initial_audio_length{0, 0, 0, 0}

{
    // TODO:
    // 1. LOAD AUDIO
    // 2. GET the sample rate / bitrate
    // 3. Count the length (h, m, s, ms)
    // 4. Setup the parameters
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Audio_asset::~Audio_asset()
{
    // TODO: 
    // 0) Stop playing command
    // 1) Instances free logic
    // 2) Asset free logic
}


// Initial sample rate getter

unsigned int Audio_asset::get_sample_rate() const
{
    return initial_sample_rate;
}


// Initial bitrate getter

unsigned int Audio_asset::get_bitrate() const
{
    return initial_bitrate;
}


// Initial length getter

const timecode& Audio_asset::get_length() const
{
    return initial_audio_length;
}


// Instance create and delete methods


Audio_instance* Audio_asset::add_instance()
{
    // Create an instance by the friendly class constructor
    auto* instance = new Audio_instance(this);

    // Insert the instance inside the instances list
    register_instance(instance);

    return instance; // Return and adress for the class-object (variable) to use
}

void Audio_asset::delete_instance(Audio_instance* instance)
{
    // Error handler
    if (!instance) return;

    // Remove the instance from the instances list
    unregister_instance(instance);

    // Delete from the memory
    delete instance;
}

// =========================================================================================== AUDIO ASSET CLASS