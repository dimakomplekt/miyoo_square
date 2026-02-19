// asset.cpp


// =========================================================================================== IMPORT

#include <iostream>

#include "new_asset_instance.h"     // Automatically #include "new_asset.h"
#include <algorithm>                // For std::remove


#include <cassert>                  // assert include

// =========================================================================================== IMPORT


// =========================================================================================== ASSET CLASS

// Constructor realization 

Asset::Asset(Asset_type type, const std::string& path) : type(type), source_path(path) {};


// Destructor realization 

Asset::~Asset() = default;


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

// =========================================================================================== ASSET CLASS

// Image asset constructor

Image_asset::Image_asset(const std::string& path) :

    // Default values
    Asset(Asset_type::IMAGE, path), 

{
    this->set_format();
    this->set_sizes();
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Image_asset::~Image_asset()
{

    // Iteratively clears up the image asset instances list and calls
    // instance destructor for every one of them 

    while (!this->instances.empty())
    {
        Image_instance* instance = *instances.begin();

        this->delete_instance(instance);
    }
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
    Image_instance* instance = new Image_instance(this);

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

    // Default values for basic class
    Asset(Asset_type::AUDIO, path)

{
    this->set_sample_rate();
    this->set_bitrate();
    this->set_length();
    this->set_channel_mode();
    this->set_audio_format();
}


// Audip asset destructor - free the memory and null the pointers for the asset and all asset instances

Audio_asset::~Audio_asset()
{
    // Iteratively clears up the audio asset instances list and calls
    // instance destructor for every one of them 

    while (!this->instances.empty())
    {
        Audio_instance* instance = *instances.begin();

        this->delete_instance(instance);
    }
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
    return this->initial_channel_mode;
}


// Initial format getter

audio_format Audio_asset::get_audio_format()
{
    return Audio_asset::initial_format;
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
