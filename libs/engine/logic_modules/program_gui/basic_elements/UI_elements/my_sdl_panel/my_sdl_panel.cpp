// my_sdl_panel.cpp


// =========================================================================================== IMPORT

#include "my_sdl_panel.h"
#include <algorithm>                                     // For std::remove_if

// =========================================================================================== IMPORT


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR

My_SDL_panel::My_SDL_panel()
{
    // Basic settings setter
    this->panel_width_size = 300;
    this->panel_height_size = 150;

    this->x_render_point = panel_width_size / 2 + 300;
    this->y_render_point = panel_height_size / 2 + 300;

    this->border_width_size = 5;
    this->border_radius_size = 30;


    this->current_form = RECTANGLE_EF;

    this->shadow_offset_x = 10;
    this->shadow_offset_y = 10;
    
    this->shadow_scale_factor = 1.0f;


    // Basic color pass by the current palette

    this->background_color = App_palette.get_current_palette().inverted_basic_background_color;
    this->border_color = App_palette.get_current_palette().inverted_basic_border_color;
    this->shadow_color = App_palette.get_current_palette().inverted_basic_shadow_color;
    
    this->render_background_color = this->background_color;
    this->render_border_color = this->border_color;
    this->render_shadow_color = this->shadow_color;


    this->inner_elements.clear();

    this->reset_anchor_points();

    this->set_opacity(255);

    this->render_data_reset();
}


void My_SDL_panel::delete_element()
{
    // Delete all inner elements
    this->clear_elements();


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

void My_SDL_panel::update()
{
    // No actions for not visiable element
    if (!this->visible_flag) return;

    // Movement logic if the movement is on
    this->movement_logic_in_update_loop();

    // Check if the palette was switched and update the colors by the new palette if it was
    this->reset_colors_if_palette_switched();

    
    // Just update all inner elements by auto type link and overrided update() methods
    for (auto& inner : inner_elements)
    {
        inner.element_pointer->update();
    }
}


void My_SDL_panel::set_opacity(Uint8 new_opacity)
{
    this->basic_opacity = new_opacity;
    
    // In case where we got the container
    if (this->element_container != nullptr)
    {
        this->recalculate_opacity_by_container();
    }
    else
    {
        // Equal opacity in case without container
        this->opacity = new_opacity;
    }

    // Change colors by the new opacity for the panel


    // Change the opacity for all inner elements by the new container opacity
    for (auto& inner : inner_elements)
    {

        inner.element_pointer->set_opacity(inner.element_pointer->get_basic_opacity());
    }


    this->panel_palette_prepare();
}


void My_SDL_panel::set_visible_flag(bool new_flag)
{   
    this->My_SDL_element::set_visible_flag(new_flag);

    
    // Change the opacity for all inner elements by the new container opacity
    for (auto& inner : inner_elements)
    {
        inner.element_pointer->set_visible_flag(new_flag);
    }
}



void My_SDL_panel::panel_palette_prepare()
{
    // GLOBAL element opacity implementation
    float opacity_scaler = static_cast<float>(this->opacity) / 255.0f;

    this->render_shadow_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_shadow_color.a) * opacity_scaler));
    this->render_border_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_border_color.a) * opacity_scaler));
    this->render_background_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_background_color.a) * opacity_scaler));
}

// =========================================================================================== MAIN LOGIC


// =========================================================================================== GUI

