// My_SDL_textbox.cpp


// =========================================================================================== IMPORT

#include "my_sdl_textbox.h"

// Onetime CPP include for remove_element() method providing
#include "../my_sdl_panel/my_sdl_panel.h"

// =========================================================================================== IMPORT


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR

My_SDL_textbox::My_SDL_textbox()
{
    // Basic settings setter

    // Data

    // Render points

    this->x_render_point = 300;
    this->y_render_point = 300;


    // Font - basically set as ordinary text font

    this->passed_by_font_palette = true;


    // Basic data, which will be switch further
    // need to be here for initialization on the first
    // cycle steps

    this->font_size = 18;

    this->content_width = 0;
    this->content_height = 0;

    this->content_dirty = true;

    this->content_texture = nullptr;

    // Basic data, which will be switch further
    // need to be here for initialization on the first
    // cycle steps


    this->prev_textbox_type = ORDINARY_TEXT;
    this->textbox_type = ORDINARY_TEXT;

    // Relative path to the font file from the executable (can be changed by the setter for the font path)
    this->font_path = App_fonts.get_current_fonts_palette().ordinary_text_font.font_path;
    
    if (App_fonts.get_current_fonts_palette().ordinary_text_font.ttf_font_link != nullptr)
    {
        this->ttf_font_link = App_fonts.get_current_fonts_palette().ordinary_text_font.ttf_font_link;
        this->font_size = App_fonts.get_current_fonts_palette().ordinary_text_font.font_size;
    }
    
    else
    {
        // this->ttf_font_link = TTF_OpenFont(this->font_path.c_str(), this->font_size); - Maybe destroys the logic

        if (!this->ttf_font_link)
        {
            SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        }
    }


    // if (!ttf_font_link) this->update(); - Maybe destroys the logic

    // Content and sizes set with anchor points reset
    this->set_content("Text");


    // ===== Default palette =====

    this->set_opacity(255);


    // Basic text color by palette
    // flag for showing that the color setted by the pallet, automatically set to true inside the basic class constructor
    set_content_color(App_palette.get_current_palette().basic_content_color);

    // Upper function resetted this flag XD
    this->passed_by_palette = true;


    // Blinking mode setup

    // Turn off by default 

    this->blinking_mode_context.blinking_mode_on = false;


    this->blinking_mode_context.blinking_period = 1400;
    this->blinking_mode_context.blinking_duty = 0.4;

    this->blinking_mode_context.active_time = static_cast<Uint64>(std::round(

        this->blinking_mode_context.blinking_period * 
        this->blinking_mode_context.blinking_duty
        
    ));

    this->blinking_mode_context.phase = 0;

    // Start with inactive condition (constantly switch inside the 1st update)
    this->blinking_mode_context.active_now = true;


    this->content_dirty = true;

    this->textbox_type_changed = true;

    this->textbox_sizes_initiation_update_flag = true;
}


void My_SDL_textbox::delete_element()
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


My_SDL_textbox::~My_SDL_textbox()
{
    // TODO: Realization with panels linked list clear (both side registration) or just siple comment about 
    // destructor workflow rules - basic destructor from global space or My_SDL_panel.textbox_delete method
    // only for textboxes inside panels

    // Textures destructors
    this->cleanup();
}


void My_SDL_textbox::cleanup()
{
    // Texture delete - the content texture is ALWAYS owned by the textbox
    if (this->content_texture)
    {
        SDL_DestroyTexture(this->content_texture);
        this->content_texture = nullptr;
    }

    // Font delete - ONLY for the owned (self-opened) font,
    // the shared palette fonts are owned and closed by App_fonts
    if (this->owned_font)
    {
        TTF_CloseFont(this->owned_font);
        this->owned_font = nullptr;
    }

    this->ttf_font_link = nullptr;
}


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR

// Helper-function predeclare
void blinking_mode_control(blinking_textbox_ctx &blinking_ctx);


void My_SDL_textbox::update()
{
    // No actions for not visiable element
    if (!this->visible_flag) return;

    // Font update
    update_font();


    // Movement logic if the movement is on
    this->movement_logic_in_update_loop();

    // Control blinking if it's needed
    blinking_mode_control(this->blinking_mode_context);


    // Check if the palette was switched and update the colors by the new palette if it was
    this->reset_colors_if_palette_switched();
}


