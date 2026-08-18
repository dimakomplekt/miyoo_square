// 1.2_MASKS_SETUP.cpp


// =========================================================================================== IMPORT

#include "1.2_MASKS_SETUP.h"


#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"

#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"


// Predeclare for switching states
#include "../../program_states.h"

#include "../../../app.h"

// Log
#include <iostream>


// Check
#include "../1.1_FILE_CHOOSE/1.1_FILE_CHOOSE.h"

// =========================================================================================== IMPORT


// =========================================================================================== STATE DATA

// RAII + lifecycle management

My_SDL_panel* Masks_setup_panel = nullptr;

My_SDL_textbox* Masks_setup_textbox = nullptr;

// =========================================================================================== STATE DATA


// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION

void masks_setup_elements_create();

void masks_setup_elements_setup();

void masks_setup_elements_free_and_nullptr();

void masks_setup_elements_update();

void reset_passed_by_dictionary_textboxes_if_language_switched_ms();

void masks_setup_actions();

void masks_setup_elements_render(SDL_Renderer* renderer);

// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


// =========================================================================================== MAIN STATE API


void masks_setup_enter()
{
    // Log the enter in console
    std::cout << "Entering MASKS_SETUP\n"; 

    // ===== State allocation =====

    masks_setup_elements_create();


    // !!! WARNING !!!
    //
    // CREATE MATs and texture which 
    // should be deallocated at the 
    // state 1.4
    //
    // !!! WARNING !!!

    opencv_global_setup();

    // ===== State allocation =====


    // Elements setup

    masks_setup_elements_setup();

}



void masks_setup_exit()
{
    // ===== State deallocation =====

    masks_setup_elements_free_and_nullptr();

    // ===== State deallocation =====


    // Log the exit in console

    std::cout << "Exiting MASKS_SETUP\n"; 

}


void masks_setup_update()
{
    // Update inputs
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {
        App_inputs.update();
    }


    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {

        masks_setup_elements_update();

        masks_setup_actions();

    }
}


void masks_setup_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        masks_setup_elements_render(renderer);
    }
}




// =========================================================================================== MAIN STATE API




// =========================================================================================== INNER STATE FUNCTIONS


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void masks_setup_elements_create()
{
    // Masks_setup panel create
    Masks_setup_panel = new My_SDL_panel();

    // Masks_setup textbox create
    Masks_setup_textbox = new My_SDL_textbox();
}


void masks_setup_elements_setup()
{
    // Masks_setup panel setup

    Masks_setup_panel->set_render_point(MAIN_WINDOW_H_SIZE / 2, MAIN_WINDOW_V_SIZE / 2);
    Masks_setup_panel->set_size(MAIN_WINDOW_H_SIZE, MAIN_WINDOW_V_SIZE);
    Masks_setup_panel->set_border_radius(0);

    // Masks_setup textbox setup
    Masks_setup_textbox->switch_textbox_type(HEADER_2);

    Masks_setup_textbox->set_content(str_by_dictionary(gd_masks_setup_info));

    Masks_setup_textbox->switch_blinking_mode_flag(true);



    // Put the texture inside the middle of the panel

    Masks_setup_panel->add_element(

        Masks_setup_textbox,
         (Masks_setup_panel->get_width_size()) * 0.5,
          (Masks_setup_panel->get_height_size()) * 0.5,
           1
    );

}


void masks_setup_elements_free_and_nullptr()
{
    // Free all elements

    Masks_setup_panel->delete_element();

    // Nullptr the pointers

    Masks_setup_panel = nullptr;
    Masks_setup_textbox = nullptr;
    
}


void masks_setup_elements_update()
{
    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched_ms();

    // Update all elements
    Masks_setup_panel->update();

}


void reset_passed_by_dictionary_textboxes_if_language_switched_ms()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        Masks_setup_textbox->set_content(str_by_dictionary(gd_masks_setup_info));
    }
}


void masks_setup_actions()
{
    // Switch the state to EXIT if EXIT pressed

    if (App_inputs.is_just_released(Key_actions::EXIT))
    {
        this_app.app_sm.request_state_change(FILE_CHOOSE_ID);
    }

    // Switch the state to MAIN MENU if ENTER pressed

    if (App_inputs.is_just_released(Key_actions::ENTER))
    {
        this_app.app_sm.request_state_change(MASKS_SETUP_ID_1);
    }

}


