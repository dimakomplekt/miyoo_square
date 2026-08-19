// my_sdl_fader.cpp


// =========================================================================================== IMPORT

#include "my_sdl_fader.h"

// Onetime CPP include for remove_element() method providing
#include "../my_sdl_panel/my_sdl_panel.h"

// =========================================================================================== IMPORT


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR

My_SDL_fader::My_SDL_fader()
{
    // Basic settings setter

    // Data

    this->fader_value = 0.5f;
    this->prev_fader_value = 0.5f;

    // Control flags

    this->slot_hovered = false;
    this->slot_clicked = false;
    this->slot_clicked_tmp = false;
    this->current_slot_state = DEFAULT_ES;

    this->knob_hovered = false;
    this->knob_clicked = false;
    this->knob_clicked_tmp = false;
    this->current_knob_state = DEFAULT_ES;


    this->delta_x_from_click_to_knob = 0;

    this->push_mode_on = true;
    this->press_offset = 0;

    // Sizes

    this->slot_border_width_size = 2;
    this->slot_border_radius_size = 5;

    this->knob_border_width_size = 2;
    this->knob_border_radius_size = 5;


    this->set_slot_size(120, 20);

    this->set_knob_size(20, 40);


    this->set_slot_shadow_offset(3, -2);
    this->slot_shadow_scale_factor = 1.0f;

    this->set_knob_shadow_offset(3, -2);
    this->knob_shadow_scale_factor = 1.0f;


    this->slot_current_form = ROUNDED_RECTANGLE_EF;
    this->knob_current_form = ROUNDED_RECTANGLE_EF;


    // Render points

    this->slot_x_render_point = this->slot_width_size / 2 + 1;
    this->slot_y_render_point = this->slot_height_size / 2 + 1 + 200;   // test value


    this->knob_x_render_point = this->knob_position_by_fader_value();
    this->knob_y_render_point = this->slot_y_render_point;


    // borders

    this->reset_knob_boundaries_points();
    this->reset_slot_boundaries_points();


    // Anchor points
    this->reset_anchor_points();


    // Palette

    this->set_opacity(255);


    // Basic colors by palette

    this->slot_background_color = App_palette.get_current_palette().basic_background_color; 
    this->slot_border_color = App_palette.get_current_palette().basic_border_color; 
    this->slot_shadow_color = App_palette.get_current_palette().basic_shadow_color; 

    this->slot_background_color_hovered = App_palette.get_current_palette().hover_background_color;
    this->slot_border_color_hovered = App_palette.get_current_palette().hover_border_color;
    this->slot_shadow_color_hovered = App_palette.get_current_palette().hover_shadow_color;


    this->knob_background_color = App_palette.get_current_palette().basic_background_color;
    this->knob_border_color = App_palette.get_current_palette().basic_border_color;
    this->knob_shadow_color = App_palette.get_current_palette().basic_shadow_color;

    this->knob_background_color_hovered = App_palette.get_current_palette().hover_background_color;
    this->knob_border_color_hovered = App_palette.get_current_palette().hover_border_color;
    this->knob_shadow_color_hovered = App_palette.get_current_palette().hover_shadow_color;

    this->knob_background_color_clicked = App_palette.get_current_palette().click_background_color;
    this->knob_border_color_clicked = App_palette.get_current_palette().click_border_color;
    this->knob_shadow_color_clicked = App_palette.get_current_palette().click_shadow_color;


    // Initial render palette setup

    this->slot_render_shadow_color = this->slot_shadow_color;
    this->slot_render_border_color = this->slot_border_color;
    this->slot_render_background_color = this->slot_background_color;

    this->knob_render_shadow_color = this->knob_shadow_color;
    this->knob_render_border_color = this->knob_border_color;
    this->knob_render_background_color = this->knob_background_color;
}


void My_SDL_fader::delete_element()
{
    My_SDL_panel* container = this->get_element_container();

    
    if (container)
    {
        container->remove_element(this);
    }
    else
    {
        delete this;
    }
}


My_SDL_fader::~My_SDL_fader() = default;


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR


// =========================================================================================== MAIN LOGIC

float My_SDL_fader::get_fader_value() const
{
    return this->fader_value;
}


void My_SDL_fader::set_fader_value(float new_value)
{
    if (new_value < 0.0 || new_value > 1.0) return;

    this->fader_value = new_value;

    int new_x_point = this->knob_position_by_fader_value();

    this->set_knob_render_point(new_x_point);
}


int My_SDL_fader::fader_value_to_int_from_range(int min, int max)
{
    float val = this->get_fader_value();
    
    // std::round ensures correct switching at the midpoint of the fader travel
    return min + static_cast<int>(std::round(val * (max - min)));
}


