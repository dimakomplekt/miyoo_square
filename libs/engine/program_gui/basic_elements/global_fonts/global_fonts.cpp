// global_fonts.cpp

// =========================================================================================== IMPORT

#include "global_fonts.h"

// =========================================================================================== IMPORT


// =========================================================================================== APP FONTS

// ===== Palette 1 =====

static app_fonts_palette_ctx make_fonts_palette_1()
{
    app_font_ctx font_1;
    app_font_ctx emoji_font_1;

    font_1.font_path = absolute_by_relative_from_exe("content/ttf_fonts/Miracode.ttf");
    emoji_font_1.font_path = absolute_by_relative_from_exe("content/ttf_fonts/NotoEmoji.ttf");
    
    app_fonts_palette_ctx palette;

    palette.header_1_font = font_1;
    palette.header_2_font = font_1;
    palette.header_3_font = font_1;
    palette.ordinary_text_font = font_1;
    palette.small_text_font = font_1;
    palette.button_text_font = font_1;
    palette.emoji_text_font = emoji_font_1;

    return palette;
}

// ===== Palette 1 =====


// ===== Palette 2 =====

static app_fonts_palette_ctx make_fonts_palette_2()
{
    app_font_ctx font_2;
    app_font_ctx emoji_font_2;

    font_2.font_path = absolute_by_relative_from_exe("content/ttf_fonts/basis33.ttf");
    emoji_font_2.font_path = absolute_by_relative_from_exe("content/ttf_fonts/NotoEmoji.ttf");

    app_fonts_palette_ctx palette;

    palette.header_1_font = font_2;
    palette.header_2_font = font_2;
    palette.header_3_font = font_2;
    palette.ordinary_text_font = font_2;
    palette.small_text_font = font_2;
    palette.button_text_font = font_2;
    palette.emoji_text_font = emoji_font_2;

    return palette;
}

// ===== Palette 2 =====


// ===== Palette 3 =====

static app_fonts_palette_ctx make_fonts_palette_3()
{
    app_font_ctx font_3;
    app_font_ctx emoji_font_3;

    font_3.font_path = absolute_by_relative_from_exe("content/ttf_fonts/MidnightLetters.ttf");
    emoji_font_3.font_path = absolute_by_relative_from_exe("content/ttf_fonts/NotoEmoji.ttf");

    app_fonts_palette_ctx palette;

    palette.header_1_font = font_3;
    palette.header_2_font = font_3;
    palette.header_3_font = font_3;
    palette.ordinary_text_font = font_3;
    palette.small_text_font = font_3;
    palette.button_text_font = font_3;
    palette.emoji_text_font = emoji_font_3;

    return palette;
}

// ===== Palette 3 =====



// =========================================================================================== APP FONTS


// =========================================================================================== APP FONTS PALETTE SINGLETON


Global_fonts& Global_fonts::Instance()
{
    static Global_fonts instance;

    static bool initialized = false;

    if (!initialized)
    {
        // Palettes initialization zone
        instance.add_fonts_palette(make_fonts_palette_1());
        instance.add_fonts_palette(make_fonts_palette_2());
        instance.add_fonts_palette(make_fonts_palette_3());

        initialized = true;
    }

    return instance;
}


Global_fonts::Global_fonts()
{
    this->current_fonts_palette_id = 0;

    this->fonts_palette_init_flag = true;
    this->fonts_palette_reset_flag_loops_counter = 0;


    // Constructor is private and only called once by Instance().
    // No dynamic memory allocation needed here.
}


// ===== Palette workflow =====

