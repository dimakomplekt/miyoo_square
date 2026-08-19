// my_sdl_button.cpp


// =========================================================================================== IMPORT

#include "my_sdl_button.h"

// Onetime CPP include for remove_element() method providing
#include "../my_sdl_panel/my_sdl_panel.h"


#include <algorithm>

// =========================================================================================== IMPORT


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR

My_SDL_button::My_SDL_button()
{
    // Basic settings setter

    // Types

    this->click_access_type  = BUTTON_DEFAULT_CLICK_PERMISSION;

    this->set_gui_type(STATIC_ELEMENT_GUI);


    // Callbacks nullptr

    this->extern_click_permission = {};
    this->on_hover = {};
    this->on_click = {};


    // Data

    // Control flags

    this->button_hovered = false;

    this->click_permission = true;
    this->button_clicked = false;
    this->button_clicked_tmp = false;

    this->current_button_state = DEFAULT_ES;
    this->prev_button_state = DEFAULT_ES;

    this->push_mode_on = true;
    this->press_offset = 0;


    // Textbox setup

    this->button_textbox = My_SDL_textbox();

    // Now the button content color controlled only by button itself 
    this->button_textbox.switch_passed_by_palette_flag(false);

    this->button_textbox.switch_textbox_type(BUTTON_TEXT);

    this->button_textbox.set_content("But");


    // Sizes

    this->set_size(100, 50);


    // Render points

    this->x_render_point = this->button_width_size / 2 + 1;
    this->y_render_point = this->button_height_size / 2 + 1;

    // Textbox render point reset
    this->button_textbox.set_render_point(this->x_render_point, this->y_render_point);


    this->reset_boundaries_points();

    this->border_width_size = 5;
    this->border_radius_size = 15;

    this->set_shadow_offset(10, -10);
    this->shadow_scale_factor = 1.0f;


    // Anchor points
    this->reset_anchor_points();

    this->current_form = ROUNDED_RECTANGLE_EF;


    // ===== Default palette =====

    this->set_opacity(255);


    // Basic colors

    this->basic_background_color = App_palette.get_current_palette().basic_background_color;
    this->basic_border_color = App_palette.get_current_palette().basic_border_color;
    this->basic_content_color = App_palette.get_current_palette().basic_content_color;
    this->basic_shadow_color = App_palette.get_current_palette().basic_shadow_color;

    // Hover colors

    this->hover_background_color = App_palette.get_current_palette().hover_background_color;
    this->hover_border_color = App_palette.get_current_palette().hover_border_color;
    this->hover_content_color = App_palette.get_current_palette().hover_content_color;
    this->hover_shadow_color = App_palette.get_current_palette().hover_shadow_color;


    // button_clicked colors

    this->click_background_color = App_palette.get_current_palette().click_background_color;
    this->click_border_color = App_palette.get_current_palette().click_border_color;
    this->click_content_color = App_palette.get_current_palette().click_content_color;
    this->click_shadow_color = App_palette.get_current_palette().click_shadow_color;


    // 2nd palette

    this->access_denied_background_color = App_palette.get_current_palette().access_denied_background_color;
    this->access_denied_border_color = App_palette.get_current_palette().access_denied_border_color;
    this->access_denied_content_color = App_palette.get_current_palette().access_denied_content_color;;
    this->access_denied_shadow_color = App_palette.get_current_palette().access_denied_shadow_color;


    this->access_permitted_background_color = App_palette.get_current_palette().access_permitted_background_color;
    this->access_permitted_border_color = App_palette.get_current_palette().access_permitted_border_color;
    this->access_permitted_content_color = App_palette.get_current_palette().access_permitted_content_color;
    this->access_permitted_shadow_color = App_palette.get_current_palette().access_permitted_shadow_color;

    this->access_permitted_hover_background_color = App_palette.get_current_palette().access_permitted_hover_background_color;
    this->access_permitted_hover_border_color = App_palette.get_current_palette().access_permitted_hover_border_color;
    this->access_permitted_hover_content_color = App_palette.get_current_palette().access_permitted_hover_content_color;
    this->access_permitted_hover_shadow_color = App_palette.get_current_palette().access_permitted_hover_shadow_color;

    this->access_permitted_click_background_color = App_palette.get_current_palette().access_permitted_click_background_color;
    this->access_permitted_click_border_color = App_palette.get_current_palette().access_permitted_click_border_color;
    this->access_permitted_click_content_color = App_palette.get_current_palette().access_permitted_click_content_color;
    this->access_permitted_click_shadow_color = App_palette.get_current_palette().access_permitted_click_shadow_color;


    this->current_palette_number = 1;

    this->po_base_font = this->get_button_content_textbox()->ttf_font_link;
    this->po_base_size = (int)this->button_textbox.get_font_size();
    this->po_base_font_owned = false;
    this->po_cached = false;


    this->button_textbox_font_passed_by_font_palette  = true;
}


