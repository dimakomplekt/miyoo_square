// asset.cpp


// =========================================================================================== IMPORT

#include <iostream>

#include "asset_instance.h"         // Automatically #include "asset.h"
#include <algorithm>                // For std::remove

// =========================================================================================== IMPORT


// =========================================================================================== ASSET CLASS

// Constructor realization 

Asset::Asset(Asset_type type, const std::string& path) : type(type), source_path(path) {};


// Destructor - deletes class data and all active instances

Asset::~Asset()
{
    while (!instances.empty())
    {
        // Destroy all instances safely
        auto* instance = *instances.begin();
        delete_instance(instance);
    }

    // Destroy the other data automatically
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

Asset_instance* Asset::add_instance()
{
    // Create an instance by the friendly class constructor
    auto* instance = new Asset_instance(this);

    // Insert the instance inside the instances list
    instances.insert(instance);


    return instance; // Return and adress for the class-object (variable) to use
}

void Asset::delete_instance(Asset_instance* instance)
{
    if (!instance) return;

    instances.erase(instance);

    delete instance;
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

const std::vector<unsigned int>& Audio_asset::get_length() const
{
    return initial_audio_length;
}


// =========================================================================================== AUDIO ASSET CLASS