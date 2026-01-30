// asset_instance.cpp


// =========================================================================================== IMPORT

#include "asset_instance.h"

#include <cassert> // assert include

// =========================================================================================== IMPORT



// =========================================================================================== ASSET INSTANCE CLASS


// Basic constructor which initializes the main_asset link (called by main asset methods)

Asset_instance::Asset_instance(Asset* asset) : main_asset(asset)
{
    // Error handler
    assert(asset != nullptr);
};


// Basic destructor 
Asset_instance::~Asset_instance() = default;


const Asset* Asset_instance::get_main_asset_link() const
{
    return main_asset;
}


// =========================================================================================== ASSET INSTANCE CLASS


// =========================================================================================== IMAGE INSTANCE CLASS


Image_instance::Image_instance(Image_asset* asset) : Asset_instance(asset)
{
    // Error handler
    assert(asset != nullptr);
};


Image_instance::~Image_instance() {};


const Image_asset* Image_instance::get_main_asset_link() const
{
    return static_cast<const Image_asset*>(Asset_instance::get_main_asset_link());
}


// =========================================================================================== IMAGE INSTANCE CLASS



// =========================================================================================== AUDIO INSTANCE CLASS


Audio_instance::Audio_instance(Audio_asset* asset) : Asset_instance(asset)
{
    // Error handler
    assert(asset != nullptr);
};


Audio_instance::~Audio_instance() {}


const Audio_asset* Audio_instance::get_main_asset_link() const
{
    return static_cast<const Audio_asset*>(Asset_instance::get_main_asset_link());
}


// =========================================================================================== AUDIO INSTANCE CLASS