void My_SDL_button::delete_element()
{
    My_SDL_panel* container = this->get_element_container();

    if (container) container->remove_element(this);

    else delete this;
}


My_SDL_button::~My_SDL_button()
{
    // default

    this->cleanup();
}


void My_SDL_button::cleanup()
{
    // Base font guardianship return in case when the button is destroyed
    // in the middle of the press animation (the last press step font closes inside)
    if (this->po_cached)
    {
        if (this->po_base_font_owned) this->button_textbox.set_owned_font(this->po_base_font);

        else this->button_textbox.set_shared_font(this->po_base_font);

        this->po_cached = false;
    }

    // Texture and owned font delete by the textbox itself (the content texture is always
    // owned by the textbox, the fonts close only if they were self-opened - the shared
    // palette fonts are owned and closed by App_fonts)
    this->button_textbox.cleanup();
}



// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR


// =========================================================================================== MAIN LOGIC

void My_SDL_button::update()
{    
    // Write prev state
    this->prev_button_state = this->current_button_state;

    // No actions for not visible element
    if (!this->visible_flag) return;

    // Movement logic if the movement is on
    this->movement_logic_in_update_loop();

    // Hover check
    this->button_hover_check();

    // Hover logic
    if (this->button_hovered)
    {
        // Hover logic by the callback
        if (this->on_hover) this->on_hover();

        // Block the hover-click GUI conflict
        if (!this->button_clicked_tmp)
            this->current_button_state = HOVERED_ES;

        // New click or release check  
        this->button_clicked = lb_click_check();
    }
    else
    {
        if (!this->button_clicked_tmp) // Only without press
            this->current_button_state = DEFAULT_ES;

        // button_clicked flags reset with hover ending (blocks the click logic without hovering)
        this->button_clicked = false;
        this->button_clicked_tmp = false; 
    }


    // Click with hover check with the click permission logic by the button access type


    if (this->click_access_type == BUTTON_DEFAULT_CLICK_PERMISSION)
    {
        this->click_permission = true;
    }

    // There can not be anything instead of BUTTON_EXTERN_CLICK_PERMISSION
    else if (this->click_access_type == BUTTON_EXTERN_CLICK_PERMISSION)
    {
        // Permission check by the callback
        if (this->extern_click_permission)
        {
            this->click_permission = this->extern_click_permission();
        }
        else 
        {
            // Error handler
            std::cerr << "No callback for the button access status check! Click can't be handled!" << std::endl;
            return;
        }
    }


    if (this->click_permission && (this->button_hovered && this->button_clicked))
        if (!this->button_clicked_tmp) 
        {
            this->button_clicked_tmp = true;

            this->current_button_state = CLICKED_ES;
        }


    // If we press and then release - reset permission
    if (!this->button_clicked && this->button_clicked_tmp) 
    {
        // One callback call after release
        if (this->on_click)
        {
            this->on_click();
        }

        // Block repeats and reset
        this->button_clicked_tmp = false;

        if (this->button_hovered) this->current_button_state = HOVERED_ES; 
        else this->current_button_state = DEFAULT_ES;
    }

    // Check if the palette was switched and update the colors by the new palette if it was
    this->reset_colors_if_palette_switched();

    // Check if the font palette was switched and reset the button textbox if it was,
    // to update the font link and path if they are setted by the font palette
    this->reset_button_textbox_if_font_palette_switched();

    // Font prepare (onetime at the start or onetime by reset_flag from fonts palette)
    this->button_textbox.update();

    // Palette prepare for rendering
    this->button_palette_prepare();  

    this->render_data_recalculation();
}


void My_SDL_button::move_to_point(
            
    int new_x_cc_rp, 
    int new_y_cc_rp,

    movement_easing easing,

    Uint64 movement_time

)
{
    My_SDL_element::move_to_point(
            
        new_x_cc_rp, 
        new_y_cc_rp,
    
        easing,
    
        movement_time
    
    );

    this->get_button_content_textbox()->move_to_point(
            
        new_x_cc_rp, 
        new_y_cc_rp,
    
        easing,
    
        movement_time
    
    );
}