void My_SDL_textbox::update_font()
{
    // Just font start initialization in case when font not passed by font palette
    if (this->ttf_font_link == nullptr && !this->passed_by_font_palette)
    {
        // Self-opened font - the textbox takes the ownership
        if (!this->font_path.empty())
        {
            TTF_Font* self_opened_font = TTF_OpenFont(this->font_path.c_str(), this->font_size);

            if (self_opened_font) this->set_owned_font(self_opened_font);

            else SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        }
    }

    // Reset the font if current font palette was switched and the font was set by the font palette,
    // to update the font by the new palette
    if (this->passed_by_font_palette && (App_fonts.get_fonts_palette_reset_flag() || this->textbox_type_changed))
    {

        std::string curr_font_path;
        TTF_Font* curr_font_link = nullptr;    // nullptr init vs the uninitialized read on the default case


        switch (this->textbox_type)
        {
            case HEADER_1:

                curr_font_path = App_fonts.get_current_fonts_palette().header_1_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().header_1_font.ttf_font_link;
                this->font_size = App_fonts.get_current_fonts_palette().header_1_font.font_size;

                break;


            case HEADER_2:

                curr_font_path = App_fonts.get_current_fonts_palette().header_2_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().header_2_font.ttf_font_link;       
                this->font_size = App_fonts.get_current_fonts_palette().header_2_font.font_size;

                break;


            case HEADER_3:

                curr_font_path = App_fonts.get_current_fonts_palette().header_3_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().header_3_font.ttf_font_link;
                this->font_size = App_fonts.get_current_fonts_palette().header_3_font.font_size;
                
                break;


            case ORDINARY_TEXT:

                curr_font_path = App_fonts.get_current_fonts_palette().ordinary_text_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().ordinary_text_font.ttf_font_link;
                this->font_size = App_fonts.get_current_fonts_palette().ordinary_text_font.font_size;

                break;


            case SMALL_TEXT:

                curr_font_path = App_fonts.get_current_fonts_palette().small_text_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().small_text_font.ttf_font_link;
                this->font_size = App_fonts.get_current_fonts_palette().small_text_font.font_size;

                break;


            case BUTTON_TEXT:

                curr_font_path = App_fonts.get_current_fonts_palette().button_text_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().button_text_font.ttf_font_link;
                this->font_size = App_fonts.get_current_fonts_palette().button_text_font.font_size;

                break;


            case EMOJI_TEXT:

                curr_font_path = App_fonts.get_current_fonts_palette().emoji_text_font.font_path;
                curr_font_link = App_fonts.get_current_fonts_palette().emoji_text_font.ttf_font_link;
                this->font_size = App_fonts.get_current_fonts_palette().emoji_text_font.font_size;

                break;



            default:
            
                break;
                
        }


        if (curr_font_link != nullptr)
        {
            // Renew by the shared palette font (the previous owned font closes inside)

            this->font_path = curr_font_path;
            this->set_shared_font(curr_font_link);

        }
        else
        {
            // Reset
            
            // this->ttf_font_link = TTF_OpenFont(this->font_path.c_str(), this->font_size); - Maybe the reason of errors!!!
    
            if (!this->ttf_font_link)
            {
                SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
            }
        }

        this->textbox_type_changed = false;
        this->content_dirty = true;
    }
}


// Font ownership setters - the ONLY valid ways to change ttf_font_link inside the class code

void My_SDL_textbox::set_shared_font(TTF_Font* shared_font)
{
    // Previous owned font close (shared fonts are never closed here)
    if (this->owned_font != nullptr && this->owned_font != shared_font)
    {
        TTF_CloseFont(this->owned_font);
    }

    this->owned_font = nullptr;

    this->ttf_font_link = shared_font;

    this->content_dirty = true;
}


void My_SDL_textbox::set_owned_font(TTF_Font* new_owned_font)
{
    // Previous owned font close (with the same font double set protection)
    if (this->owned_font != nullptr && this->owned_font != new_owned_font)
    {
        TTF_CloseFont(this->owned_font);
    }

    this->owned_font = new_owned_font;

    this->ttf_font_link = new_owned_font;

    this->content_dirty = true;
}