void Global_fonts::fonts_management_in_update_loop()
{
    // Reset flags or check

    // Go through the palettes list and initialize / reinitialize the SDL_TTF fonts if the palette reset flag is set
    if (this->fonts_palette_init_flag)
    {
        for (app_fonts_palette_ctx& palette : this->fonts_palettes_list)
        {
            // Counter for further logic switch by UI textbox type
            this->fonts_palette_reset_flag = true;
            int iterator_font_counter = 0;

            // Initialize / reinitialize SDL_TTF fonts for the palette
            for (app_font_ctx* font_ctx : {

                &palette.header_1_font, 
                &palette.header_2_font, 
                &palette.header_3_font, 
                &palette.ordinary_text_font,
                &palette.small_text_font,
                &palette.button_text_font,
                &palette.emoji_text_font

            })
            {
                if (font_ctx->ttf_font_link)
                {
                    TTF_CloseFont(static_cast<TTF_Font*>(font_ctx->ttf_font_link));
                    // font_ctx->ttf_font_link = nullptr; - MAYBE DESTRUCT THE LOGIC
                }


                if (!font_ctx->font_path.empty())
                {

                    // ====== FONT INITIALIZATION ======

                    // Calls before any state machine cycle comands - guarantees the valid use of the palette font inside the program code
                    switch (iterator_font_counter)
                    {
                        // header_1
                        case 0:

                            font_ctx->font_size = 48;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);

                            iterator_font_counter++;

                            break;
                            

                        // header_2
                        case 1:

                            font_ctx->font_size = 36;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);

                            iterator_font_counter++;

                            break;
                        

                        // header_3
                        case 2:

                            font_ctx->font_size = 24;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);

                            iterator_font_counter++;

                            break;


                        // ordinary text
                        case 3:

                            font_ctx->font_size = 18;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);

                            iterator_font_counter++;

                            break;
                            

                        // small text
                        case 4:

                            font_ctx->font_size = 12;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);

                            iterator_font_counter++;

                            break;


                        // button text
                        case 5:

                            font_ctx->font_size = 18;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);
                            iterator_font_counter++;
                            
                            break;

                        // emoji text
                        case 6:

                            font_ctx->font_size = 24;
                            font_ctx->ttf_font_link = TTF_OpenFont(font_ctx->font_path.c_str(), font_ctx->font_size);
                            iterator_font_counter = 0;
                            
                            break;
                        

                        default: break;
                    }
                   

                    // ====== FONT INITIALIZATION ======

                    if (!font_ctx->ttf_font_link)
                    {
                        SDL_Log("TTF_OpenFont failed for font path %s: %s", font_ctx->font_path.c_str(), SDL_GetError());
                    }
                }
            }
        }

        this->fonts_palette_init_flag = false;
    }
}


void Global_fonts::add_fonts_palette(const app_fonts_palette_ctx& new_fonts_palette)
{
    this->fonts_palettes_list.push_back(new_fonts_palette);
}



const app_fonts_palette_ctx& Global_fonts::get_current_fonts_palette() const
{
    return this->fonts_palettes_list[this->current_fonts_palette_id];
}



void Global_fonts::switch_to_the_next_font_palette()
{
    if (this->current_fonts_palette_id + 1 < this->fonts_palettes_list.size())
    {
        this->current_fonts_palette_id++;
    }
    else
    {
        this->current_fonts_palette_id = 0;
    }

    this->fonts_palette_reset_flag = true;
    this->fonts_palette_reset_flag_loops_counter = 0;
}


void Global_fonts::switch_to_the_fonts_palette(unsigned int palette_id)
{
    if (palette_id < this->fonts_palettes_list.size())
    {
        this->current_fonts_palette_id = palette_id;

        this->fonts_palette_reset_flag = true;
        this->fonts_palette_reset_flag_loops_counter = 0;
    }
}


// ===== Palette reset =====

bool Global_fonts::get_fonts_palette_reset_flag() const
{
    return this->fonts_palette_reset_flag;
}


// Function, called inside the main loop, before state loop
// sets palette_reset_flag to false after 2 loops, to avoid constant resettings.
// Called inside SDL_app_cycle(SDL_app_ctx* app) in app.cpp
void Global_fonts::fonts_palette_reset_flag_state_loop_update()
{
    // If the flag is set, we need to track how many loops have passed
    if (fonts_palette_reset_flag)
    {
        fonts_palette_reset_flag_loops_counter++;

        // After 2 loops, reset the flag to avoid constant resetting
        if (fonts_palette_reset_flag_loops_counter >= 2)
        {
            fonts_palette_reset_flag = false;
            fonts_palette_reset_flag_loops_counter = 0; // Reset counter for next time
        }
    }
}