void My_SDL_button::switch_push_mode()
{
    this->push_mode_on = !this->push_mode_on;
}


void My_SDL_button::switch_button_textbox_type(app_textbox_type new_type)
{
    // Type switch
    this->get_button_content_textbox()->switch_textbox_type(new_type);

    // Font link rewrite
    switch (this->get_button_content_textbox()->textbox_type)
    {
        case BUTTON_TEXT:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().button_text_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().button_text_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().button_text_font.font_size;

            break;


        case HEADER_1:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().header_1_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().header_1_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().header_1_font.font_size;

            break;


        case HEADER_2:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().header_2_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().header_2_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().header_2_font.font_size;

            break;


        case HEADER_3:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().header_3_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().header_3_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().header_3_font.font_size;

            break;

            
        case ORDINARY_TEXT:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().ordinary_text_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().ordinary_text_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().ordinary_text_font.font_size;

            break;
        

        case SMALL_TEXT:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().small_text_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().small_text_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().small_text_font.font_size;

            break;


        case EMOJI_TEXT:

            this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().emoji_text_font.font_path);

            this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().emoji_text_font.ttf_font_link);

            this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().emoji_text_font.font_size;

            break;


        default: break;
    }

    // Reset base data
    this->po_base_font = this->get_button_content_textbox()->ttf_font_link;
    this->po_base_size = (int)this->button_textbox.get_font_size();

    this->po_base_font_owned = (

        this->po_base_font != nullptr &&
        this->get_button_content_textbox()->owned_font == this->po_base_font

    );

    this->po_cached = false;

    // Reset flag to not pass by palette after the first update by the palette switch

    this->get_button_content_textbox()->content_dirty = true;

    this->get_button_content_textbox()->passed_by_font_palette = false;
}


void My_SDL_button::set_access_type(button_access_type new_access_type)
{
    // Error handling for invalid access type
    if (new_access_type != BUTTON_DEFAULT_CLICK_PERMISSION && new_access_type != BUTTON_EXTERN_CLICK_PERMISSION)
    {
        std::cerr << "Invalid button access type. Access type not changed." << std::endl;
        return;
    }

    // New type setting
    else this->click_access_type = new_access_type;

    if (new_access_type == BUTTON_DEFAULT_CLICK_PERMISSION) this->set_gui_type(STATIC_ELEMENT_GUI);

    else if (new_access_type == BUTTON_EXTERN_CLICK_PERMISSION) this->set_gui_type(DYNAMIC_ELEMENT_GUI);
}


void My_SDL_button::button_hover_check()
{
    this->button_hovered = hover_check_by_boundaries(this->boundaries_points);
}

// =========================================================================================== MAIN LOGIC



// =========================================================================================== GUI

// Main render method

void My_SDL_button::render(SDL_Renderer* renderer)
{
    // No actions for not visible element
    if (!this->visible_flag) return;

    
    // Render 3 figures (shadow (border sizes * scaler), border and background (width or hight - 2 * border_width)) 
    // by their sizes, with use of current colors and render point (center-center)
    if (this->current_form == RECTANGLE_EF)
    {
        // SHADOW
        rectangle_draw_by_color(this->render_data.sw_cx, this->render_data.sw_cy, this->render_data.sw_w, this->render_data.sw_h, this->render_shadow_color, renderer);

        // BORDER
        rectangle_draw_by_color(this->render_data.br_cx, this->render_data.br_cy, this->render_data.br_w, this->render_data.br_h, this->render_border_color, renderer);

        // BACKGROUND
        rectangle_draw_by_color(this->render_data.bd_cx, this->render_data.bd_cy, this->render_data.bg_w, this->render_data.bg_h, this->render_background_color, renderer);
    }

    else if (this->current_form == ROUNDED_RECTANGLE_EF)
    {
        // SHADOW
        rounded_rectangle_draw_by_color(this->render_data.sw_cx, this->render_data.sw_cy, this->render_data.sw_w, this->render_data.sw_h, this->render_data.sw_r, this->render_shadow_color, renderer);

        // BORDER
        rounded_rectangle_draw_by_color(this->render_data.br_cx, this->render_data.br_cy, this->render_data.br_w, this->render_data.br_h, this->render_data.br_r, this->render_border_color, renderer);

        // BACKGROUND
        rounded_rectangle_draw_by_color(this->render_data.bd_cx, this->render_data.bd_cy, this->render_data.bg_w, this->render_data.bg_h, this->render_data.bg_r, this->render_background_color, renderer);
    }

    else if (this->current_form == CIRCLE_EF)
    {
        // SHADOW
        circle_draw_by_color(this->render_data.sw_cx, this->render_data.sw_cy, this->render_data.sw_w / 2, this->render_shadow_color, renderer);

        // BORDER
        circle_draw_by_color(this->render_data.br_cx, this->render_data.br_cy, this->render_data.br_w / 2, this->render_border_color, renderer);

        // BACKGROUND
        circle_draw_by_color(this->render_data.bd_cx, this->render_data.bd_cy, this->render_data.bg_w / 2, this->render_background_color, renderer);
    }


    // Content draw by SDL ttf
    // Update content texture if the content_dirty flag us true (or pass the previous texture if not)
    this->button_textbox.render(renderer);
}