void My_SDL_textbox::switch_textbox_type(app_textbox_type new_type)
{
    // Error handler
    if (new_type < HEADER_1 || new_type >= LIMIT)
    {
        std::cerr << "Wrong font type setting attempt!\n";
        return;
    }

    // No type case
    if (new_type == NO_TYPE)
    {
        // Type set

        // Save previous one with protection of double NO_TYPE set
        if (this->textbox_type != NO_TYPE) this->prev_textbox_type = this->textbox_type;

        this->textbox_type = new_type;

        // Flag reset
        this->passed_by_font_palette = false;

        return;
    }


    this->prev_textbox_type = this->textbox_type;
    this->textbox_type = new_type;

    this->passed_by_font_palette = true;
    
    this->textbox_type_changed = true;
    this->content_dirty = true;
}


app_textbox_type My_SDL_textbox::get_textbox_type() const
{
    return this->textbox_type;
}


app_textbox_type My_SDL_textbox::get_prev_textbox_type() const
{
    return this->prev_textbox_type;
}


// Helper-function for blinking mode parameters calculation
void blinking_mode_control(blinking_textbox_ctx &blinking_ctx)
{
    if (!blinking_ctx.blinking_mode_on)
    {
        blinking_ctx.active_now = true;
        return;
    }

    // Current time
    Uint64 t = SDL_GetTicks();

    // 1. Phase inside cycle
    blinking_ctx.phase = t % blinking_ctx.blinking_period;

    // 2. Condition
    blinking_ctx.active_now = (blinking_ctx.phase < blinking_ctx.active_time);
}


// =========================================================================================== GUI

void My_SDL_textbox::render(SDL_Renderer* renderer)
{
    // No actions for not visiable element
    if (!this->visible_flag) return;

    if (this->ttf_font_link == nullptr) return;

    if (this->content.empty()) return;

    this->reset_colors_if_palette_switched();

    // Render logic
    this->update_content_texture(renderer, this->content_render_color);
    
    // Basic draw if blinking mode is off
    if (!this->blinking_mode_context.blinking_mode_on) this->text_draw(renderer);

    else
    {
        // Draw only if the drawing is active
        if (this->blinking_mode_context.active_now) this->text_draw(renderer);
    }
}


void My_SDL_textbox::set_render_point(int x_cc_rp, int y_cc_rp)
{
    this->x_render_point = x_cc_rp;
    this->y_render_point = y_cc_rp;

    this->reset_anchor_points();
}


unsigned int My_SDL_textbox::get_width_size() const
{
    return this->content_width;
}


unsigned int My_SDL_textbox::get_height_size() const
{
    return this->content_height;
}



void My_SDL_textbox::set_content(const std::string& new_text)
{
    if (TEST_MODE) std::cout << "Set content: " << content << std::endl;

    this->content = new_text;

    if (!this->ttf_font_link)
    {
        std::cerr << "Font not initialized!\n";
        return;
    }
    
    this->content_dirty = true;
}


void My_SDL_textbox::switch_passed_by_font_palette_flag(bool new_flag)
{
    // Switch the type if false flag
    if (!new_flag)
    {
        this->switch_textbox_type(NO_TYPE);
    }

    // Protection vs double No_TYPE set flag reset cases
    if (new_flag && this->textbox_type == NO_TYPE)
    {
        this->switch_textbox_type(this->prev_textbox_type);
    }

    if (new_flag && this->textbox_type != NO_TYPE)
    {
        this->switch_textbox_type(this->textbox_type);
    }
}


bool My_SDL_textbox::get_passed_by_font_palette_flag() const
{
    return this->passed_by_font_palette;
}


void My_SDL_textbox::set_ttf_font_link(TTF_Font* new_ttf_font_link) 
{
    // TODO: Error handling for the invalid links
    if (!new_ttf_font_link)
    {
        std::cerr << "Invalid TTF_Font pointer! Font link not set!" << std::endl;
        return;
    }

    // External font links are SHARED (palette) fonts - link without taking ownership
    // (the previous owned font closes inside set_shared_font)
    this->set_shared_font(new_ttf_font_link);

    // TEST:

    if (TEST_MODE) std::cout << "TTF link setted: " << this->font_path << " with: " << this->font_size << "pts. \n" << std::endl;
}


void My_SDL_textbox::set_font_path(const std::string& new_font_path)
{
    // TODO: Error handling for the invalid links
    if (new_font_path.empty())
    {
        std::cerr << "Invalid font path! Font path not set!" << std::endl;
        return;
    }

    this->passed_by_font_palette = false;

    this->font_path = new_font_path;

    if (TEST_MODE) std::cout << "TTF font path setted: " << this->font_path << "\n" << std::endl;

    // Self-opened font - the textbox takes the ownership
    // (the previous owned font closes inside set_owned_font)
    TTF_Font* self_opened_font = TTF_OpenFont(this->font_path.c_str(), this->font_size);

    if (!self_opened_font)
    {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        return;
    }

    this->set_owned_font(self_opened_font);
}