float My_SDL_fader::fader_value_to_float_from_range(float min, float max)
{
    float val = this->get_fader_value();
    
    // Standart linear interpolation (lerp)
    return min + val * (max - min);
}


void My_SDL_fader::set_fader_value_by_int_from_range(int curr, int min, int max)
{
    // Prevent division by zero if the range is invalid
    if (max == min) return; 

    // Calculate the normalized proportion of the current value within the range
    float new_value = static_cast<float>(curr - min) / static_cast<float>(max - min);

    // set_fader_value handles boundary checks (0.0 - 1.0) and updates the knob position
    this->set_fader_value(new_value);
}


void My_SDL_fader::set_fader_value_by_float_from_range(float curr, float min, float max)
{
    // Prevent division by zero for floating-point values
    if (std::abs(max - min) < 1e-6f) return; 

    // Inverse linear interpolation (get normalized 0.0 - 1.0 value)
    float new_value = (curr - min) / (max - min);

    this->set_fader_value(new_value);
}




void My_SDL_fader::update()
{
    // No actions for not visiable element
    if (!this->visible_flag) return;


    // Update prev_fader_value for next step
    this->prev_fader_value = this->fader_value;


    // Movement logic if the movement is on
    this->movement_logic_in_update_loop();


    // Check if the palette was switched and update the colors by the new palette if it was
    this->reset_colors_if_palette_switched();

    // Slot hover check
    this->slot_hover_check();

    // Knob hover check
    this->knob_hover_check();


    // Knob hover logic before slot hover logic - if knob is hovered - we set the both knob
    // and slot states as HOVERED and start to check the mouse LB-click

    if (this->knob_hovered)
    {
        // Block the hover-click GUI conflict
        if (!this->knob_clicked_tmp)
        {
            this->current_slot_state = DEFAULT_ES;      // Reset the hover for the slot
            this->slot_clicked = false;                 // Reset the click for the slot     

            this->current_knob_state = HOVERED_ES;
        }

        // New click or release check  
        this->knob_clicked = lb_click_check();
    }

    // Slot hover logic - if slot is hovered and knob not hovered - we set the slot state
    // as HOVERED and start to check mouse LB-click 

    else if (this->slot_hovered && !this->knob_hovered)
    {   
        // Block the hover-click GUI conflict
        if(!this->slot_clicked_tmp) 
        {
            this->current_knob_state = DEFAULT_ES;      // Reset the hover for the knob
            this->knob_clicked = false;                 // Reset the click for the knob   

            this->current_slot_state = HOVERED_ES;  
        }

        // New click or release check  
        this->slot_clicked = lb_click_check();
    }

    else
    {
        // Only without press
        if (!this->slot_clicked_tmp) 
        {
            this->current_slot_state = DEFAULT_ES;
    
            // Clicked flags reset with hover ending (blocks the click logic without hovering)
            this->slot_clicked = false; 
        }


        // Only without press
        if (!this->knob_clicked_tmp)
        {
            this->current_knob_state = DEFAULT_ES;
    
            // Clicked flags reset with hover ending (blocks the click logic without hovering)
            this->knob_clicked = false;
        }
    }


    // If we click inside the slot zone and then release - just set the new palette, 
    // then new values, then reset everything to default state. If we click inside the slot
    // zone and then hold - we must move to the zone where we click and follow the mouse
    // x-position by the slot center. So the logic of 2 cases could be mixed only 
    // in the logic of second case.

    // Furthermore - if we click inside the knob zone - we must act like in the 2nd case, 
    // but only with exception. that we don't need to instantly change the position of the knob
    // by the small deltas between current knob center x-coordinate and the x-coordinate inside 
    // the knob zone, which was detected with the click - just move the knob with taking
    // of this delta into account!

    // Slot click case
    if (this->slot_clicked && !this->slot_clicked_tmp)
    {   
        this->slot_clicked_tmp = true;
        this->current_slot_state = CLICKED_ES;

        // Set the delta for the follow logic

        this->delta_x_from_click_to_knob = 0;
    }


    // Knob click case

    if (this->knob_clicked && !this->knob_clicked_tmp)
    {
        // We click inside the knob zone - we must move to the zone where we click and follow the mouse
        // x-position by the slot center with taking of the delta between current knob center x-coordinate and the x-coordinate inside 
        // the knob zone, which was detected with the click into account

        this->knob_clicked_tmp = true;
        this->current_knob_state = CLICKED_ES;

        // Set the delta for the follow logic
        this->delta_x_from_click_to_knob = static_cast<int>(std::round(App_mouse.get_x())) - this->knob_x_render_point;
    }

    // Click hold case both for knob and slot, but only with the knob follow logic
    if ((this->knob_clicked && this->knob_clicked_tmp) || (this->slot_clicked && this->slot_clicked_tmp))
    {
        // We must move to the zone where we click and follow the mouse x-position by the slot center
        // with taking of the delta between current knob center x-coordinate and the x-coordinate inside 
        // the knob zone, which was detected with the click into account

        int new_knob_center_x = static_cast<int>(std::round(App_mouse.get_x())) - this->delta_x_from_click_to_knob;


        // We also must check if we are in the slot borders with the new position, if not - just set the position to the nearest border

        int left_border = this->slot_boundaries_points.left_boundary + (this->knob_width_size / 2);
        int right_border = this->slot_boundaries_points.right_boundary - (this->knob_width_size / 2);

        // Overflow check for the new knob position
        if (new_knob_center_x < left_border) new_knob_center_x = left_border;
        else if (new_knob_center_x > right_border) new_knob_center_x = right_border;

        this->set_knob_render_point(new_knob_center_x);

        // Update fader value by the new knob position

        this->fader_value = this->fader_value_by_knob_position();


        // New click / release check, even without hover

        this->knob_clicked = lb_click_check();
    }


    // If we release mouse LB
    if ((!this->slot_clicked && this->slot_clicked_tmp) || (!this->knob_clicked && this->knob_clicked_tmp)) 
    {
        // Block repeats and reset
        this->slot_clicked_tmp = false;
        this->knob_clicked_tmp = false;


        // Check if we still hover after the release for the correct state setting

        // Basically could not be possible, but anyway XD
        if (this->slot_hovered && !this->knob_hovered) 
        {
            this->current_slot_state = HOVERED_ES;
            this->current_knob_state = DEFAULT_ES;
        }

        // Case if we stay on the knob after the release
        else if (this->knob_hovered)
        {
            this->current_slot_state = DEFAULT_ES;
            this->current_knob_state = HOVERED_ES;
        }

        // Case if we stay outside after the release
        else if (!this->slot_hovered && !this->knob_hovered)
        {
            this->current_slot_state = DEFAULT_ES; 
            this->current_knob_state = DEFAULT_ES; 
        }
    }

    
    // Prepare the palette for rendering by the current slot and knob states
    this->fader_palette_prepare();

    // Update render data
    this->render_data_recalculation();

}