void My_SDL_button::button_palette_prepare()
{
    // Current palette define logic
    this->prev_palette_number = this->current_palette_number;


    if (this->gui_type == STATIC_ELEMENT_GUI)
    {
        // Basic
        this->current_palette_number = 1;
    }

    else if (this->gui_type == DYNAMIC_ELEMENT_GUI && this->extern_click_permission) 
    {
        // Access denied
        if (!this->extern_click_permission()) this->current_palette_number = 2;

        // Access permitted
        else this->current_palette_number = 3;
    }

    // Access denied coloring in base case before callback extern_click_permission setup in loop
    else current_palette_number = 2;


    // Palette switch-case
    switch (this->current_palette_number)
    {

        case 1:
            // Basic
            if (this->current_button_state == DEFAULT_ES)
            {
                this->render_background_color = this->basic_background_color;
                this->render_border_color = this->basic_border_color;
                this->render_content_color = this->basic_content_color;
                this->render_shadow_color = this->basic_shadow_color;
            }

            // Hovered
            else if (this->current_button_state == HOVERED_ES)
            {
                this->render_background_color = this->hover_background_color;
                this->render_border_color = this->hover_border_color;
                this->render_content_color = this->hover_content_color;
                this->render_shadow_color = this->hover_shadow_color;
            }

            // button_clicked
            else if (this->current_button_state == CLICKED_ES)
            {
                this->render_background_color = this->click_background_color;
                this->render_border_color = this->click_border_color;
                this->render_content_color = this->click_content_color;
                this->render_shadow_color = this->click_shadow_color;
            }    

            break;


        case 2:

                // In any condition - just accept denied coloring
                this->render_background_color = this->access_denied_background_color;
                this->render_border_color = this->access_denied_border_color;
                this->render_content_color = this->access_denied_content_color;
                this->render_shadow_color = this->access_denied_shadow_color;
               

            break;


        case 3:
            // 2nd palette
            if (this->current_button_state == DEFAULT_ES)
            {
                this->render_background_color = this->access_permitted_background_color;
                this->render_border_color = this->access_permitted_border_color;
                this->render_content_color = this->access_permitted_content_color;
                this->render_shadow_color = this->access_permitted_shadow_color;
            }

            // Hovered
            else if (this->current_button_state == HOVERED_ES)
            {
                this->render_background_color = this->access_permitted_hover_background_color;
                this->render_border_color = this->access_permitted_hover_border_color;
                this->render_content_color = this->access_permitted_hover_content_color;
                this->render_shadow_color = this->access_permitted_hover_shadow_color;
            }

            // button_clicked
            else if (this->current_button_state == CLICKED_ES)
            {
                this->render_background_color = this->access_permitted_click_background_color;
                this->render_border_color = this->access_permitted_click_border_color;
                this->render_content_color = this->access_permitted_click_content_color;
                this->render_shadow_color = this->access_permitted_click_shadow_color;
            }    

            break;
    
        default: break;
    }
    

    // GLOBAL element opacity implementation
    float opacity_scaler = static_cast<float>(this->opacity) / 255.0f;

    this->render_shadow_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_shadow_color.a) * opacity_scaler));
    this->render_border_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_border_color.a) * opacity_scaler));
    this->render_background_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_background_color.a) * opacity_scaler));
    this->render_content_color.a = static_cast<uint8_t>(std::round(static_cast<float>(this->render_content_color.a) * opacity_scaler));


    // Onetime update render color for the textbox with content_dirty flag status change
    if ((this->prev_palette_number != this->current_palette_number) || (this->prev_button_state != this->current_button_state))
    {
        if (TEST_MODE) std::cout << "Need to switch button textbox color! Call switch!!!" << "\n << std::endl";
        
        this->button_textbox.set_content_color_if_palette_switched(this->render_content_color);
    } 
}