std::string My_SDL_textbox::get_font_path() const
{
    return this->font_path;
}


void My_SDL_textbox::set_font_size(unsigned int new_size)
{
    if (new_size == 0)
    {
        std::cerr << "Invalid font size! Font size not set!" << std::endl;
        return;
    }

    this->font_size = new_size;

    this->passed_by_font_palette = false;

    if (TEST_MODE) std::cout << "Try to set content: " << this->content << " new size size: " << new_size << "\n" << std::endl;

    // Open new font with the ownership taking - the previous owned font closes inside
    // set_owned_font, the shared palette fonts stay untouched (the old direct TTF_CloseFont
    // here destroyed the logic exactly because it could close a shared palette font)
    TTF_Font* self_opened_font = TTF_OpenFont(this->font_path.c_str(), this->font_size);

    if (!self_opened_font)
    {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        return;
    }

    this->set_owned_font(self_opened_font);

    // rebuild geometry
    this->set_content(this->content);

    // force texture rebuild
    this->content_dirty = true;


    if (TEST_MODE) std::cout << "Set content: " << this->content << " new size size: " << new_size << "\n" << std::endl;
}


unsigned int My_SDL_textbox::get_font_size() const
{
    return this->font_size;
}


// For textboxes, which colors ain't passed by palette
void My_SDL_textbox::set_content_color(SDL_Color new_color)
{
    // Safely check of equivalence
    if (this->content_render_color.r == new_color.r &&
        this->content_render_color.g == new_color.g &&
        this->content_render_color.b == new_color.b &&
        this->content_render_color.a == new_color.a)
    {
        return;     // Return if nothing changed
    }

    this->passed_by_palette = false;

    this->content_render_color = new_color;

    // Content is dirty after reset
    this->content_dirty = true;

    if (TEST_MODE) std::cout << "Textbox: " << this->content << "new color by palette switch setted with palette linking off\n" << std::endl;
}


// For textboxes with colors, passed by palette

void My_SDL_textbox::set_content_color_if_palette_switched(SDL_Color new_color)
{
    // Safely check of equivalence
    if (this->content_render_color.r == new_color.r &&
        this->content_render_color.g == new_color.g &&
        this->content_render_color.b == new_color.b &&
        this->content_render_color.a == new_color.a)
    {
        return;     // Return if nothing changed
    }

    // No switch!!!
    // this->passed_by_palette = false;

    this->content_render_color = new_color;

    // Content is dirty after reset
    this->content_dirty = true;

    if (TEST_MODE) std::cout << "Textbox: " << this->content << "new color by palette switch setted\n" << std::endl;
}



void My_SDL_textbox::set_content_texture(SDL_Texture* new_texture)
{
    if (this->content_texture)
        SDL_DestroyTexture(this->content_texture);

    this->passed_by_palette = false;

    this->content_texture = new_texture;

    // Content is dirty after reset
    this->content_dirty = true;
}



void My_SDL_textbox::reset_colors_if_palette_switched()
{
    // Pass case

    if (!this->passed_by_palette || !App_palette.get_palette_reset_flag()) return;

    // Renew case 
    
    //     !!!    Elements with dynamic content color textboxes  (like buttons) should          !!!
    //     !!!    call switch_passed_by_palette_flag(false) for inner textbox                   !!!
    //     !!!    and switch content color by the pallet or not by the palette by themselves    !!!
    //     !!!    for double calls protection                                                   !!!

    if (TEST_MODE) std::cout << "Textbox: " << this->content << "started reset color by palette switch\n" << std::endl;


    set_content_color_if_palette_switched(App_palette.get_current_palette().basic_content_color);

}



void My_SDL_textbox::switch_blinking_mode_flag(bool new_flag)
{
    if (this->blinking_mode_context.blinking_mode_on)
    {
        if (new_flag) return;
        else
        {
            // Reset values to basic
            this->blinking_mode_context.active_now = true;
        }
    }

    this->blinking_mode_context.blinking_mode_on = new_flag;
}