void masks_setup_elements_render(SDL_Renderer* renderer)
{
    // Render all elements
    Masks_setup_panel->render(renderer);

}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION



// =========================================================================================== OPENCV PART OF THE STATE

// ===== DATA =====

cv::VideoCapture* video_capture_device_global = nullptr;

cv::Mat* video_cv_mat_mask_1_global = nullptr;
cv::Mat* video_cv_mat_mask_2_global = nullptr;
cv::Mat* video_cv_mat_mask_3_global = nullptr;

SDL_Texture* translated_texture_global = nullptr;

bool opencv_pipeline_reset_global = false;

opencv_update_ctx opencv_global_update_ctx;

// ===== DATA =====


// ===== Functions =====

void opencv_global_setup()
{
    // Block of the reinit
    if (opencv_pipeline_reset_global != true)
    {
        video_cv_mat_mask_1_global = new cv::Mat();
        video_cv_mat_mask_2_global = new cv::Mat();
        video_cv_mat_mask_3_global = new cv::Mat();

        // Will be reseted (for size correction) at the setup 
        // of each inner state (1.2.1  1.2.6)

        // Basic setups for state 1 size-s
        translated_texture_global = SDL_CreateTexture(

            this_app.renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STREAMING, 
            files_metadata.video_1_data.width, 
            files_metadata.video_1_data.height

        );

        opencv_pipeline_reset_global = true;
    }

    // TEXTURE WORKFLOW for inner states

    /*

        // 1. FREE THE MEMORY

        if (translated_texture_global != nullptr) {
            SDL_DestroyTexture(translated_texture_global);
            translated_texture_global = nullptr;
        }


        // 2. CREATE NEW

        translated_texture_global = SDL_CreateTexture(
            this_app.renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STREAMING, 
            new_width,  // Новый размер
            new_height  // Новый размер
        );

        // 3. CHECK BEFORE USE
        
        if (translated_texture_global == nullptr) {
            // Обработка ошибки: SDL_GetError()
        }
    
    */


    if (TEST_MODE) std::cout << "Mats and texture created\n" << std::endl;   
}


void switch_video(const std::string& new_file_path) 
{
    // 1. DELETE old capture device if video is opened
    if (video_capture_device_global != nullptr)
    {
        if (video_capture_device_global->isOpened())
        {
            // Close file
            video_capture_device_global->release();
        }

        // Free the memory 
        delete video_capture_device_global;
        
        // Free the pointer
        video_capture_device_global = nullptr;
    }

    // 2. Create new capture device
    video_capture_device_global = new cv::VideoCapture(new_file_path);

    // 3. Check
    if (!video_capture_device_global->isOpened())
    {
        std::cerr << "Error: Could not open video " << new_file_path << std::endl;
    } 
    else if (TEST_MODE)
    {
        std::cout << "Successfully switched to: " << new_file_path << "\n" << std::endl;
    }


    // 4. Update metadata and starting point

    // Set the 0 index
    opencv_global_update_ctx.current_frame_index = 0;
    // Get total frame count
    opencv_global_update_ctx.total_frame_count = static_cast<int>(video_capture_device_global->get(cv::CAP_PROP_FRAME_COUNT));
        

}


// Helpers for big screen translation

void kingsize_window_init(cv::Mat* mat)
{
    cv::namedWindow(
        "KINGSIZE",
        cv::WINDOW_NORMAL
    );

    cv::resizeWindow(
        "KINGSIZE",
        mat->cols * 3,
        mat->rows * 3
    );
}


void kingsize_window_close()
{
    cv::destroyWindow("KINGSIZE");
}