void My_SDL_panel::render(SDL_Renderer* renderer)
{
    // No actions for not visiable element
    if (!this->visible_flag) return;

    // Render 3 figures (shadow (border sizes * scaler), border and background (width or hight - 2 * border_width)) 
    // by their sizes, with use of current colors and render point (center-center)

    panel_render_data dataset = this->current_render_data;

    if (this->current_form == RECTANGLE_EF)
    {
        // SHADOW
        rectangle_draw_by_color(dataset.sw_cx, dataset.sw_cy, dataset.sw_w, dataset.sw_h, this->render_shadow_color, renderer);

        // BORDER
        rectangle_draw_by_color(dataset.br_cx, dataset.br_cy, dataset.br_w, dataset.br_h, this->render_border_color, renderer);

        // BACKGROUND
        rectangle_draw_by_color(dataset.bd_cx, dataset.bd_cy, dataset.bg_w, dataset.bg_h, this->render_background_color, renderer);
    }

    else if (this->current_form == ROUNDED_RECTANGLE_EF)
    {
        // SHADOW
        rounded_rectangle_draw_by_color(dataset.sw_cx, dataset.sw_cy, dataset.sw_w, dataset.sw_h, dataset.sw_r, this->render_shadow_color, renderer);

        // BORDER
        rounded_rectangle_draw_by_color(dataset.br_cx, dataset.br_cy, dataset.br_w, dataset.br_h, dataset.br_r, this->render_border_color, renderer);

        // BACKGROUND
        rounded_rectangle_draw_by_color(dataset.bd_cx, dataset.bd_cy, dataset.bg_w, dataset.bg_h, dataset.bg_r, this->render_background_color, renderer);
    }

    else if (this->current_form == CIRCLE_EF)
    {
        // SHADOW
        circle_draw_by_color(dataset.sw_cx, dataset.sw_cy, dataset.sw_w / 2, this->render_shadow_color, renderer);

        // BORDER
        circle_draw_by_color(dataset.br_cx, dataset.br_cy, dataset.br_w / 2, this->render_border_color, renderer);

        // BACKGROUND
        circle_draw_by_color(dataset.bd_cx, dataset.bd_cy, dataset.bg_w / 2, this->render_background_color, renderer);
    }


    // Render the elements by the z-order (just go through the vector it's already sorted)
    
    for (auto it = this->inner_elements.begin(); it != this->inner_elements.end(); ++it)
    {
        if (!it->element_pointer->visible_flag) continue;
        
        it->element_pointer->render(renderer);
    }
}


void My_SDL_panel::set_render_point(int x_cc_rp, int y_cc_rp)
{
    // Set panel position
    this->x_render_point = x_cc_rp;
    this->y_render_point = y_cc_rp;

    // Reset anchor points
    this->reset_anchor_points();

    this->render_data_reset();

    // Update inner elements positions (though they will be set again in render, this ensures consistency)
    this->change_inner_elements_global_coordinates();
}


void My_SDL_panel::movement_logic_in_update_loop()
{
    // Basic movement
    My_SDL_element::movement_logic_in_update_loop();

    reset_anchor_points();
    render_data_reset();

    change_inner_elements_global_coordinates(); // ONLY update mapping
}


// Size setters and getters

void My_SDL_panel::set_size(unsigned int new_width, unsigned int new_height)
{
    this->panel_width_size = new_width;
    this->panel_height_size = new_height;

    // New form check
    this->reset_current_form();

    // Reset anchor points
    this->reset_anchor_points();

    this->render_data_reset();

    this->change_inner_elements_global_coordinates();

}


unsigned int My_SDL_panel::get_width_size() const { return this->panel_width_size; }

unsigned int My_SDL_panel::get_height_size() const { return this->panel_height_size; }


// Styling setters

void My_SDL_panel::set_border_width_size(unsigned int new_size)
{
    if ((new_size > this->panel_width_size / 2) || 
        (new_size > this->panel_height_size / 2) ||
        (this->border_radius_size != 0 && new_size > (this->border_radius_size - 1)))
    {
        std::cerr << "Wrong border size value pass! Border width size ain't changed" << std::endl;
        return;
    }

    this->border_width_size = new_size;

    this->render_data_reset();
}


void My_SDL_panel::set_border_radius(unsigned int new_size)
{

    if (new_size > this->panel_width_size / 2 || new_size > this->panel_height_size / 2)
    {
        std::cerr << "Wrong radius size value pass! Border radius size ain't changed" << std::endl;
        return;
    }

    this->border_radius_size = new_size;

    // New form check
    this->reset_current_form();

    this->render_data_reset();
}


void My_SDL_panel::set_shadow_offset(int new_x_offset, int new_y_offset)
{
    this->shadow_offset_x = new_x_offset;
    this->shadow_offset_y = -new_y_offset;  // SDL works with inverted y-axis on offset

    this->render_data_reset();
}

void My_SDL_panel::set_shadow_scale_factor(float new_scale_factor)
{
    this->shadow_scale_factor = new_scale_factor;

    this->render_data_reset();
}


// Color setters

void My_SDL_panel::set_panel_background_color(SDL_Color new_color)
{
    this->background_color = new_color;
    this->render_background_color = new_color;
}