void My_SDL_fader::switch_push_mode()
{
    this->push_mode_on = !this->push_mode_on;
}


bool My_SDL_fader::fader_value_changed_at_last_step()
{
    // Will return true if value been changed
    return (this->fader_value != this->prev_fader_value);
}




float My_SDL_fader::fader_value_by_knob_position()
{
    // Fader value calculation by the knob position like ((s.x.rp + (s.w - k.w) / 2) - (s.x.rp + (s.w - k.w) / 2) - curr.x)) / (s.w - k.w) jr
    float new_fader_value = static_cast<float>(this->knob_x_render_point - (this->slot_x_render_point - 
    (this->slot_width_size - this->knob_width_size) / 2)) / static_cast<float>(this->slot_width_size - this->knob_width_size);

    // Overflow check for the fader value
    if (new_fader_value < 0.0f) new_fader_value = 0.0f;
    else if (new_fader_value > 1.0f) new_fader_value = 1.0f;

    return new_fader_value;
}


int My_SDL_fader::knob_position_by_fader_value()
{
    // Fader position calculation by the fader value like (s.x.rp + (s.w - k.w) / 2) + fader_value * (s.w - k.w)
    int new_knob_position = static_cast<int>(std::round((this->slot_x_render_point - (this->slot_width_size - this->knob_width_size) / 2) + this->fader_value * (this->slot_width_size - this->knob_width_size)));

    return new_knob_position;
}


void My_SDL_fader::slot_hover_check()
{
    this->slot_hovered = hover_check_by_boundaries(this->slot_boundaries_points);
}


void My_SDL_fader::knob_hover_check()
{
    this->knob_hovered = hover_check_by_boundaries(this->knob_boundaries_points);
}


// =========================================================================================== MAIN LOGIC


// =========================================================================================== GUI