void opencv_global_update()
{

    // ===== PREPROCESSING =====

    // Current data to work with

    std::string file_path;

    cv::Mat* current_basic_mat_to_show;

    // Which file
    switch (opencv_global_update_ctx.current_file_for_mask_setup)
    {
        case (FILE_1_CF):
        {
            file_path =  file_choose_info.file_1_path;
            break;
        }

        case (FILE_2_CF):
        {
            file_path =  file_choose_info.file_2_path;
            break;
        }

        case (FILE_3_CF):
        {
            file_path =  file_choose_info.file_3_path;
            break; 
        }

        case (FILE_4_CF):
        {
            file_path =  file_choose_info.file_4_path;
            break;
        }

        case (FILE_5_CF):
        {
            file_path =  file_choose_info.file_5_path;
            break;
        }

        case (FILE_6_CF):
        {
            file_path =  file_choose_info.file_6_path;
            break;
        }

        default: break;
    }

    // Which mask
    switch (opencv_global_update_ctx.current_mask_for_mask_setup)
    {
        case (MASK_1_CM):
        {
            current_basic_mat_to_show = video_cv_mat_mask_1_global;
            break;
        }

        case (MASK_2_CM):
        {
            current_basic_mat_to_show = video_cv_mat_mask_2_global;
            break;
        }

        case (MASK_3_CM):
        {
            current_basic_mat_to_show = video_cv_mat_mask_3_global;
            break;
        }

        default: break;
    }


    if (current_basic_mat_to_show == nullptr)
    {
        std::cerr << "ERROR: current_basic_mat_to_show == nullptr\n";
        return;
    }


    // By container, setted at the inner state start (1.2.1 - 1.2.6)
    My_SDL_texture* my_texture = opencv_global_update_ctx.current_texture_container;

    // ===== PREPROCESSING =====


    // ===== DEFAULT TRANSLATION LOGIC =====

    // First call at the state start
    // we need to create new VCD

    if (opencv_global_update_ctx.need_reset)
    {
        // USE HELPER
        switch_video(file_path);

        // Block reinits after reset
        opencv_global_update_ctx.need_reset = false;
    }

    // Play logic (nothing at pause)
    if (opencv_global_update_ctx.playback_state == VIDEO_PLAYING_VPS)
    {
        // Show from the start (if it's 1st call)
        if (current_basic_mat_to_show->empty())
        {
            opencv_global_update_ctx.current_frame_index = 0;
        }
    
        // Rewind to current frame
        video_capture_device_global->set(
            cv::CAP_PROP_POS_FRAMES,
            opencv_global_update_ctx.current_frame_index
        );
    
        // Read current frame
        *video_capture_device_global >> *current_basic_mat_to_show;
    
        // Move to the next frame for the next update()
        opencv_global_update_ctx.current_frame_index++;
    }
    else
    {
        // Need to be here to save order read - move inside play state
        video_capture_device_global->set(
            cv::CAP_PROP_POS_FRAMES,
            opencv_global_update_ctx.current_frame_index
        );

        // Read current frame
        *video_capture_device_global >> *current_basic_mat_to_show;
    }


    if (TEST_MODE) std::cout << "Capture passed to MAT!\n" << std::endl;

    // ===== DEFAULT TRANSLATION LOGIC =====


    // ===== BLACKBOX WITH PROCESSING LOGIC BY CALLBACK =====

    if (opencv_global_update_ctx.current_frame_processor != nullptr)
    {
        if (current_basic_mat_to_show != nullptr)
            // CALL A CALLBACK FOR CURRENT MAT
            // THE RENDERER will show the video after processing
            opencv_global_update_ctx.current_frame_processor(current_basic_mat_to_show);
    }

    // ===== BLACKBOX WITH PROCESSING LOGIC BY CALLBACK =====


    // ===== SHOW SCALED COPY OF CURRENT MAT INSIDE OTHER WINDOW =====

    // If frame is empty, skip this update
    if (!current_basic_mat_to_show->empty())
    {
        if (opencv_global_update_ctx.show_kingsize)
        {
            // First call after activation
            if (!opencv_global_update_ctx.kingsize_live_transmission)
            {
                kingsize_window_init(current_basic_mat_to_show);

                // Block reinit
                opencv_global_update_ctx.kingsize_live_transmission = true;
            }


            // USER INPUT ERROR HANDLER
            // Check whether user closed the window manually
            if (cv::getWindowProperty("KINGSIZE", cv::WND_PROP_VISIBLE) < 1)
            {
                opencv_global_update_ctx.show_kingsize = false;
                opencv_global_update_ctx.kingsize_live_transmission = false;

                return;
            }


            // Create scaled copy
            cv::Mat kingsize_mat;

            cv::resize(
                *current_basic_mat_to_show,
                kingsize_mat,
                cv::Size(
                    current_basic_mat_to_show->cols * 3,
                    current_basic_mat_to_show->rows * 3
                ),
                0,
                0,
                cv::INTER_NEAREST
            );

            // Show scaled image
            cv::imshow(
                "KINGSIZE",
                kingsize_mat
            );
        }
        else
        {
            // Close window after deactivation
            if (opencv_global_update_ctx.kingsize_live_transmission)
            {
                kingsize_window_close();

                // Allow init on next activation
                opencv_global_update_ctx.kingsize_live_transmission = false;
            }
        }
    }

    // ===== SHOW SCALED COPY OF CURRENT MAT INSIDE OTHER WINDOW =====


    // ===== TRANSLATION FROM OPENCV TO SDL =====

    // Renew basic texture sizes
    if (translated_texture_global != nullptr) 
    {
        
        if (TEST_MODE) std::cout << "OPENCV - SDL translation started!\n" << std::endl;


        translate_cv_mat_to_sdl_texture(

            current_basic_mat_to_show,
            translated_texture_global,
            this_app.renderer

        );


        if (TEST_MODE) std::cout << "OPENCV - SDL translation ended!\n" << std::endl;

        if (TEST_MODE) std::cout << "Start texture pass to My_SDL_Texture!\n" << std::endl;


        int target_width = my_texture->get_width_size();
        int target_height = my_texture->get_height_size();

        // Renew my_SDL_texture by link to curr texture with target resize
        my_texture->set_texture(translated_texture_global, false);

        if (TEST_MODE) std::cout << "Texture pass to My_SDL_Texture complete!\n" << std::endl;


        my_texture->set_size(target_width, target_height);

        if (TEST_MODE) std::cout << "Texture sizes changed to target inside My_SDL_Texture!\n" << std::endl;


        SDL_SetTextureBlendMode(translated_texture_global, SDL_BLENDMODE_NONE);
    }

    // ===== TRANSLATION FROM OPENCV TO SDL =====
}