void My_SDL_panel::set_panel_border_color(SDL_Color new_color)
{
    this->border_color = new_color;
    this->render_border_color = new_color;
}

void My_SDL_panel::set_panel_shadow_color(SDL_Color new_color)
{
    this->shadow_color = new_color;
    this->render_shadow_color = new_color;
}



void My_SDL_panel::reset_colors_if_palette_switched()
{
    // Pass case

    if (!this->passed_by_palette || !App_palette.get_palette_reset_flag()) return;

    // Renew case

    this->background_color = App_palette.get_current_palette().inverted_basic_background_color;
    this->border_color = App_palette.get_current_palette().inverted_basic_border_color;
    this->shadow_color = App_palette.get_current_palette().inverted_basic_shadow_color;
    
    this->render_background_color = this->background_color;
    this->render_border_color = this->border_color;
    this->render_shadow_color = this->shadow_color;

    
    panel_palette_prepare();
}



void My_SDL_panel::reset_anchor_points()
{
    
    /**
     * 
     * P P P P P
     * 1 2 3 4 5
     * 
     * center is 3 -> (n + 1) / 2
     *
     */


    // Uses current crop to set the current anchor points

    // Always the same rounding accuracy, because we work with crop map in initial scale and new scalers
    int half_w = static_cast<int>(std::round(static_cast<float>(this->panel_width_size) * 0.5));
    int half_h = static_cast<int>(std::round(static_cast<float>(this->panel_height_size) * 0.5));

    // Element_anchor_points reset


    int c_w = this->x_render_point;         // Horizontal center
    int c_h = this->y_render_point;         // Vertical center


    // SDL windows points goes from TL(0; 0) to BR(Max_W, Max_H)
    
    this->element_anchor_points.top_left         =     { c_w - half_w, c_h - half_h };
    this->element_anchor_points.top_center       =     { c_w , c_h - half_h };
    this->element_anchor_points.top_right        =     { c_w + half_w, c_h - half_h };

    this->element_anchor_points.center_left      =     { c_w - half_w, c_h };
    this->element_anchor_points.center_center    =     { c_w, c_h };
    this->element_anchor_points.center_right     =     { c_w + half_w, c_h };

    this->element_anchor_points.bottom_left      =     { c_w - half_w, c_h + half_h  };
    this->element_anchor_points.bottom_center    =     { c_w, c_h + half_h };
    this->element_anchor_points.bottom_right     =     { c_w + half_w, c_h + half_h };

}


void My_SDL_panel::reset_current_form()
{
    float half_w = this->panel_width_size / 2.0f;
    float half_h = this->panel_height_size / 2.0f;

    if (this->border_radius_size >= half_w && this->border_radius_size >= half_h)
        this->current_form = CIRCLE_EF;         

    else if (this->border_radius_size > 0)
        this->current_form = ROUNDED_RECTANGLE_EF; 
         
    else
        this->current_form = RECTANGLE_EF;           
}


void My_SDL_panel::render_data_reset()
{
    this->current_render_data.sw_cx = this->x_render_point + this->shadow_offset_x;
    this->current_render_data.sw_cy = this->y_render_point + this->shadow_offset_y;

    this->current_render_data.br_cx = this->x_render_point;
    this->current_render_data.br_cy = this->y_render_point;

    this->current_render_data.bd_cx = this->x_render_point;
    this->current_render_data.bd_cy = this->y_render_point;


    this->current_render_data.sw_w = static_cast<unsigned int>(std::round((this->panel_width_size) * this->shadow_scale_factor));
    this->current_render_data.sw_h = static_cast<unsigned int>(std::round((this->panel_height_size) * this->shadow_scale_factor));

    this->current_render_data.br_w = this->panel_width_size; 
    this->current_render_data.br_h = this->panel_height_size;

    this->current_render_data.bg_w = (int)this->panel_width_size - 2 * (int)this->border_width_size;
    this->current_render_data.bg_h = (int)this->panel_height_size - 2 * (int)this->border_width_size;

    this->current_render_data.sw_r = static_cast<unsigned int>(std::round(this->border_radius_size * this->shadow_scale_factor));
    this->current_render_data.br_r = this->border_radius_size;


    int bg_r_signed =  static_cast<int>(this->border_radius_size) -  static_cast<int>(this->border_width_size);

    this->current_render_data.bg_r = std::max(0, bg_r_signed);
}