void My_SDL_fader::render(SDL_Renderer *renderer)
{
    // No actions for not visiable element
    if (!this->visible_flag) return;

    // Render the slot and knob with the appropriate colors, sizes and positions, based on the current states

    // Slot render

    if (this->slot_current_form == RECTANGLE_EF)
    {
        // SHADOW
        rectangle_draw_by_color(this->render_data.slot_sw_cx, this->render_data.slot_sw_cy, this->render_data.slot_sw_w, this->render_data.slot_sw_h, this->slot_render_shadow_color, renderer);

        // BORDER
        rectangle_draw_by_color(this->render_data.slot_br_cx, this->render_data.slot_br_cy, this->render_data.slot_br_w, this->render_data.slot_br_h, this->slot_render_border_color, renderer);

        // BACKGROUND
        rectangle_draw_by_color(this->render_data.slot_bd_cx, this->render_data.slot_bd_cy, this->render_data.slot_bg_w, this->render_data.slot_bg_h, this->slot_render_background_color, renderer);
    }

    else if (this->slot_current_form == ROUNDED_RECTANGLE_EF)
    {
        // SHADOW
        rounded_rectangle_draw_by_color(this->render_data.slot_sw_cx, this->render_data.slot_sw_cy, this->render_data.slot_sw_w, this->render_data.slot_sw_h, this->render_data.slot_sw_r, this->slot_render_shadow_color, renderer);

        // BORDER
        rounded_rectangle_draw_by_color(this->render_data.slot_br_cx, this->render_data.slot_br_cy, this->render_data.slot_br_w, this->render_data.slot_br_h, this->render_data.slot_br_r, this->slot_render_border_color, renderer);

        // BACKGROUND
        rounded_rectangle_draw_by_color(this->render_data.slot_bd_cx, this->render_data.slot_bd_cy, this->render_data.slot_bg_w, this->render_data.slot_bg_h, this->render_data.slot_bg_r, this->slot_render_background_color, renderer);
    }

    // Knob render

    if (this->knob_current_form == RECTANGLE_EF)
    {
        // SHADOW
        rectangle_draw_by_color(this->render_data.knob_sw_cx, this->render_data.knob_sw_cy, this->render_data.knob_sw_w, this->render_data.knob_sw_h, this->knob_render_shadow_color, renderer);

        // BORDER
        rectangle_draw_by_color(this->render_data.knob_br_cx, this->render_data.knob_br_cy, this->render_data.knob_br_w, this->render_data.knob_br_h, this->knob_render_border_color, renderer);

        // BACKGROUND
        rectangle_draw_by_color(this->render_data.knob_bd_cx, this->render_data.knob_bd_cy, this->render_data.knob_bg_w, this->render_data.knob_bg_h, this->knob_render_background_color, renderer);
    }

    else if (this->knob_current_form == ROUNDED_RECTANGLE_EF)
    {
        // SHADOW
        rounded_rectangle_draw_by_color(this->render_data.knob_sw_cx, this->render_data.knob_sw_cy, this->render_data.knob_sw_w, this->render_data.knob_sw_h, this->render_data.knob_sw_r, this->knob_render_shadow_color, renderer);

        // BORDER
        rounded_rectangle_draw_by_color(this->render_data.knob_br_cx, this->render_data.knob_br_cy, this->render_data.knob_br_w, this->render_data.knob_br_h, this->render_data.knob_br_r, this->knob_render_border_color, renderer);

        // BACKGROUND
        rounded_rectangle_draw_by_color(this->render_data.knob_bd_cx, this->render_data.knob_bd_cy, this->render_data.knob_bg_w, this->render_data.knob_bg_h, this->render_data.knob_bg_r, this->knob_render_background_color, renderer);
    }

    else if (this->knob_current_form == CIRCLE_EF)
    {
        // SHADOW
        circle_draw_by_color(this->render_data.knob_sw_cx, this->render_data.knob_sw_cy, this->render_data.knob_sw_w / 2, this->knob_render_shadow_color, renderer);

        // BORDER
        circle_draw_by_color(this->render_data.knob_br_cx, this->render_data.knob_br_cy, this->render_data.knob_br_w / 2, this->knob_render_border_color, renderer);

        // BACKGROUND
        circle_draw_by_color(this->render_data.knob_bd_cx, this->render_data.knob_bd_cy, this->render_data.knob_bg_w / 2, this->knob_render_background_color, renderer);
    }
}


void My_SDL_fader::set_render_point(int x_cc_rp, int y_cc_rp)
{
    this->x_render_point = x_cc_rp;
    this->y_render_point = y_cc_rp;

    this->slot_x_render_point = x_cc_rp;
    this->slot_y_render_point = y_cc_rp;

    this->knob_y_render_point = y_cc_rp;


    this->knob_x_render_point = this->knob_position_by_fader_value();
    this->knob_y_render_point = this->slot_y_render_point;


    // borders

    this->reset_knob_boundaries_points();
    this->reset_slot_boundaries_points();


    // Anchor points
    this->reset_anchor_points();
    
}


