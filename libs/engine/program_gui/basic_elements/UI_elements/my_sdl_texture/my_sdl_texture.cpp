// my_sdl_texture.cpp

// =========================================================================================== IMPORT

#include "my_sdl_texture.h"

// Onetime CPP include for remove_element() method providing
#include "../my_sdl_panel/my_sdl_panel.h"

// =========================================================================================== IMPORT


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR

My_SDL_texture::My_SDL_texture()
{
    this->texture = nullptr;

    this->width_size = 0;
    this->height_size = 0;

    this->basic_width_size = 0;  
    this->basic_height_size = 0;


    this->x_render_point = 0;
    this->y_render_point = 0;

    this->owns_texture = true;

    this->reset_anchor_points();
}


My_SDL_texture::~My_SDL_texture()
{
    // Texture destructor
    if (this->texture != nullptr)
    {
        if (this->owns_texture) SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}


void My_SDL_texture::delete_element()
{
    My_SDL_panel* container = this->get_element_container();

    // Delete itself by upper level panel or by itself
    if (container)
    {
        container->remove_element(this);
    }
    else
    {
        delete this;
    }
}

// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR



// =========================================================================================== MAIN LOGIC

void My_SDL_texture::update()
{
    // No actions for not visiable element
    if (!this->visible_flag) return;


    // Movement logic if the movement is on
    this->movement_logic_in_update_loop();

}


void My_SDL_texture::set_texture(SDL_Texture* new_texture, bool take_ownership)
{
    // Texture link repeat - just reset size to basic and return
    if (this->texture == new_texture)
    {
        this->width_size = this->basic_width_size;
        this->height_size = this->basic_height_size;

        return;
    } 


    // New texture link case

    // Delete old before set
    if (this->texture != nullptr)
    {
        if (this->owns_texture) SDL_DestroyTexture(this->texture);

        this->texture = nullptr;
    }


    this->texture = new_texture;
    this->owns_texture = take_ownership;


    // Error handler for nullptr pass (could be possible on different workflows)
    if (this->texture == nullptr)
    {
        this->width_size = 0;
        this->height_size = 0;
        this->reset_anchor_points();

        return;
    }


    // Sizes recalculation for not empty (nullptr) texture

    float w = 0.0f;
    float h = 0.0f;

    SDL_GetTextureSize(this->texture, &w, &h);


    // Reset basic sizes
    this->basic_width_size = static_cast<int>(std::round(w));
    this->basic_height_size = static_cast<int>(std::round(h));

    // Set the new current sizes by basic sizes on texture pass
    this->width_size = this->basic_width_size;
    this->height_size = this->basic_height_size;

    
    this->reset_anchor_points();
}


void My_SDL_texture::set_texture_by_image(const std::string& link, SDL_Renderer* renderer)
{
    if (!renderer)
    {
        SDL_Log("Renderer is null");
        this->set_texture(nullptr, false);
        return;
    }

    

    SDL_Texture* new_texture = IMG_LoadTexture(renderer, link.c_str());

    if (!new_texture)
    {
        SDL_Log("IMG_LoadTexture failed: %s", SDL_GetError());
        this->set_texture(nullptr, false);
        return;
    }

    // Images textures controlled by object himself
    this->set_texture(new_texture, true);
}


// =========================================================================================== MAIN LOGIC



// =========================================================================================== GUI

void My_SDL_texture::render(SDL_Renderer* renderer)
{
    if (this->texture == nullptr) return;

    // No actions for not visiable element
    if (!this->visible_flag) return;

    SDL_FRect dst;

    dst.w = static_cast<float>(this->width_size);
    dst.h = static_cast<float>(this->height_size);

    dst.x = static_cast<float>(this->x_render_point) - dst.w / 2.0f;
    dst.y = static_cast<float>(this->y_render_point) - dst.h / 2.0f;


    SDL_SetTextureAlphaMod(this->texture, this->opacity);

    SDL_RenderTexture(renderer, this->texture, nullptr, &dst);
}



void My_SDL_texture::set_render_point(int x_cc_rp, int y_cc_rp)
{
    this->x_render_point = x_cc_rp;
    this->y_render_point = y_cc_rp;


    this->reset_anchor_points();
}


int My_SDL_texture::get_basic_width_size() const
{
    return this->basic_width_size;
}


int My_SDL_texture::get_basic_height_size() const
{
    return this->basic_height_size;
}



int My_SDL_texture::get_width_size() const
{
    return this->width_size;
}


int My_SDL_texture::get_height_size() const
{
    return this->height_size;
}


void My_SDL_texture::set_x_scaler(float new_x_scaler)
{
    if (new_x_scaler <= 0.0f) return;

    this->x_scaler = new_x_scaler;

    this->width_size = static_cast<int>(std::round(this->basic_width_size * this->x_scaler));

    this->reset_anchor_points();
}


void My_SDL_texture::set_y_scaler(float new_y_scaler)
{
    if (new_y_scaler <= 0.0f) return;

    this->y_scaler = new_y_scaler;

    this->height_size = static_cast<int>(std::round(this->basic_height_size * this->y_scaler));

    this->reset_anchor_points();
}


void My_SDL_texture::set_scalers(float new_x_scaler, float new_y_scaler)
{
    if (new_x_scaler <= 0.0f || new_y_scaler <= 0.0f) return;

    this->x_scaler = new_x_scaler;
    this->y_scaler = new_y_scaler;

    this->width_size = static_cast<int>(std::round(this->basic_width_size * this->x_scaler));
    this->height_size = static_cast<int>(std::round(this->basic_height_size * this->y_scaler));

    this->reset_anchor_points();
}



float My_SDL_texture::get_x_scaler() const
{
    return this->x_scaler;
}

float My_SDL_texture::get_y_scaler() const
{
    return this->y_scaler;
}


void My_SDL_texture::set_width(int new_width)
{
    if (new_width <= 0) return;

    this->width_size = new_width;

    this->x_scaler = static_cast<float>(this->width_size) / static_cast<float>(this->basic_width_size);

    this->reset_anchor_points();
}


void My_SDL_texture::set_height(int new_height)
{
    if (new_height <= 0) return;

    this->height_size = new_height;

    this->y_scaler = static_cast<float>(this->height_size) / static_cast<float>(this->basic_height_size);

    this->reset_anchor_points();
}


void My_SDL_texture::set_size(unsigned int new_width, unsigned int new_height)
{
    if (new_width <= 0 || new_height <= 0) return;

    this->width_size = new_width;
    this->height_size = new_height;

    this->x_scaler = static_cast<float>(this->width_size) / static_cast<float>(this->basic_width_size);
    this->y_scaler = static_cast<float>(this->height_size) / static_cast<float>(this->basic_height_size);

    this->reset_anchor_points();
}



void My_SDL_texture::reset_colors_if_palette_switched()
{

}




void My_SDL_texture::reset_size()
{
    this->width_size = this->basic_width_size;
    this->height_size = this->basic_height_size;

    this->x_scaler = 1.0f;
    this->y_scaler = 1.0f;

    this->reset_anchor_points();
}


void My_SDL_texture::reset_anchor_points()
{
    int half_w = static_cast<int>(std::round(this->width_size * 0.5f));
    int half_h = static_cast<int>(std::round(this->height_size * 0.5f));

    int c_w = this->x_render_point;
    int c_h = this->y_render_point;

    this->element_anchor_points.top_left         = { c_w - half_w, c_h - half_h };
    this->element_anchor_points.top_center       = { c_w,          c_h - half_h };
    this->element_anchor_points.top_right        = { c_w + half_w, c_h - half_h };

    this->element_anchor_points.center_left      = { c_w - half_w, c_h };
    this->element_anchor_points.center_center    = { c_w,          c_h };
    this->element_anchor_points.center_right     = { c_w + half_w, c_h };

    this->element_anchor_points.bottom_left      = { c_w - half_w, c_h + half_h };
    this->element_anchor_points.bottom_center    = { c_w,          c_h + half_h };
    this->element_anchor_points.bottom_right     = { c_w + half_w, c_h + half_h };
}

// =========================================================================================== GUI