// =========================================================================================== GUI


// =========================================================================================== PANEL SPECIFIC

void My_SDL_panel::add_element(My_SDL_element* element_pointer, int local_x, int local_y, unsigned int local_z)
{
    if (!element_pointer) return;
    if (element_pointer->get_element_container() != nullptr) return;

    element_pointer->set_render_point(
        this->global_x_by_local_x(local_x),
        this->global_y_by_local_y(local_y)
    );

    panel_inner_element new_inner = {

        element_pointer,
        local_x,
        local_y,
        local_z

    };


    // Addition by z-order

    auto it = std::lower_bound(
        inner_elements.begin(),
        inner_elements.end(),
        local_z,
        [](const panel_inner_element& elem, unsigned int z)
        {
            return elem.local_z_position < z;
        }
    );

    inner_elements.insert(it, new_inner);


    element_pointer->element_container = this;

    element_pointer->set_opacity(element_pointer->get_basic_opacity());

    element_pointer->set_visible_flag(this->visible_flag);
}


void My_SDL_panel::remove_element(My_SDL_element* element_pointer)
{
    // Find an element inside the list of elements
    for (auto it = this->inner_elements.begin(); it != this->inner_elements.end(); ++it)
    {
        // If element is found
        if (it->element_pointer == element_pointer)
        {
            // 1. Delete the element from the panel inner elements list
            this->inner_elements.erase(it);

            // 2. Delete element 
            delete element_pointer; 

            return;
        }
    }
}


void My_SDL_panel::clear_elements()
{
    for (auto it = this->inner_elements.begin(); it != this->inner_elements.end(); )
    {
        // Always delete the first element until the list is empty, because after deletion the iterator will be invalidated
        inner_elements.front().element_pointer->delete_element();
    }
}


void My_SDL_panel::change_element_local_coordinate(

    My_SDL_element* element_pointer,

    int new_local_x,
    int new_local_y,
    unsigned int new_local_z

)
{
    // 1. Update render position immediately
    element_pointer->set_render_point(

        this->global_x_by_local_x(new_local_x),
        this->global_y_by_local_y(new_local_y)

    );

    // 2. Find element in container
    for (auto it = this->inner_elements.begin(); it != this->inner_elements.end(); ++it)
    {
        if (it->element_pointer == element_pointer)
        {
            it->local_x_position = new_local_x;
            it->local_y_position = new_local_y;

            bool z_changed = (it->local_z_position != new_local_z);
            it->local_z_position = new_local_z;

            // 3. If Z changed → reinsert in correct position
            if (z_changed)
            {
                panel_inner_element updated = *it;
                inner_elements.erase(it);

                auto insert_it = std::lower_bound(
                    inner_elements.begin(),
                    inner_elements.end(),
                    updated.local_z_position,
                    [](const panel_inner_element& elem, unsigned int z)
                    {
                        return elem.local_z_position < z;
                    }
                );

                inner_elements.insert(insert_it, updated);
            }

            return;
        }
    }
}

void My_SDL_panel::change_inner_elements_global_coordinates()
{
    for (auto it = this->inner_elements.begin(); it != this->inner_elements.end(); ++it)
    {
        it->element_pointer->set_render_point(this->global_x_by_local_x(it->local_x_position), this->global_y_by_local_y(it->local_y_position));
    }
}


int My_SDL_panel::global_x_by_local_x(int local_x)
{
    // Global panel render points
    int g_p_r_p_x_0 = this->element_anchor_points.top_left.x;

    // Required global x-coordinate by offset from (0; 0)
    return g_p_r_p_x_0 + local_x;
}


int My_SDL_panel::global_y_by_local_y(int local_y)
{
    // Global panel render points
    int g_p_r_p_y_0 = this->element_anchor_points.top_left.y;

    // Required global y-coordinate by offset from (0; 0)
    return g_p_r_p_y_0 + local_y;
}


int My_SDL_panel::local_x_by_global_x(int global_x)
{
    return global_x - this->element_anchor_points.top_left.x;
}

int My_SDL_panel::local_y_by_global_y(int global_y)
{
    return global_y - this->element_anchor_points.top_left.y;
}


// =========================================================================================== PANEL SPECIFIC