void My_SDL_button::reset_anchor_points()
{
    // Uses current crop to set the current anchor points

    // Always the same rounding accuracy, because we work with crop map in initial scale and new scalers
    int half_w = static_cast<int>(std::round(static_cast<float>(this->button_width_size) * 0.5));
    int half_h = static_cast<int>(std::round(static_cast<float>(this->button_height_size) * 0.5));


    // Element_anchor_points reset
    
    /**
     * 
     * P P P P P
     * 1 2 3 4 5
     * 
     * center is 3 -> (n + 1) / 2
     *
     */

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


// Render point setter (override) 

void My_SDL_button::set_render_point(int x_cc_rp, int y_cc_rp)
{
    this->x_render_point = x_cc_rp;
    this->y_render_point = y_cc_rp;

    // Button textbox render point reset
    this->button_textbox.set_render_point(x_cc_rp, y_cc_rp);


    this->reset_boundaries_points();

    this->reset_anchor_points();
}


// Size setters and getters

void My_SDL_button::set_size(unsigned int new_width, unsigned int new_height)
{
    // if (this->border_radius_size > new_width / 2 || this->border_radius_size > new_height / 2)
    // {
    //     std::cerr << "Wrong radius size value pass! Border radius size ain't changed" << std::endl;
    //     return;
    // }

    
    this->button_width_size = new_width;
    this->button_height_size = new_height;

    this->reset_boundaries_points();

    this->reset_current_form();

    this->reset_anchor_points();
}


unsigned int My_SDL_button::get_width_size() const { return this->button_width_size; }

unsigned int My_SDL_button::get_height_size() const { return this->button_height_size; }


// Styling setters

void My_SDL_button::set_border_width_size(unsigned int new_size)
{
    if ((new_size > this->button_width_size / 2) || 
        (new_size > this->button_height_size / 2) ||
        (this->border_radius_size != 0 && new_size > (this->border_radius_size - 1)))
    {
        std::cerr << "Wrong border size value pass! Border width size ain't changed" << std::endl;
        return;
    }

    this->border_width_size = new_size;
}


void My_SDL_button::set_border_radius(unsigned int new_size)
{
    if (new_size > this->button_width_size / 2 || new_size > this->button_height_size / 2)
    {
        std::cerr << "Wrong radius size value pass! Border radius size ain't changed" << std::endl;
        return;
    }

    this->border_radius_size = new_size;

    this->reset_current_form();
}


void My_SDL_button::set_shadow_offset(int new_x_offset, int new_y_offset)
{
    this->shadow_offset_x = new_x_offset;
    this->shadow_offset_y = -new_y_offset;  // SDL works with inverted y-axis on offset
}

void My_SDL_button::set_shadow_scale_factor(float new_scale_factor)
{ 
    this->shadow_scale_factor = new_scale_factor;
}


void My_SDL_button::reset_boundaries_points()
{
    this->boundaries_points.left_boundary = this->x_render_point - this->button_width_size / 2;
    this->boundaries_points.right_boundary = this->x_render_point + this->button_width_size / 2;
    this->boundaries_points.top_boundary = this->y_render_point - this->button_height_size / 2;
    this->boundaries_points.bottom_boundary = this->y_render_point + this->button_height_size / 2;
}


void My_SDL_button::reset_current_form()
{
    float half_w = this->button_width_size / 2.0f;
    float half_h = this->button_height_size / 2.0f;

    if (this->border_radius_size >= half_w && this->border_radius_size >= half_h)
        this->current_form = CIRCLE_EF;         

    else if (this->border_radius_size > 0)
        this->current_form = ROUNDED_RECTANGLE_EF; 
         
    else
        this->current_form = RECTANGLE_EF;           
}


// Color setters

// Palette 1

void My_SDL_button::set_basic_background_color(SDL_Color new_color)
{
    this->basic_background_color = new_color;
}

void My_SDL_button::set_basic_border_color(SDL_Color new_color)
{
    this->basic_border_color = new_color;
}

void My_SDL_button::set_basic_content_color(SDL_Color new_color)
{
    this->basic_content_color = new_color;
}

void My_SDL_button::set_basic_shadow_color(SDL_Color new_color)
{
    this->basic_shadow_color = new_color;
}

void My_SDL_button::set_hover_background_color(SDL_Color new_color)
{
    this->hover_background_color = new_color;
}

void My_SDL_button::set_hover_border_color(SDL_Color new_color)
{
    this->hover_border_color = new_color;
}

void My_SDL_button::set_hover_content_color(SDL_Color new_color)
{
    this->hover_content_color = new_color;
}

void My_SDL_button::set_hover_shadow_color(SDL_Color new_color)
{
    this->hover_shadow_color = new_color;
}

void My_SDL_button::set_click_background_color(SDL_Color new_color)
{
    this->click_background_color = new_color;
}

void My_SDL_button::set_click_border_color(SDL_Color new_color)
{
    this->click_border_color = new_color;
}

void My_SDL_button::set_click_content_color(SDL_Color new_color)
{
    this->click_content_color = new_color;
}

void My_SDL_button::set_click_shadow_color(SDL_Color new_color)
{
    this->click_shadow_color = new_color;
}


// Palette 2

void My_SDL_button::set_access_denied_background_color(SDL_Color new_color)
{
    this->access_denied_background_color = new_color;
}

void My_SDL_button::set_access_denied_border_color(SDL_Color new_color)
{
    this->access_denied_border_color = new_color;
}

void My_SDL_button::set_access_denied_content_color(SDL_Color new_color)
{
    this->access_denied_content_color = new_color;
}

void My_SDL_button::set_access_denied_shadow_color(SDL_Color new_color)
{
    this->access_denied_shadow_color = new_color;
}


void My_SDL_button::set_access_permitted_background_color(SDL_Color new_color)
{
    this->access_permitted_background_color = new_color;
}

void My_SDL_button::set_access_permitted_border_color(SDL_Color new_color)
{
    this->access_permitted_border_color = new_color;
}

void My_SDL_button::set_access_permitted_content_color(SDL_Color new_color)
{
    this->access_permitted_content_color = new_color;
}

void My_SDL_button::set_access_permitted_shadow_color(SDL_Color new_color)
{
    this->access_permitted_shadow_color = new_color;
}

void My_SDL_button::set_access_permitted_hover_background_color(SDL_Color new_color)
{
    this->access_permitted_hover_background_color = new_color;
}

void My_SDL_button::set_access_permitted_hover_border_color(SDL_Color new_color)
{
    this->access_permitted_hover_border_color = new_color;
}

void My_SDL_button::set_access_permitted_hover_content_color(SDL_Color new_color)
{
    this->access_permitted_hover_content_color = new_color;
}

void My_SDL_button::set_access_permitted_hover_shadow_color(SDL_Color new_color)
{
    this->access_permitted_hover_shadow_color = new_color;
}

void My_SDL_button::set_access_permitted_click_background_color(SDL_Color new_color)
{
    this->access_permitted_click_background_color = new_color;
}

void My_SDL_button::set_access_permitted_click_border_color(SDL_Color new_color)
{
    this->access_permitted_click_border_color = new_color;
}

void My_SDL_button::set_access_permitted_click_content_color(SDL_Color new_color)
{
    this->access_permitted_click_content_color = new_color;
}

void My_SDL_button::set_access_permitted_click_shadow_color(SDL_Color new_color)
{
    this->access_permitted_click_shadow_color = new_color;
}




void My_SDL_button::reset_colors_if_palette_switched()
{
    // Pass case

    if (!this->passed_by_palette || !App_palette.get_palette_reset_flag()) return;


    // Renew case

    // Basic colors

    this->basic_background_color = App_palette.get_current_palette().basic_background_color;
    this->basic_border_color = App_palette.get_current_palette().basic_border_color;
    this->basic_content_color = App_palette.get_current_palette().basic_content_color;
    this->basic_shadow_color = App_palette.get_current_palette().basic_shadow_color;

    // Hover colors

    this->hover_background_color = App_palette.get_current_palette().hover_background_color;
    this->hover_border_color = App_palette.get_current_palette().hover_border_color;
    this->hover_content_color = App_palette.get_current_palette().hover_content_color;
    this->hover_shadow_color = App_palette.get_current_palette().hover_shadow_color;


    // button_clicked colors

    this->click_background_color = App_palette.get_current_palette().click_background_color;
    this->click_border_color = App_palette.get_current_palette().click_border_color;
    this->click_content_color = App_palette.get_current_palette().click_content_color;
    this->click_shadow_color = App_palette.get_current_palette().click_shadow_color;


    // 2nd palette

    this->access_denied_background_color = App_palette.get_current_palette().access_denied_background_color;
    this->access_denied_border_color = App_palette.get_current_palette().access_denied_border_color;
    this->access_denied_content_color = App_palette.get_current_palette().access_denied_content_color;;
    this->access_denied_shadow_color = App_palette.get_current_palette().access_denied_shadow_color;


    this->access_permitted_background_color = App_palette.get_current_palette().access_permitted_background_color;
    this->access_permitted_border_color = App_palette.get_current_palette().access_permitted_border_color;
    this->access_permitted_content_color = App_palette.get_current_palette().access_permitted_content_color;
    this->access_permitted_shadow_color = App_palette.get_current_palette().access_permitted_shadow_color;

    this->access_permitted_hover_background_color = App_palette.get_current_palette().access_permitted_hover_background_color;
    this->access_permitted_hover_border_color = App_palette.get_current_palette().access_permitted_hover_border_color;
    this->access_permitted_hover_content_color = App_palette.get_current_palette().access_permitted_hover_content_color;
    this->access_permitted_hover_shadow_color = App_palette.get_current_palette().access_permitted_hover_shadow_color;

    this->access_permitted_click_background_color = App_palette.get_current_palette().access_permitted_click_background_color;
    this->access_permitted_click_border_color = App_palette.get_current_palette().access_permitted_click_border_color;
    this->access_permitted_click_content_color = App_palette.get_current_palette().access_permitted_click_content_color;
    this->access_permitted_click_shadow_color = App_palette.get_current_palette().access_permitted_click_shadow_color;

    
    // Renew current colors

    this->button_palette_prepare();

    this->button_textbox.set_content_color_if_palette_switched(this->render_content_color);
}


void My_SDL_button::reset_button_textbox_if_font_palette_switched()
{
    if (this->button_textbox_font_passed_by_font_palette && App_fonts.get_fonts_palette_reset_flag())
    {
        switch (this->get_button_content_textbox()->textbox_type)
        {
            case BUTTON_TEXT:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().button_text_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().button_text_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().button_text_font.font_size;
                }

                break;


            case HEADER_1:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().header_1_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().header_1_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().header_1_font.font_size;
                }

                break;


            case HEADER_2:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().header_2_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().header_2_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().header_2_font.font_size;
                }

                break;


            case HEADER_3:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().header_3_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().header_3_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().header_3_font.font_size;
                }

                break;


            case ORDINARY_TEXT:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().ordinary_text_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().ordinary_text_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().ordinary_text_font.font_size;
                }

                break;
            

            case SMALL_TEXT:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().small_text_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().small_text_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().small_text_font.font_size;
                }

                break;


            case EMOJI_TEXT:

                this->get_button_content_textbox()->font_path = (App_fonts.get_current_fonts_palette().emoji_text_font.font_path);

                if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
                {
                    this->button_textbox.set_ttf_font_link(App_fonts.get_current_fonts_palette().emoji_text_font.ttf_font_link);

                    this->get_button_content_textbox()->font_size = App_fonts.get_current_fonts_palette().emoji_text_font.font_size;
                }

                break;


            default: break;
        }

        // Reset base data (only outside of the press animation - the po_base_* data
        // must stay valid for the release restore; the two-loops-long reset flag
        // guarantees the second pass after the release restore)
        if (this->press_offset == 0 && !this->button_clicked && !this->button_clicked_tmp)
        {
            this->po_base_font = this->get_button_content_textbox()->ttf_font_link;
            this->po_base_size = (int)this->button_textbox.get_font_size();

            this->po_base_font_owned = (

                this->po_base_font != nullptr &&
                this->get_button_content_textbox()->owned_font == this->po_base_font

            );

            this->po_cached = false;
        }

        // Reset flag to not pass by palette after the first update by the palette switch

        this->get_button_content_textbox()->content_dirty = true;

        // Now the button content font controlled only by button itself
        this->get_button_content_textbox()->passed_by_font_palette = false;

    }
}


