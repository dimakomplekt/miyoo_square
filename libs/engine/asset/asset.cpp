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
        // This logic choosen for the precise control of the destructors selection,
        // by the reason, that Asset_instance could have some exclusive destructors logic
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
    this->set_format();
    this->set_sizes();
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Image_asset::~Image_asset()
{
    // TODO: 
    // 1) Universal instances free logic by Asset class
    // 2) Universal asset free logic by Asset class
    // 3) Universal player free logic by Asset_instance class
}



image_format Image_asset::get_format() const
{
    return this->initial_format;
}

// Initial width getter

unsigned int Image_asset::get_width() const
{
    return this->initial_width;
}


// Initial height getter

unsigned int Image_asset::get_height() const
{
    return this->initial_height;
}


// Set initial image format
void Image_asset::set_format()
{
    // TODO: FILE PARSER
}


// Set initial width of the image
void Image_asset::set_sizes()
{
    // TODO: FILE PARSER
}



// Instance create and delete methods


Image_instance* Image_asset::add_instance()
{
    // Create an instance by the friendly class constructor
    auto* instance = new Image_instance(this);

    // Insert the instance inside the instances list
    register_instance(instance);

    return instance; // Return the link of the class-object (variable) to use
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
    this->set_sample_rate();
    this->set_bitrate();
    this->set_length();
    this->set_channel_mode();
    this->set_audio_format();
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Audio_asset::~Audio_asset()
{
    // TODO: 
    // 1) Universal instances free logic by Asset class
    // 2) Universal asset free logic by Asset class
    // 3) Universal player free logic by Asset_instance class
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


// Initial channel mode getter

channel_mode Audio_asset::get_channel_mode()
{
    // TODO: FILE PARSER
}


// Initial format getter

audio_format Audio_asset::get_audio_format()
{
    // TODO: FILE PARSER
}


// Initial sample rate file parser

void Audio_asset::set_sample_rate()
{
    // TODO: FILE PARSER
}


// Initial bitrate file parser

void Audio_asset::set_bitrate()
{
    // TODO: FILE PARSER
}


// Initial audio length file parser

void Audio_asset::set_length()
{
    // TODO: FILE PARSER
}

// Channel mode initial file parser

void Audio_asset::set_channel_mode()
{
    // TODO: FILE PARSER
}

// Format initial file parser

void Audio_asset::set_audio_format()
{
    // TODO: FILE PARSER
}


// Instance create and delete methods

Audio_instance* Audio_asset::add_instance()
{
    // Create an instance by the friendly class constructor
    auto* instance = new Audio_instance(this);

    // Insert the instance inside the instances list
    register_instance(instance);

    return instance; // Return the link of the class-object (variable) to use
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