void My_SDL_fader::set_slot_size(unsigned int new_width, unsigned int new_height)
{
    // Slot can't be circle
    if ((new_width < 120 || new_height < 10) || 
        (this->slot_border_width_size > new_width / 2) || 
        (this->slot_border_width_size > new_height / 2) || 
        (this->slot_border_radius_size != 0 && this->slot_border_width_size > (this->slot_border_radius_size - 1)))
    {
        std::cerr << "Wrong size value pass! Slot size ain't changed" << std::endl;
        return;
    }

    this->slot_width_size = new_width;
    this->slot_height_size = new_height;

    this->reset_slot_boundaries_points();
    this->reset_slot_current_form();

    this->reset_anchor_points();
}


unsigned int My_SDL_fader::get_slot_width_size() const
{
    return this->slot_width_size;
}


unsigned int My_SDL_fader::get_slot_height_size() const
{
    return this->slot_height_size;
}


void My_SDL_fader::set_knob_size(unsigned int new_width, unsigned int new_height)
{
    // (s.w - k.w  > 20px)
    // k.r < k.w / 2 or k.h / 2
    if ((this->slot_width_size - new_width <= 20) ||
        (this->knob_border_radius_size > new_width / 2 || this->knob_border_radius_size > new_height / 2)
    )
    {
        std::cerr << "Wrong size value pass! Knob size ain't changed" << std::endl;
        return;
    }

    this->knob_width_size = new_width;
    this->knob_height_size = new_height;

    this->reset_knob_boundaries_points();
    this->reset_knob_current_form();
}


unsigned int My_SDL_fader::get_knob_width_size() const
{
    return this->knob_width_size;
}


unsigned int My_SDL_fader::get_knob_height_size() const
{
    return this->knob_height_size;
}


void My_SDL_fader::set_slot_border_width_size(unsigned int new_size)
{
    if ((new_size > this->slot_width_size / 2) ||
        (new_size > this->slot_height_size / 2) ||
        (this->slot_border_radius_size != 0 && new_size > (this->slot_border_radius_size - 1)))
    {
        std::cerr << "Wrong border size value pass! Slot border width size ain't changed" << std::endl;
        return;
    }

    this->slot_border_width_size = new_size;
}


void My_SDL_fader::set_slot_border_radius(unsigned int new_size)
{
    if ((new_size > this->slot_width_size / 2) || (new_size > this->slot_height_size / 2))
    {
        std::cerr << "Wrong radius size value pass! Slot border radius size ain't changed" << std::endl;
        return;
    }

    this->slot_border_radius_size = new_size;

    this->reset_slot_current_form();
}


void My_SDL_fader::set_slot_shadow_offset(int new_x_offset, int new_y_offset)
{
    this->slot_shadow_offset_x = new_x_offset;
    this->slot_shadow_offset_y = -new_y_offset;
}


void My_SDL_fader::set_slot_shadow_scale_factor(float new_scale_factor)
{
    this->slot_shadow_scale_factor = new_scale_factor;
}


void My_SDL_fader::set_knob_border_width_size(unsigned int new_size)
{
    if ((new_size > this->knob_width_size / 2) || 
        (new_size > this->knob_height_size / 2) ||
        (this->knob_border_radius_size != 0 && new_size > (this->knob_border_radius_size - 1)))
    {
        std::cerr << "Wrong border size value pass! knob border width size ain't changed" << std::endl;
        return;
    }

    this->knob_border_width_size = new_size;
}


void My_SDL_fader::set_knob_border_radius(unsigned int new_size)
{
    if ((new_size > this->knob_width_size / 2) || (new_size > this->knob_height_size / 2))
    {
        std::cerr << "Wrong radius size value pass! knob border radius size ain't changed" << std::endl;
        return;
    }

    this->knob_border_radius_size = new_size;

    this->reset_knob_current_form();
}


void My_SDL_fader::set_knob_shadow_offset(int new_x_offset, int new_y_offset)
{
    this->knob_shadow_offset_x = new_x_offset;
    this->knob_shadow_offset_y = -new_y_offset;
}


void My_SDL_fader::set_knob_shadow_scale_factor(float new_scale_factor)
{
    this->knob_shadow_scale_factor = new_scale_factor;
}


