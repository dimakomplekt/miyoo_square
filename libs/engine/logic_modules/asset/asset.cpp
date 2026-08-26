// asset.cpp


// =========================================================================================== IMPORT

#include <iostream>

#include "asset.h"

// =========================================================================================== IMPORT


// =========================================================================================== ASSET CLASS

// ===== LIFETIME =====

// Constructor realization 

Asset::Asset() = default;


// Destructor realization 

Asset::~Asset() = default;

// ===== LIFETIME =====


// ===== METHODS =====

// Asset type getter

const Asset_type Asset::get_type() const
{
    // Returns the current asset type
    return this->type;
}


// Asset path getter

const std::string& Asset::get_path() const 
{
    // Returns the asset path
    return this->source_path;
}

// ===== METHODS =====

// =========================================================================================== ASSET CLASS


// =========================================================================================== IMAGE ASSET CLASS

// ===== LIFETIME =====

// Image asset constructor

Image_asset::Image_asset(const std::string& path) {

    this->type = Asset_type::IMAGE_AT; 
    this->source_path = path;          

    this->set_format();
    this->set_sizes();
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Image_asset::~Image_asset() = default;


// ===== LIFETIME =====


// ===== METHODS =====

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

// ===== METHODS =====

// =========================================================================================== IMAGE ASSET CLASS