void My_SDL_textbox::set_blinking_period(Uint64 new_blinking_period)
{
    bool tmp_mode_on_flag;


    if (this->blinking_mode_context.blinking_mode_on) tmp_mode_on_flag = true;

    else tmp_mode_on_flag = false;


    // Stop anyway and reset values to default
    this->blinking_mode_context.blinking_mode_on = false;


    this->blinking_mode_context.active_now = true;


    // Set new value
    this->blinking_mode_context.blinking_period = new_blinking_period;

    // Recalculate active time value
    this->blinking_mode_context.active_time = static_cast<Uint64>(std::round(

        this->blinking_mode_context.blinking_period * 
        this->blinking_mode_context.blinking_duty

    ));

    // Phase reset
    this->blinking_mode_context.phase = 0;

    // Set temped flag value
    this->blinking_mode_context.blinking_mode_on = tmp_mode_on_flag;
}


void My_SDL_textbox::set_blinking_duty(float new_blinking_duty)
{
    bool tmp_mode_on_flag;


    if (this->blinking_mode_context.blinking_mode_on) tmp_mode_on_flag = true;

    else tmp_mode_on_flag = false;


    // Stop anyway and reset values to default
    this->blinking_mode_context.blinking_mode_on = false;


    this->blinking_mode_context.active_now = true;


    // Set new value
    this->blinking_mode_context.blinking_duty = new_blinking_duty;

    // Recalculate active time value
    this->blinking_mode_context.active_time = static_cast<Uint64>(std::round(

        this->blinking_mode_context.blinking_period * 
        this->blinking_mode_context.blinking_duty

    ));

    // Phase reset
    this->blinking_mode_context.phase = 0;

    // Set temped flag value
    this->blinking_mode_context.blinking_mode_on = tmp_mode_on_flag;
}


void My_SDL_textbox::reset_anchor_points()
{
    // Uses current crop to set the current anchor points

    // Always the same rounding accuracy, because we work with crop map in initial scale and new scalers
    int half_w = static_cast<int>(std::round(static_cast<float>(this->content_width) * 0.5));
    int half_h = static_cast<int>(std::round(static_cast<float>(this->content_height) * 0.5));


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


void My_SDL_textbox::update_content_texture(SDL_Renderer* renderer, SDL_Color new_color)
{
    if (!this->content_dirty) return;


    if (!this->ttf_font_link)
    {
        std::cerr << "Font not initialized\n";
        this->content_dirty = false;   // FIX
        return;
    }

    if (content.empty())
    {
        if (content_texture)
        {
            SDL_DestroyTexture(content_texture);
            content_texture = nullptr;
        }

        this->content_dirty = false;
        return;
    }

    if (this->content_texture)
    {
        SDL_DestroyTexture(this->content_texture);
        this->content_texture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(
        this->ttf_font_link,
        this->content.c_str(),
        0,
        new_color
    );

    if (!surface)
    {
        this->content_dirty = false; // FIX
        return;
    }

    this->content_texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    // Renew sizes before render (autoupdated on the 1st step after init)
    if (this->content_dirty || this->textbox_sizes_initiation_update_flag)
    {

        if (TEST_MODE) std::cout << "Try to update textbox: " << this->content << "sizes" << "\n" << std::endl;

        int w = 0, h = 0; 
    
        if (this->ttf_font_link != nullptr && !this->content.empty())
        {
            if (!TTF_GetStringSize(this->ttf_font_link, this->content.c_str(), this->content.length(), &w, &h))
            { 
                std::cerr << "TTF_GetStringSize failed\n";
        
                this->content_dirty = false; 
                
                return; 
            } 
            
        }
    
        this->content_width = w; 
        this->content_height = h;

        
        reset_anchor_points();

        // Block the next step autoupdate
        this->textbox_sizes_initiation_update_flag = false;

        if (TEST_MODE) std::cout << "Textbox: " << this->content << "sizes updated: " << this->content_width << "x" << this->content_height << "\n" << std::endl;
    }

    this->content_dirty = false;
}


void My_SDL_textbox::text_draw(SDL_Renderer* renderer)
{
    if (this->content_texture)
    {
        SDL_FRect dst;

        dst.w = this->content_width;
        dst.h = this->content_height;

        dst.x = this->x_render_point - dst.w / 2.0f;
        dst.y = this->y_render_point - dst.h / 2.0f;

        SDL_RenderTexture(renderer, content_texture, nullptr, &dst);
    }
}

// =========================================================================================== GUI