void My_SDL_button::button_textbox_font_passed_by_font_palette_flag_switch(bool new_flag)
{
    this->button_textbox_font_passed_by_font_palette = new_flag;
}


void My_SDL_button::render_data_recalculation()
{
    // Figures to build data calculation


    // Render points
    this->render_data.sw_cx = this->x_render_point + this->shadow_offset_x;
    this->render_data.sw_cy = this->y_render_point + this->shadow_offset_y;

    this->render_data.br_cx = this->x_render_point;
    this->render_data.br_cy = this->y_render_point;

    this->render_data.bd_cx = this->x_render_point;
    this->render_data.bd_cy = this->y_render_point;


    // Sizes 

    this->render_data.sw_w = static_cast<unsigned int>(std::round((this->button_width_size - this->press_offset) * this->shadow_scale_factor));
    this->render_data.sw_h = static_cast<unsigned int>(std::round((this->button_height_size - this->press_offset) * this->shadow_scale_factor));

    this->render_data.br_w = this->button_width_size - this->press_offset; 
    this->render_data.br_h = this->button_height_size - this->press_offset;

    int bg_w_signed = (int)this->button_width_size - 2 * (int)this->border_width_size - static_cast<int>(std::round(static_cast<float>(this->press_offset) / 2));
    int bg_h_signed = (int)this->button_height_size - 2 * (int)this->border_width_size - static_cast<int>(std::round(static_cast<float>(this->press_offset) / 2));
    
    this->render_data.bg_w = std::max(0, bg_w_signed);
    this->render_data.bg_h = std::max(0, bg_h_signed);

    this->render_data.sw_r = static_cast<unsigned int>(std::round(this->border_radius_size * this->shadow_scale_factor));
    this->render_data.br_r = this->border_radius_size - static_cast<int>(std::round(static_cast<float>(this->press_offset) / 2));


    int bg_r_signed = (int)this->border_radius_size - (int)this->border_width_size - static_cast<int>(std::round(static_cast<float>(this->press_offset) / 2));

    this->render_data.bg_r = std::max(0, bg_r_signed);

    
    // Increment the press offset at every render repeat

    // PRESS ANIMATION (simplified)
    
    // Cache
    if (this->button_clicked_tmp && this->press_offset == 0 && !this->po_cached)
    {
        int current_size = (int)this->button_textbox.get_font_size();

        if (current_size > 0)
        {
            this->po_base_font = this->get_button_content_textbox()->ttf_font_link;
            this->po_base_size = current_size;

            // Base font guardianship goes to the button for the press animation time,
            // so the press step fonts close by the set_owned_font() calls chain
            // without touching the base font
            this->po_base_font_owned = (

                this->po_base_font != nullptr &&
                this->get_button_content_textbox()->owned_font == this->po_base_font

            );

            this->get_button_content_textbox()->owned_font = nullptr;

            this->po_cached = true;
        }
    }


    if (this->button_clicked_tmp)
    {
        if (this->push_mode_on && this->po_cached)
        {
            // Temporarily disable palette font management during press animation.
            // Previous textbox type will be restored on release.
            this->button_textbox.switch_textbox_type(NO_TYPE);


            if (this->press_offset < 5) this->press_offset++;
    
            int new_size = std::max(10, static_cast<int>(this->po_base_size - 0.5 * this->press_offset));
    
    
            // Only 4 + 1 (on release) TTF_OpenFont operation quantity control by:
            // (second condition for repetitive update block)
            if (this->press_offset <= 5 && this->button_textbox.get_font_size() != new_size)
            {
                TTF_Font* new_font = TTF_OpenFont(
                    this->button_textbox.get_font_path().c_str(),
                    new_size
                );
        
                if (new_font)
                {
                    // Set new font with the ownership pass - the previous press step font
                    // closes inside set_owned_font(), the base font is not owned by the
                    // textbox during the press, so it stays untouched
                    this->get_button_content_textbox()->set_owned_font(new_font);

                    this->get_button_content_textbox()->font_size = static_cast<unsigned int>(new_size);

                    this->get_button_content_textbox()->content_dirty = true;
                }
            }
        }
    }
    else
    {
        if (this->push_mode_on && this->press_offset >= 1)
        {
            this->button_textbox.switch_textbox_type(this->button_textbox.get_prev_textbox_type());


            this->press_offset = 0;

            if (this->po_cached)
            {
                // Base font return with the guardianship return - the last press step font
                // closes inside the set_owned_font() / set_shared_font() call

                if (this->po_base_font_owned) this->get_button_content_textbox()->set_owned_font(this->po_base_font);

                else this->get_button_content_textbox()->set_shared_font(this->po_base_font);


                this->get_button_content_textbox()->font_size = static_cast<unsigned int>(this->po_base_size);

                this->po_cached = false;
            }

            this->get_button_content_textbox()->content_dirty = true;


            // Block the palette font re-fetch on the next update_font() call and keep the
            // pre-press flags state - the base font is already restored manually here
            this->get_button_content_textbox()->textbox_type_changed = false;

            this->get_button_content_textbox()->passed_by_font_palette = false;
        }
    }

}


My_SDL_textbox* My_SDL_button::get_button_content_textbox()
{
    return &this->button_textbox;
}
// =========================================================================================== GUI