void opencv_global_free_and_nullptr()
{
    // 1. Clear mats

    if (video_cv_mat_mask_1_global != nullptr)
    {
        delete video_cv_mat_mask_1_global;

        video_cv_mat_mask_1_global = nullptr; 
    }

    if (video_cv_mat_mask_2_global != nullptr)
    {
        delete video_cv_mat_mask_2_global;

        video_cv_mat_mask_2_global = nullptr; 
    }

    if (video_cv_mat_mask_3_global != nullptr)
    {
        delete video_cv_mat_mask_3_global;

        video_cv_mat_mask_3_global = nullptr; 
    }


    // 2. Clear texture SDL: by inner destroy function
    if (translated_texture_global != nullptr)
    {
        SDL_DestroyTexture(translated_texture_global); // Clear VRAM и RAM

        translated_texture_global = nullptr; 
    }

    // Activate reinit
    opencv_pipeline_reset_global = false;
}


// ===== Functions =====

// =========================================================================================== OPENCV PART OF THE STATE




// =========================================================================================== ADDITIONAL STATES API


void masks_setup_2_enter()
{
    //
}
void masks_setup_2_exit()
{
    //
}

void masks_setup_2_update()
{
    //
}
void masks_setup_2_render(SDL_Renderer* renderer)
{
    //
}


void masks_setup_3_enter()
{
    //
}
void masks_setup_3_exit()
{
    //
}

void masks_setup_3_update()
{
    //
}
void masks_setup_3_render(SDL_Renderer* renderer)
{
    //
}


void masks_setup_4_enter()
{
    //
}
void masks_setup_4_exit()
{
    //
}

void masks_setup_4_update()
{
    //
}
void masks_setup_4_render(SDL_Renderer* renderer)
{
    //
}


void masks_setup_5_enter()
{
    //
}
void masks_setup_5_exit()
{
    //
}

void masks_setup_5_update()
{
    //
}
void masks_setup_5_render(SDL_Renderer* renderer)
{
    //
}


void masks_setup_6_enter()
{
    //
}
void masks_setup_6_exit()
{
    //
}

void masks_setup_6_update()
{
    //
}
void masks_setup_6_render(SDL_Renderer* renderer)
{
    //
}


// =========================================================================================== ADDITIONAL STATES API