void My_SDL_fader::reset_anchor_points()
{
    // TODO: RESET LOGIC WITH ADD OF THE HORIZONTAL AND VERTICAL VERSIONS OF FADER AND BY THE SIZES OF SLOT AND 
    // KNOB - F.E., LIKE: IF SLOT.H < KNOB.H => "half_h = static_cast<int>(std::round(static_cast<float>(this->knob_height_size) * 0.5))" ... 

    // Uses current crop to set the current anchor points

    // Always the same rounding accuracy, because we work with crop map in initial scale and new scalers
    int half_w = static_cast<int>(std::round(static_cast<float>(this->slot_width_size) * 0.5));
    
    int half_h = static_cast<int>(std::round(static_cast<float>(this->slot_height_size) * 0.5));

    // element_anchor_points reset
    
    /**
     * 
     * P P P P P
     * 1 2 3 4 5
     * 
     * center is 3 -> (n + 1) / 2
     *
     */

    int c_w = this->slot_x_render_point;         // Horizontal center
    int c_h = this->slot_y_render_point;         // Vertical center


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


// Color setters

void My_SDL_fader::set_slot_background_color(SDL_Color new_color)               { this->slot_background_color = new_color; }

void My_SDL_fader::set_slot_border_color(SDL_Color new_color)                   { this->slot_border_color = new_color; }

void My_SDL_fader::set_slot_shadow_color(SDL_Color new_color)                   { this->slot_shadow_color = new_color; }

void My_SDL_fader::set_slot_background_color_hovered(SDL_Color new_color)       { this->slot_background_color_hovered = new_color; }

void My_SDL_fader::set_slot_border_color_hovered(SDL_Color new_color)           { this->slot_border_color_hovered = new_color; }

void My_SDL_fader::set_slot_shadow_color_hovered(SDL_Color new_color)           { this->slot_shadow_color_hovered = new_color; }

void My_SDL_fader::set_knob_background_color(SDL_Color new_color)               { this->knob_background_color = new_color; }

void My_SDL_fader::set_knob_border_color(SDL_Color new_color)                   { this->knob_border_color = new_color; }

void My_SDL_fader::set_knob_shadow_color(SDL_Color new_color)                   { this->knob_shadow_color = new_color; }

void My_SDL_fader::set_knob_background_color_hovered(SDL_Color new_color)       { this->knob_background_color_hovered = new_color; }

void My_SDL_fader::set_knob_border_color_hovered(SDL_Color new_color)           { this->knob_border_color_hovered = new_color; }

void My_SDL_fader::set_knob_shadow_color_hovered(SDL_Color new_color)           { this->knob_shadow_color_hovered = new_color; }

void My_SDL_fader::set_knob_background_color_clicked(SDL_Color new_color)       { this->knob_background_color_clicked = new_color; }

void My_SDL_fader::set_knob_border_color_clicked(SDL_Color new_color)           { this->knob_border_color_clicked = new_color; }

void My_SDL_fader::set_knob_shadow_color_clicked(SDL_Color new_color)           { this->knob_shadow_color_clicked = new_color; }



void My_SDL_fader::reset_colors_if_palette_switched()
{
    // Pass case

    if (!this->passed_by_palette || !App_palette.get_palette_reset_flag()) return;

    // Renew case 

    this->slot_background_color = App_palette.get_current_palette().basic_background_color; 
    this->slot_border_color = App_palette.get_current_palette().basic_border_color; 
    this->slot_shadow_color = App_palette.get_current_palette().basic_shadow_color; 

    this->slot_background_color_hovered = App_palette.get_current_palette().hover_background_color;
    this->slot_border_color_hovered = App_palette.get_current_palette().hover_border_color;
    this->slot_shadow_color_hovered = App_palette.get_current_palette().hover_shadow_color;


    this->knob_background_color = App_palette.get_current_palette().basic_background_color;
    this->knob_border_color = App_palette.get_current_palette().basic_border_color;
    this->knob_shadow_color = App_palette.get_current_palette().basic_shadow_color;

    this->knob_background_color_hovered = App_palette.get_current_palette().hover_background_color;
    this->knob_border_color_hovered = App_palette.get_current_palette().hover_border_color;
    this->knob_shadow_color_hovered = App_palette.get_current_palette().hover_shadow_color;

    this->knob_background_color_clicked = App_palette.get_current_palette().click_background_color;
    this->knob_border_color_clicked = App_palette.get_current_palette().click_border_color;
    this->knob_shadow_color_clicked = App_palette.get_current_palette().click_shadow_color;

}




// Knob render point inner setter 

void My_SDL_fader::set_knob_render_point(int x_cc_rp)
{
    this->knob_x_render_point = x_cc_rp;

    this->reset_knob_boundaries_points();
}


// Form checkers

void My_SDL_fader::reset_slot_current_form()
{
    float half_w = this->slot_width_size / 2.0f;
    float half_h = this->slot_height_size / 2.0f;

    if (this->slot_border_radius_size >= half_w && this->slot_border_radius_size >= half_h)
        this->slot_current_form = CIRCLE_EF;         

    else if (this->slot_border_radius_size > 0)
        this->slot_current_form = ROUNDED_RECTANGLE_EF; 
         
    else
        this->slot_current_form = RECTANGLE_EF;       
}


void My_SDL_fader::reset_knob_current_form()
{
    float half_w = this->knob_width_size / 2.0f;
    float half_h = this->knob_height_size / 2.0f;

    if (this->knob_border_radius_size >= half_w && this->knob_border_radius_size >= half_h)
        this->knob_current_form = CIRCLE_EF;         

    else if (this->knob_border_radius_size > 0)
        this->knob_current_form = ROUNDED_RECTANGLE_EF; 
         
    else
        this->knob_current_form = RECTANGLE_EF;       
}


// Boundaries points resetters

void My_SDL_fader::reset_slot_boundaries_points()
{
    this->slot_boundaries_points.left_boundary = this->slot_x_render_point - this->slot_width_size / 2;
    this->slot_boundaries_points.right_boundary = this->slot_x_render_point + this->slot_width_size / 2;
    this->slot_boundaries_points.top_boundary = this->slot_y_render_point - this->slot_height_size / 2;
    this->slot_boundaries_points.bottom_boundary = this->slot_y_render_point + this->slot_height_size / 2;
}


void My_SDL_fader::reset_knob_boundaries_points()
{
    this->knob_boundaries_points.left_boundary = this->knob_x_render_point - this->knob_width_size / 2;
    this->knob_boundaries_points.right_boundary = this->knob_x_render_point + this->knob_width_size / 2;
    this->knob_boundaries_points.top_boundary = this->knob_y_render_point - this->knob_height_size / 2;
    this->knob_boundaries_points.bottom_boundary = this->knob_y_render_point + this->knob_height_size / 2;
}


// Palette preparation for render step (last update() action)

void My_SDL_fader::fader_palette_prepare()
{
    // Slot palette preparation

    if (this->current_slot_state == DEFAULT_ES)
    {
        this->slot_render_shadow_color = this->slot_shadow_color;
        this->slot_render_border_color = this->slot_border_color;
        this->slot_render_background_color = this->slot_background_color;
    }

    // Hovered
    else if (this->current_slot_state == HOVERED_ES)
    {
        this->slot_render_shadow_color = this->slot_shadow_color_hovered;
        this->slot_render_border_color = this->slot_border_color_hovered;
        this->slot_render_background_color = this->slot_background_color_hovered;
    }

    // Fader clicked, so we colorize the knob and reset the slot color to default / hovered
    else if (this->current_slot_state == CLICKED_ES)
    {
        this->slot_render_shadow_color = this->slot_shadow_color;
        this->slot_render_border_color = this->slot_border_color;
        this->slot_render_background_color = this->slot_background_color;
    }    


    // Knob palette preparation

    if (this->current_knob_state == DEFAULT_ES)
    {
        this->knob_render_shadow_color = this->knob_shadow_color;
        this->knob_render_border_color = this->knob_border_color;
        this->knob_render_background_color = this->knob_background_color;
    }

    // Hovered
    else if (this->current_knob_state == HOVERED_ES)
    {
        this->knob_render_shadow_color = this->knob_shadow_color_hovered;
        this->knob_render_border_color = this->knob_border_color_hovered;
        this->knob_render_background_color = this->knob_background_color_hovered;
    }

    // Fader clicked, so we colorize the knob and reset the slot color to default / hovered
    else if (this->current_knob_state == CLICKED_ES)
    {
        this->knob_render_shadow_color = this->knob_shadow_color_clicked;
        this->knob_render_border_color = this->knob_border_color_clicked;
        this->knob_render_background_color = this->knob_background_color_clicked;
    }    


    // Global opacity scaler for the render palette

    float opacity_scaler = static_cast<float>(this->opacity) / 255.0f;

    this->slot_render_shadow_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->slot_render_shadow_color.a) * opacity_scaler));
    this->slot_render_border_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->slot_render_border_color.a) * opacity_scaler));
    this->slot_render_background_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->slot_render_background_color.a) * opacity_scaler));


    this->knob_render_shadow_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->knob_render_shadow_color.a) * opacity_scaler));
    this->knob_render_border_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->knob_render_border_color.a) * opacity_scaler));
    this->knob_render_background_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->knob_render_background_color.a) * opacity_scaler));
}



