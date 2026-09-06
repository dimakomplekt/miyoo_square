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

const asset_type Asset::get_type() const
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

    this->type = IMAGE_AT; 
    this->source_path = path;          


    this->image_load();
}


// Image asset destructor - free the memory and null the pointers for the asset and all asset instances

Image_asset::~Image_asset()
{

    if (this->surface != nullptr)
    {
        SDL_FreeSurface(this->surface);
        this->surface = nullptr;
    }

}

// ===== LIFETIME =====


// ===== METHODS =====

const SDL_Surface* Image_asset::provide_surface() const
{
    if (this->surface != nullptr) return this->surface;

    else std::cout << "Empty surface translation by asset.";
}

// ===== METHODS =====
        

// ===== METHODS =====


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


// Set initial width of the image
void Image_asset::image_load()
{
    // Load by SDL_Image
    this->surface = IMG_Load(this->source_path.c_str());


    if (this->surface == nullptr)
    {
        this->initial_width = 0;
        this->initial_height = 0;

        return;
    }

    this->initial_width = static_cast<unsigned int>(this->surface->w);

    this->initial_height = static_cast<unsigned int>(this->surface->h);
}

// ===== METHODS =====

// =========================================================================================== IMAGE ASSET CLASS