void My_SDL_fader::render_data_recalculation()
{
    // ===== PRESS OFFSET RESET =====
    if (this->current_slot_state != CLICKED_ES &&
        this->current_knob_state != CLICKED_ES)
    {
        this->press_offset = 0;
    }

    auto& rd = this->render_data;


    // ===== SLOT CENTERS =====
    rd.slot_sw_cx = this->slot_x_render_point + this->slot_shadow_offset_x;
    rd.slot_sw_cy = this->slot_y_render_point + this->slot_shadow_offset_y;

    rd.slot_br_cx = this->slot_x_render_point;
    rd.slot_br_cy = this->slot_y_render_point;

    rd.slot_bd_cx = this->slot_x_render_point;
    rd.slot_bd_cy = this->slot_y_render_point;


    // ===== KNOB CENTERS =====
    rd.knob_sw_cx = this->knob_x_render_point + this->knob_shadow_offset_x;
    rd.knob_sw_cy = this->knob_y_render_point + this->knob_shadow_offset_y;

    rd.knob_br_cx = this->knob_x_render_point;
    rd.knob_br_cy = this->knob_y_render_point;

    rd.knob_bd_cx = this->knob_x_render_point;
    rd.knob_bd_cy = this->knob_y_render_point;


    // ===== SLOT SIZES =====
    rd.slot_sw_w = (unsigned int)std::round((this->slot_width_size - this->press_offset) * this->slot_shadow_scale_factor);
    rd.slot_sw_h = (unsigned int)std::round((this->slot_height_size - this->press_offset) * this->slot_shadow_scale_factor);

    rd.slot_br_w = this->slot_width_size - this->press_offset;
    rd.slot_br_h = this->slot_height_size - this->press_offset;

    int slot_bg_w_signed = (int)this->slot_width_size - 2 * (int)this->slot_border_width_size - (int)std::round((float)this->press_offset / 2);
    int slot_bg_h_signed = (int)this->slot_height_size - 2 * (int)this->slot_border_width_size - (int)std::round((float)this->press_offset / 2);

    rd.slot_bg_w = std::max(0, slot_bg_w_signed);
    rd.slot_bg_h = std::max(0, slot_bg_h_signed);


    // ===== SLOT RADIUS =====
    rd.slot_sw_r = (unsigned int)std::round(this->slot_border_radius_size * this->slot_shadow_scale_factor);
    rd.slot_br_r = this->slot_border_radius_size - (int)std::round((float)this->press_offset / 2);

    int slot_bg_r_signed = (int)this->slot_border_radius_size - (int)this->slot_border_width_size - (int)std::round((float)this->press_offset / 2);
    rd.slot_bg_r = std::max(0, slot_bg_r_signed);


    // ===== KNOB SIZES =====
    rd.knob_sw_w = (unsigned int)std::round((this->knob_width_size - this->press_offset) * this->knob_shadow_scale_factor);
    rd.knob_sw_h = (unsigned int)std::round((this->knob_height_size - this->press_offset) * this->knob_shadow_scale_factor);

    rd.knob_br_w = this->knob_width_size - this->press_offset;
    rd.knob_br_h = this->knob_height_size - this->press_offset;

    int knob_bg_w_signed = (int)this->knob_width_size - 2 * (int)this->knob_border_width_size - (int)std::round((float)this->press_offset / 2);
    int knob_bg_h_signed = (int)this->knob_height_size - 2 * (int)this->knob_border_width_size - (int)std::round((float)this->press_offset / 2);

    rd.knob_bg_w = std::max(0, knob_bg_w_signed);
    rd.knob_bg_h = std::max(0, knob_bg_h_signed);


    // ===== KNOB RADIUS =====
    rd.knob_sw_r = (unsigned int)std::round(this->knob_border_radius_size * this->knob_shadow_scale_factor);
    rd.knob_br_r = this->knob_border_radius_size - (int)std::round((float)this->press_offset / 2);

    int knob_bg_r_signed = (int)this->knob_border_radius_size - (int)this->knob_border_width_size - (int)std::round((float)this->press_offset / 2);
    rd.knob_bg_r = std::max(0, knob_bg_r_signed);


    // ===== PRESS OFFSET UPDATE =====
    if (this->current_knob_state == CLICKED_ES)
    {
        if (this->push_mode_on && this->press_offset <= 5)
        {
            this->press_offset += 1;
        }
    }
    else
    {
        if (this->push_mode_on && this->press_offset > 0)
        {
            this->press_offset = 0;
        }
    }
}

// =========================================================================================== GUI
