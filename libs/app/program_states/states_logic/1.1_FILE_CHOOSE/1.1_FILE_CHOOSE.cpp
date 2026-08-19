// 1.1_FILE_CHOOSE.cpp


// =========================================================================================== IMPORT

#include "1.1_FILE_CHOOSE.h"


#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_textbox/my_sdl_textbox.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_panel/my_sdl_panel.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_button/my_sdl_button.h"
#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_texture/my_sdl_texture.h"


// Predeclare for switching states
#include "../../program_states.h"

#include "../../../app.h"

// Log
#include <iostream>


// System file manager
#include <windows.h>
#include <commdlg.h>

// =========================================================================================== IMPORT


// =========================================================================================== STATE DATA

// RAII + lifecycle management

// Background
My_SDL_panel* File_choose_background_panel = nullptr;

// File choose panel (5 elements now)


My_SDL_panel* File_choose_panel = nullptr;

// Choose video file
My_SDL_panel* File_1_panel = nullptr;
My_SDL_textbox* File_1_textbox = nullptr;
My_SDL_button* File_1_button = nullptr;

// Choose video file meta-file (.txt with setted format)
My_SDL_panel* File_1_data_panel = nullptr;
My_SDL_textbox* File_1_data_textbox = nullptr;
My_SDL_button* File_1_data_button = nullptr;


My_SDL_panel* File_2_panel = nullptr;
My_SDL_textbox* File_2_textbox = nullptr;
My_SDL_button* File_2_button = nullptr;

My_SDL_panel* File_2_data_panel = nullptr;
My_SDL_textbox* File_2_data_textbox = nullptr;
My_SDL_button* File_2_data_button = nullptr;


My_SDL_panel* File_3_panel = nullptr;
My_SDL_textbox* File_3_textbox = nullptr;
My_SDL_button* File_3_button = nullptr;

My_SDL_panel* File_3_data_panel = nullptr;
My_SDL_textbox* File_3_data_textbox = nullptr;
My_SDL_button* File_3_data_button = nullptr;


My_SDL_panel* File_4_panel = nullptr;
My_SDL_textbox* File_4_textbox = nullptr;
My_SDL_button* File_4_button = nullptr;

My_SDL_panel* File_4_data_panel = nullptr;
My_SDL_textbox* File_4_data_textbox = nullptr;
My_SDL_button* File_4_data_button = nullptr;


My_SDL_panel* File_5_panel = nullptr;
My_SDL_textbox* File_5_textbox = nullptr;
My_SDL_button* File_5_button = nullptr;

My_SDL_panel* File_5_data_panel = nullptr;
My_SDL_textbox* File_5_data_textbox = nullptr;
My_SDL_button* File_5_data_button = nullptr;


My_SDL_panel* File_6_panel = nullptr;
My_SDL_textbox* File_6_textbox = nullptr;
My_SDL_button* File_6_button = nullptr;

My_SDL_panel* File_6_data_panel = nullptr;
My_SDL_textbox* File_6_data_textbox = nullptr;
My_SDL_button* File_6_data_button = nullptr;


// Preview panel

My_SDL_panel* Video_panel = nullptr;
My_SDL_textbox* PSMVA_textbox_fc = nullptr;
My_SDL_texture* File_preview_texture = nullptr;


// State control button

My_SDL_button* Main_menu_button = nullptr;
My_SDL_button* Study_start_button = nullptr;


// =========================================================================================== STATE DATA


// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION

void file_choose_elements_create();

void file_choose_elements_setup();

void file_choose_elements_free_and_nullptr();

void file_choose_elements_update();

void reset_passed_by_dictionary_textboxes_if_language_switched_fc();

void file_choose_actions();

void file_choose_elements_render(SDL_Renderer* renderer);

std::string file_name_from_path(std::string path);

// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


// =========================================================================================== OPENCV PART OF THE STATE

// ===== Data =====

// Global capture
cv::VideoCapture* video_capture_device = nullptr; 

// Global MAT for capture frames
cv::Mat* test_cv_mat = nullptr;

// Global texture for test_cv_mat_translation
SDL_Texture* translated_opencv_mat_texture = nullptr; 

bool reset_opencv_data = false;


// ===== Functions =====

void opencv_setup_fc();

void opencv_update_fc();

void opencv_render_by_translator_fc(SDL_Renderer* renderer);

void file_choose_opencv_free_and_nullptr();

// =========================================================================================== OPENCV PART OF THE STATE



// =========================================================================================== MAIN STATE API


void file_choose_enter()
{
    // Log the enter in console
    std::cout << "Entering FILE_CHOOSE\n"; 

    // Check after new enter
    if (TEST_MODE)
    {
        // std::cout << file_choose_info.file_1_path << "\n" << std::endl;
        // std::cout << file_choose_info.file_2_path << "\n" << std::endl;
        // std::cout << file_choose_info.file_3_path << "\n" << std::endl;
        // std::cout << file_choose_info.file_4_path << "\n" << std::endl;
        // std::cout << file_choose_info.file_5_path << "\n" << std::endl;
        // std::cout << file_choose_info.file_6_path << "\n" << std::endl;
        // std::cout << static_cast<int>(file_choose_info.panels_states.file_1_panel_state) << "\n" << std::endl;
        // std::cout << static_cast<int>(file_choose_info.panels_states.file_2_panel_state) << "\n" << std::endl;
        // std::cout << static_cast<int>(file_choose_info.panels_states.file_3_panel_state) << "\n" << std::endl;
        // std::cout << static_cast<int>(file_choose_info.panels_states.file_4_panel_state) << "\n" << std::endl;
        // std::cout << static_cast<int>(file_choose_info.panels_states.file_5_panel_state) << "\n" << std::endl;
        // std::cout << static_cast<int>(file_choose_info.panels_states.file_6_panel_state) << "\n" << std::endl;


        std::cout << "Video 1 bit depth: " << files_metadata.video_1_data.bit_depth << ".\n" << std::endl;
        std::cout << "Video 1 frames quantity: " << files_metadata.video_1_data.frames_quantity << ".\n" << std::endl;
        std::cout << "Video 1 FPS: " << files_metadata.video_1_data.frames_per_second << ".\n" << std::endl;
        std::cout << "Video 1 frame time: " << files_metadata.video_1_data.frame_time << ".\n" << std::endl;
        std::cout << "Video 1 exposition time: " << files_metadata.video_1_data.exposition_time << ".\n" << std::endl;
        std::cout << "Video 1 width: " << files_metadata.video_1_data.width << ".\n" << std::endl;
        std::cout << "Video 1 height: " << files_metadata.video_1_data.height << ".\n" << std::endl;
        std::cout << "Video 1 status: " << static_cast<int>(files_metadata.video_1_data.status) << ".\n" << std::endl;
    }


    // ===== State allocation =====

    file_choose_elements_create();

    // ===== State allocation =====


    // File choose data init

    file_choose_data_init();

    // Elements setup

    file_choose_elements_setup();

}



void file_choose_exit()
{
    // ===== State deallocation =====

    file_choose_opencv_free_and_nullptr();

    file_choose_elements_free_and_nullptr();


    // ===== State deallocation =====


    // Log the exit in console

    std::cout << "Exiting FILE_CHOOSE\n"; 

}


void file_choose_update()
{

    // ===== OPENCV =====

    // Max speed update not to loose synchronization
    // TODO: maybe block max speed by timer?
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_30))
    {
        opencv_update_fc();
    }

    // ===== OPENCV =====


    // Update inputs
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {

        App_inputs.update();

    }


    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {

        file_choose_elements_update();

        file_choose_actions();

    }
}


void file_choose_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        file_choose_elements_render(renderer);


        // ===== OPENCV =====

        // ===== OPENCV =====
    }


    if (App_timer_1.can_execute(Execute_zone_ID::HZ_60))
    {
        opencv_render_by_translator_fc(renderer);
    }
}


// =========================================================================================== MAIN STATE API


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void file_choose_elements_create()
{
    // Background
    File_choose_background_panel = new My_SDL_panel;


    // File choose panel (5 elements now)

    File_choose_panel = new My_SDL_panel;


    File_1_panel = new My_SDL_panel;
    File_1_textbox = new My_SDL_textbox;
    File_1_button = new My_SDL_button;

    File_2_panel = new My_SDL_panel;
    File_2_textbox = new My_SDL_textbox;
    File_2_button = new My_SDL_button;

    File_3_panel = new My_SDL_panel;
    File_3_textbox = new My_SDL_textbox;
    File_3_button = new My_SDL_button;

    File_4_panel = new My_SDL_panel;
    File_4_textbox = new My_SDL_textbox;
    File_4_button = new My_SDL_button;

    File_5_panel = new My_SDL_panel;
    File_5_textbox = new My_SDL_textbox;
    File_5_button = new My_SDL_button;

    File_6_panel = new My_SDL_panel;
    File_6_textbox = new My_SDL_textbox;
    File_6_button = new My_SDL_button;


    File_1_data_panel = new My_SDL_panel;
    File_1_data_textbox = new My_SDL_textbox;
    File_1_data_button = new My_SDL_button;

    File_2_data_panel = new My_SDL_panel;
    File_2_data_textbox = new My_SDL_textbox;
    File_2_data_button = new My_SDL_button;

    File_3_data_panel = new My_SDL_panel;
    File_3_data_textbox = new My_SDL_textbox;
    File_3_data_button = new My_SDL_button;

    File_4_data_panel = new My_SDL_panel;
    File_4_data_textbox = new My_SDL_textbox;
    File_4_data_button = new My_SDL_button;

    File_5_data_panel = new My_SDL_panel;
    File_5_data_textbox = new My_SDL_textbox;
    File_5_data_button = new My_SDL_button;

    File_6_data_panel = new My_SDL_panel;
    File_6_data_textbox = new My_SDL_textbox;
    File_6_data_button = new My_SDL_button;

    // Preview panel

    Video_panel = new My_SDL_panel;
    PSMVA_textbox_fc = new My_SDL_textbox();
    File_preview_texture = new My_SDL_texture;


    // State control button
    Main_menu_button = new My_SDL_button;
    Study_start_button = new My_SDL_button;
}


// ===== SETUP DATA =====

// ===== Main sizes =====

// Static-size GUI

const int BACKGROUND_WIDTH  = MAIN_WINDOW_H_SIZE;
const int BACKGROUND_HEIGHT  = MAIN_WINDOW_V_SIZE;

const int SCREEN_MARGIN_1 = 50;
const int SCREEN_MARGIN_2 = 25;

const int FIRST_ZONE_HEIGHT = static_cast<int>((BACKGROUND_HEIGHT - 3 * SCREEN_MARGIN_1) * 0.65);
const int SECOND_ZONE_HEIGHT = static_cast<int>((BACKGROUND_HEIGHT - 3 * SCREEN_MARGIN_1) * 0.35);


int file_choose_panel_width = (BACKGROUND_WIDTH - 2 * SCREEN_MARGIN_1);
int file_choose_panel_height = FIRST_ZONE_HEIGHT;

int video_panel_width = static_cast<int>((BACKGROUND_WIDTH - 4 * SCREEN_MARGIN_1) * 0.5);
int video_panel_height = SECOND_ZONE_HEIGHT;

int state_buttons_width = static_cast<int>((BACKGROUND_WIDTH - 4 * SCREEN_MARGIN_1) * 0.25);
int state_buttons_height = SECOND_ZONE_HEIGHT;


int file_choose_mini_panels_width = static_cast<int>((file_choose_panel_width - 3 * SCREEN_MARGIN_2) * 0.5);
int file_choose_mini_panels_height = static_cast<int>((file_choose_panel_height - 7 * SCREEN_MARGIN_2) / 6);

int file_choose_buttons_width = file_choose_mini_panels_width * 0.2;
int file_choose_buttons_height = file_choose_mini_panels_height;


// ====== Main points ======

int file_choose_background_x = BACKGROUND_WIDTH * 0.5;
int file_choose_background_y = BACKGROUND_HEIGHT * 0.5;


int file_choose_panel_x = SCREEN_MARGIN_1 + file_choose_panel_width * 0.5;
int file_choose_panel_y = SCREEN_MARGIN_1 + FIRST_ZONE_HEIGHT * 0.5;


int video_panel_x = static_cast<int>(SCREEN_MARGIN_1 + video_panel_width * 0.5);
int video_panel_y = static_cast<int>(SCREEN_MARGIN_1 + FIRST_ZONE_HEIGHT + SCREEN_MARGIN_1 + SECOND_ZONE_HEIGHT * 0.5);


int main_menu_button_x = static_cast<int>(video_panel_x + video_panel_width * 0.5 + SCREEN_MARGIN_1 + state_buttons_width * 0.5); 
int main_menu_button_y = video_panel_y; 


int study_start_button_x = static_cast<int>(main_menu_button_x + SCREEN_MARGIN_1 + state_buttons_width);
int study_start_button_y = video_panel_y; 


// ===== SETUP DATA =====

// ===== CALLBACKS FOR BUTTONS =====

void file_choose_or_clear(int file_number);
void file_data_choose_or_clear(int file_number);

void back_to_main_menu();
void study_start();

bool check_start_study_access();

// ===== CALLBACKS FOR BUTTONS =====


void file_choose_elements_setup()
{     
    // ===== OPENCV =====

    opencv_setup_fc();

    // ===== OPENCV =====


    // ===== Setup =====

    // Background setup

    File_choose_background_panel->set_render_point(file_choose_background_x, file_choose_background_y);
    File_choose_background_panel->set_size(BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
    File_choose_background_panel->set_border_radius(0);
    

    // ===== File choose panel =====

    File_choose_panel->set_render_point(file_choose_panel_x, file_choose_panel_y);
    File_choose_panel->set_size(file_choose_panel_width, file_choose_panel_height);
    File_choose_panel->set_border_radius(0);


    // ===== 1 File choose panel =====

    // 1st file choose
    File_1_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_1_textbox->switch_textbox_type(HEADER_3);
    
    File_1_button->switch_button_textbox_type(HEADER_2);
    File_1_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_1_button->on_click = []() { file_choose_or_clear(1); };

    File_1_button->switch_push_mode();

    File_1_panel->set_border_radius(0);
    File_1_button->set_border_radius(0);


    // 1st file data choose
    File_1_data_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_1_data_textbox->switch_textbox_type(HEADER_3);
    
    File_1_data_button->switch_button_textbox_type(HEADER_2);
    File_1_data_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_1_data_button->on_click = []() { file_data_choose_or_clear(1); };

    File_1_data_button->switch_push_mode();

    File_1_data_panel->set_border_radius(0);
    File_1_data_button->set_border_radius(0);


    File_1_panel->add_element(

        File_1_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_1_panel->add_element(

        File_1_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );


    File_1_data_panel->add_element(

        File_1_data_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_1_data_panel->add_element(

        File_1_data_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );



    // ===== 2 File choose panel =====

    // 2nd file choose
    File_2_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_2_textbox->switch_textbox_type(HEADER_3);
    
    File_2_button->switch_button_textbox_type(HEADER_2);
    File_2_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_2_button->on_click = []() { file_choose_or_clear(2); };

    File_2_button->switch_push_mode();

    File_2_panel->set_border_radius(0);
    File_2_button->set_border_radius(0);


    // 2nd file data choose
    File_2_data_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_2_data_textbox->switch_textbox_type(HEADER_3);
    
    File_2_data_button->switch_button_textbox_type(HEADER_2);
    File_2_data_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_2_data_button->on_click = []() { file_data_choose_or_clear(2); };

    File_2_data_button->switch_push_mode();

    File_2_data_panel->set_border_radius(0);
    File_2_data_button->set_border_radius(0);


    File_2_panel->add_element(

        File_2_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_2_panel->add_element(

        File_2_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_2_data_panel->add_element(

        File_2_data_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_2_data_panel->add_element(

        File_2_data_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );



    // ===== 3 File choose panel =====

    // 3rd file choose
    File_3_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_3_textbox->switch_textbox_type(HEADER_3);
    
    File_3_button->switch_button_textbox_type(HEADER_2);
    File_3_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_3_button->on_click = []() { file_choose_or_clear(3); };

    File_3_button->switch_push_mode();

    File_3_panel->set_border_radius(0);
    File_3_button->set_border_radius(0);


    // 3rd file data choose
    File_3_data_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_3_data_textbox->switch_textbox_type(HEADER_3);
    
    File_3_data_button->switch_button_textbox_type(HEADER_2);
    File_3_data_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_3_data_button->on_click = []() { file_data_choose_or_clear(3); };

    File_3_data_button->switch_push_mode();

    File_3_data_panel->set_border_radius(0);
    File_3_data_button->set_border_radius(0);


    File_3_panel->add_element(

        File_3_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_3_panel->add_element(

        File_3_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_3_data_panel->add_element(

        File_3_data_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_3_data_panel->add_element(

        File_3_data_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );



    // ===== 4 File choose panel =====

    File_4_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_4_textbox->switch_textbox_type(HEADER_3);
    
    File_4_button->switch_button_textbox_type(HEADER_2);
    File_4_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_4_button->on_click = []() { file_choose_or_clear(4); };

    File_4_button->switch_push_mode();

    File_4_panel->set_border_radius(0);
    File_4_button->set_border_radius(0);



    File_4_data_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_4_data_textbox->switch_textbox_type(HEADER_3);
    
    File_4_data_button->switch_button_textbox_type(HEADER_2);
    File_4_data_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_4_data_button->on_click = []() { file_data_choose_or_clear(4); };

    File_4_data_button->switch_push_mode();

    File_4_data_panel->set_border_radius(0);
    File_4_data_button->set_border_radius(0);


    File_4_panel->add_element(

        File_4_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_4_panel->add_element(

        File_4_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );


    File_4_data_panel->add_element(

        File_4_data_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_4_data_panel->add_element(

        File_4_data_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );



    // ===== 5 File choose panel =====

    File_5_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_5_textbox->switch_textbox_type(HEADER_3);
    
    File_5_button->switch_button_textbox_type(HEADER_2);
    File_5_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_5_button->on_click = []() { file_choose_or_clear(5); };

    File_5_button->switch_push_mode();

    File_5_panel->set_border_radius(0);
    File_5_button->set_border_radius(0);



    File_5_data_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_5_data_textbox->switch_textbox_type(HEADER_3);
    
    File_5_data_button->switch_button_textbox_type(HEADER_2);
    File_5_data_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_5_data_button->on_click = []() { file_data_choose_or_clear(5); };

    File_5_data_button->switch_push_mode();

    File_5_data_panel->set_border_radius(0);
    File_5_data_button->set_border_radius(0);


    File_5_panel->add_element(

        File_5_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_5_panel->add_element(

        File_5_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );


    File_5_data_panel->add_element(

        File_5_data_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_5_data_panel->add_element(

        File_5_data_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );


    // ===== 6 File choose panel =====

    File_6_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_6_textbox->switch_textbox_type(HEADER_3);
    
    File_6_button->switch_button_textbox_type(HEADER_2);
    File_6_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_6_button->on_click = []() { file_choose_or_clear(6); };

    File_6_button->switch_push_mode();

    File_6_panel->set_border_radius(0);
    File_6_button->set_border_radius(0);



    File_6_data_panel->set_size(file_choose_mini_panels_width , file_choose_mini_panels_height);

    File_6_data_textbox->switch_textbox_type(HEADER_3);
    
    File_6_data_button->switch_button_textbox_type(HEADER_2);
    File_6_data_button->set_size(file_choose_buttons_width, file_choose_buttons_height);

    // Lambda for callback with args
    File_6_data_button->on_click = []() { file_data_choose_or_clear(6); };

    File_6_data_button->switch_push_mode();

    File_6_data_panel->set_border_radius(0);
    File_6_data_button->set_border_radius(0);


    File_6_panel->add_element(

        File_6_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_6_panel->add_element(

        File_6_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );


    File_6_data_panel->add_element(

        File_6_data_textbox,
        (file_choose_mini_panels_width - file_choose_buttons_width) / 2,
        file_choose_mini_panels_height * 0.5,
        1

    );

    File_6_data_panel->add_element(

        File_6_data_button,
        file_choose_mini_panels_width - 0.5 * file_choose_buttons_width,
        file_choose_mini_panels_height * 0.5,
        1

    );


    // ===== Shadow =====

    File_1_button->set_shadow_offset(0, 0);
    File_2_button->set_shadow_offset(0, 0);
    File_3_button->set_shadow_offset(0, 0);
    File_4_button->set_shadow_offset(0, 0);
    File_5_button->set_shadow_offset(0, 0);
    File_6_button->set_shadow_offset(0, 0);

    File_1_panel->set_shadow_offset(-10, -10);
    File_2_panel->set_shadow_offset(-10, -10);
    File_3_panel->set_shadow_offset(-10, -10);
    File_4_panel->set_shadow_offset(-10, -10);
    File_5_panel->set_shadow_offset(-10, -10);
    File_6_panel->set_shadow_offset(-10, -10);


    File_1_data_button->set_shadow_offset(0, 0);
    File_2_data_button->set_shadow_offset(0, 0);
    File_3_data_button->set_shadow_offset(0, 0);
    File_4_data_button->set_shadow_offset(0, 0);
    File_5_data_button->set_shadow_offset(0, 0);
    File_6_data_button->set_shadow_offset(0, 0);

    File_1_data_panel->set_shadow_offset(-10, -10);
    File_2_data_panel->set_shadow_offset(-10, -10);
    File_3_data_panel->set_shadow_offset(-10, -10);
    File_4_data_panel->set_shadow_offset(-10, -10);
    File_5_data_panel->set_shadow_offset(-10, -10);
    File_6_data_panel->set_shadow_offset(-10, -10);



    // ===== Put elements in main panel =====

    // File 1 choose

    File_choose_panel->add_element(

        File_1_panel,
        file_choose_panel_width * 0.25, // Centered with equal margin
        1 * SCREEN_MARGIN_2 + 0.5 * file_choose_mini_panels_height,
        1

    );


    File_choose_panel->add_element(

        File_1_data_panel,
        file_choose_panel_width * 0.75, // Centered with equal margin
        1 * SCREEN_MARGIN_2 + 0.5 * file_choose_mini_panels_height,
        1

    );

    // File 2 choose
    
    File_choose_panel->add_element(

        File_2_panel,
        file_choose_panel_width * 0.25, // Centered with equal margin
        2 * SCREEN_MARGIN_2 + 1.5 * file_choose_mini_panels_height,
        1

    );


    File_choose_panel->add_element(

        File_2_data_panel,
        file_choose_panel_width * 0.75, // Centered with equal margin
        2 * SCREEN_MARGIN_2 + 1.5 * file_choose_mini_panels_height,
        1

    );

    // File 3 choose

    File_choose_panel->add_element(

        File_3_panel,
        file_choose_panel_width * 0.25, // Centered with equal margin
        3 * SCREEN_MARGIN_2 + 2.5 * file_choose_mini_panels_height,
        1

    );


    File_choose_panel->add_element(

        File_3_data_panel,
        file_choose_panel_width * 0.75, // Centered with equal margin
        3 * SCREEN_MARGIN_2 + 2.5 * file_choose_mini_panels_height,
        1

    );

    // File 4 choose

    File_choose_panel->add_element(

        File_4_panel,
        file_choose_panel_width * 0.25, // Centered with equal margin
        4 * SCREEN_MARGIN_2 + 3.5 * file_choose_mini_panels_height,
        1

    );


    File_choose_panel->add_element(

        File_4_data_panel,
        file_choose_panel_width * 0.75, // Centered with equal margin
        4 * SCREEN_MARGIN_2 + 3.5 * file_choose_mini_panels_height,
        1

    );


    // File 5 choose

    File_choose_panel->add_element(

        File_5_panel,
        file_choose_panel_width * 0.25, // Centered with equal margin
        5 * SCREEN_MARGIN_2 + 4.5 * file_choose_mini_panels_height,
        1

    );


    File_choose_panel->add_element(

        File_5_data_panel,
        file_choose_panel_width * 0.75, // Centered with equal margin
        5 * SCREEN_MARGIN_2 + 4.5 * file_choose_mini_panels_height,
        1

    );


    // File 6 choose

    File_choose_panel->add_element(

        File_6_panel,
        file_choose_panel_width * 0.25, // Centered with equal margin
        6 * SCREEN_MARGIN_2 + 5.5 * file_choose_mini_panels_height,
        1

    );


    File_choose_panel->add_element(

        File_6_data_panel,
        file_choose_panel_width * 0.75, // Centered with equal margin
        6 * SCREEN_MARGIN_2 + 5.5 * file_choose_mini_panels_height,
        1

    );



    // ===== Content by new or resetted states =====

    My_SDL_button* state_buttons[] = {

        File_1_button,
        File_2_button,
        File_3_button,
        File_4_button,
        File_5_button,
        File_6_button

    };

    My_SDL_textbox* state_textboxes[] = {

        File_1_textbox,
        File_2_textbox,
        File_3_textbox,
        File_4_textbox,
        File_5_textbox,
        File_6_textbox

    };

    My_SDL_panel* state_panels[] = {

        File_1_panel,
        File_2_panel,
        File_3_panel,
        File_4_panel,
        File_5_panel,
        File_6_panel

    };

    file_choose_panel_state* state_panels_states[] = {

        &file_choose_info.panels_states.file_1_panel_state,
        &file_choose_info.panels_states.file_2_panel_state,
        &file_choose_info.panels_states.file_3_panel_state,
        &file_choose_info.panels_states.file_4_panel_state,
        &file_choose_info.panels_states.file_5_panel_state,
        &file_choose_info.panels_states.file_6_panel_state

    };

    std::string* file_paths[] = {

        &file_choose_info.file_1_path,
        &file_choose_info.file_2_path,
        &file_choose_info.file_3_path,
        &file_choose_info.file_4_path,
        &file_choose_info.file_5_path,
        &file_choose_info.file_6_path

    };



    My_SDL_button* state_data_buttons[] = {

        File_1_data_button,
        File_2_data_button,
        File_3_data_button,
        File_4_data_button,
        File_5_data_button,
        File_6_data_button

    };

    My_SDL_textbox* state_data_textboxes[] = {

        File_1_data_textbox,
        File_2_data_textbox,
        File_3_data_textbox,
        File_4_data_textbox,
        File_5_data_textbox,
        File_6_data_textbox

    };

    My_SDL_panel* state_data_panels[] = {

        File_1_data_panel,
        File_2_data_panel,
        File_3_data_panel,
        File_4_data_panel,
        File_5_data_panel,
        File_6_data_panel

    };

    file_choose_panel_state* state_data_panels_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };

    std::string* file_data_paths[] = {

        &file_data_choose_info.file_1_data_path,
        &file_data_choose_info.file_2_data_path,
        &file_data_choose_info.file_3_data_path,
        &file_data_choose_info.file_4_data_path,
        &file_data_choose_info.file_5_data_path,
        &file_data_choose_info.file_6_data_path

    };


    for (int i = 0; i <= 5; i++)
    {
        // File add

        if (*state_panels_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            state_buttons[i]->get_button_content_textbox()->set_content("+");

            *file_paths[i] = "";

            std::string new_file_string = "File " + std::to_string(i + 1);
            state_textboxes[i]->set_content(new_file_string);

            state_panels[i]->set_visible_flag(true);
        }

        else if (*state_panels_states[i] == file_choose_panel_state::CHOSEN_STATE)
        {
            state_buttons[i]->get_button_content_textbox()->set_content("-");

            std::string new_file_string = file_name_from_path(*file_paths[i]);
            state_textboxes[i]->set_content(new_file_string);

            state_panels[i]->set_visible_flag(true);
        }

        // Just hide
        else
        {
            *file_paths[i] = "";
            state_panels[i]->set_visible_flag(false);
        }


        // File data add

        if (*state_data_panels_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            state_data_buttons[i]->get_button_content_textbox()->set_content("+");

            *file_data_paths[i] = "";

            std::string new_data_file_string = "File " + std::to_string(i + 1) + " data";
            state_data_textboxes[i]->set_content(new_data_file_string);

            state_data_panels[i]->set_visible_flag(true);
        }

        else if (*state_data_panels_states[i] == file_choose_panel_state::CHOSEN_STATE)
        {
            state_data_buttons[i]->get_button_content_textbox()->set_content("-");

            std::string new_data_file_string = file_name_from_path(*file_data_paths[i]);
            state_data_textboxes[i]->set_content(new_data_file_string);

            state_data_panels[i]->set_visible_flag(true);
        }

        // Just hide
        else
        {
            *file_data_paths[i] = "";
            state_data_panels[i]->set_visible_flag(false);
        }
        
    }

    // ===== Content by new or resetted states =====


    // ===== Preview panel =====

    Video_panel->set_size(video_panel_width, video_panel_height);
    Video_panel->set_render_point(video_panel_x, video_panel_y);
    Video_panel->set_border_radius(0);

    /*

    // Set picture 
    File_preview_texture->set_texture_by_image(
        
        "C:/creator/it/programs/cpp/PSMVA/libs/program_gui/basic_elements/content/pictures/example.png",
        this_app.renderer

    );

    */

    PSMVA_textbox_fc->switch_textbox_type(HEADER_1);
    PSMVA_textbox_fc->set_content(THIS_APP_NAME);


    File_preview_texture->set_size(video_panel_width - SCREEN_MARGIN_2, video_panel_height - SCREEN_MARGIN_2);

    Video_panel->add_element(File_preview_texture, video_panel_width * 0.5, video_panel_height * 0.5, 1);
    Video_panel->add_element(PSMVA_textbox_fc, video_panel_width * 0.5, video_panel_height * 0.5, 2);


    // ===== Preview panel =====

     
    // Main menu button

    Main_menu_button->switch_button_textbox_type(ORDINARY_TEXT);
    Main_menu_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_back_to_main_menu));
    Main_menu_button->set_size(state_buttons_width, state_buttons_height);
    Main_menu_button->set_render_point(main_menu_button_x, main_menu_button_y);

    Main_menu_button->on_click = back_to_main_menu;

    Main_menu_button->set_border_radius(0);


    // Study start button

    Study_start_button->switch_button_textbox_type(ORDINARY_TEXT);
    Study_start_button->get_button_content_textbox()->set_content(str_by_dictionary(gd_study_start));
    Study_start_button->set_size(state_buttons_width, state_buttons_height);
    Study_start_button->set_render_point(study_start_button_x, study_start_button_y);

    Study_start_button->on_click = study_start;

    Study_start_button->extern_click_permission = check_start_study_access;
    Study_start_button->set_access_type(BUTTON_EXTERN_CLICK_PERMISSION);

    Study_start_button->set_border_radius(0);



    Video_panel->set_opacity(255);
    File_preview_texture->set_opacity(255);
}


void file_choose_elements_free_and_nullptr()
{
    // Protection
    if (File_choose_background_panel == nullptr) return;


    // ===== Free all elements =====

    // Background
    File_choose_background_panel->delete_element();


    // File choose panel (36 elements now)

    File_choose_panel->delete_element();

    // Preview panel

    Video_panel->delete_element();


    // State control button
    Main_menu_button->delete_element();
    Study_start_button->delete_element();
    

    // ===== Nullptr the pointers =====

    // Background
    File_choose_background_panel = nullptr;


    // File choose panel (5 elements now)

    File_choose_panel = nullptr;

    File_1_panel = nullptr;
    File_1_textbox = nullptr;
    File_1_button = nullptr;

    File_2_panel = nullptr;
    File_2_textbox = nullptr;
    File_2_button = nullptr;

    File_3_panel = nullptr;
    File_3_textbox = nullptr;
    File_3_button = nullptr;

    File_4_panel = nullptr;
    File_4_textbox = nullptr;
    File_4_button = nullptr;

    File_5_panel = nullptr;
    File_5_textbox = nullptr;
    File_5_button = nullptr;

    File_6_panel = nullptr;
    File_6_textbox = nullptr;
    File_6_button = nullptr;


    File_1_data_panel = nullptr;
    File_1_data_textbox = nullptr;
    File_1_data_button = nullptr;

    File_2_data_panel = nullptr;
    File_2_data_textbox = nullptr;
    File_2_data_button = nullptr;

    File_3_data_panel = nullptr;
    File_3_data_textbox = nullptr;
    File_3_data_button = nullptr;

    File_4_data_panel = nullptr;
    File_4_data_textbox = nullptr;
    File_4_data_button = nullptr;

    File_5_data_panel = nullptr;
    File_5_data_textbox = nullptr;
    File_5_data_button = nullptr;
    
    File_6_data_panel = nullptr;
    File_6_data_textbox = nullptr;
    File_6_data_button = nullptr;


    // Preview panel

    Video_panel = nullptr;
    PSMVA_textbox_fc = nullptr;
    File_preview_texture = nullptr;


    // State control button
    Main_menu_button = nullptr;
    Study_start_button = nullptr;

}


void file_choose_elements_update()
{
    // ===== Check if textboxes need content renew =====

    reset_passed_by_dictionary_textboxes_if_language_switched_fc();


    // ===== Update all elements =====

    // Background
    File_choose_background_panel->update();

    // File choose panel (5 elements now)

    File_choose_panel->update();

    // Preview panel

    Video_panel->update();

    
    // State control button
    Main_menu_button->update();
    Study_start_button->update();
        
}


void reset_passed_by_dictionary_textboxes_if_language_switched_fc()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        // Textboxes for update - no option to switch lang on this state, so just pass through

        // No free textboxes on state

    }
}

int counter_1 = 0;


void file_choose_actions()
{
    // State actions


    if (App_inputs.is_just_released(Key_actions::EXIT))
    {
        this_app.app_sm.request_state_change(MAIN_MENU_ID);
    }


    if (App_inputs.is_just_released(Key_actions::LEFT))
    {
        File_preview_texture->set_size(File_preview_texture->get_width_size() - 5, File_preview_texture->get_height_size());
    }

    
    if (App_inputs.is_just_released(Key_actions::RIGHT))
    {
        File_preview_texture->set_size(File_preview_texture->get_width_size() + 5, File_preview_texture->get_height_size());
    }


    if (App_inputs.is_just_released(Key_actions::UP))
    {
        File_preview_texture->set_size(File_preview_texture->get_width_size(), File_preview_texture->get_height_size() + 5);
    }

    
    if (App_inputs.is_just_released(Key_actions::DOWN))
    {
        File_preview_texture->set_size(File_preview_texture->get_width_size(), File_preview_texture->get_height_size() -5);
    }



    if (App_inputs.is_just_released(Key_actions::ENTER))
    {

        if (counter_1 == 0)
        {

            File_preview_texture->set_texture_by_image(
        
                "C:/creator/it/programs/cpp/PSMVA/libs/program_gui/basic_elements/content/pictures/example_2.png",
                this_app.renderer
        
            );


            counter_1 +=1;
        }

        else if (counter_1 == 1)
        {
            File_preview_texture->set_texture_by_image(
        
                "C:/creator/it/programs/cpp/PSMVA/libs/program_gui/basic_elements/content/pictures/example.png",
                this_app.renderer
        
            );

            counter_1 = 0;
        }

        File_preview_texture->set_size(video_panel_width - SCREEN_MARGIN_2, video_panel_height - SCREEN_MARGIN_2);
    }


}


void file_choose_elements_render(SDL_Renderer* renderer)
{
    // Render all elements

    // Background
    File_choose_background_panel->render(renderer);

    
    // File choose panel (5 elements now)

    File_choose_panel->render(renderer);

    
    // State control button

    Main_menu_button->render(renderer);
    Study_start_button->render(renderer);

}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS


void file_choose_data_init()
{
    // Block reinitialization

    bool need_init = true;

    if (file_choose_info.panels_states.file_1_panel_state == file_choose_panel_state::CHOSEN_STATE) need_init = false;

    if (need_init)
    {
        file_choose_info.file_1_path = "";
        file_choose_info.file_2_path = "";
        file_choose_info.file_3_path = "";
        file_choose_info.file_4_path = "";
        file_choose_info.file_5_path = "";
        file_choose_info.file_6_path = "";
    
    
        file_choose_info.panels_states.file_1_panel_state = file_choose_panel_state::EMPTY_STATE;
        file_choose_info.panels_states.file_2_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_choose_info.panels_states.file_3_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_choose_info.panels_states.file_4_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_choose_info.panels_states.file_5_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_choose_info.panels_states.file_6_panel_state = file_choose_panel_state::HIDDEN_STATE;

        file_data_choose_info.file_1_data_path = "";
        file_data_choose_info.file_2_data_path = "";
        file_data_choose_info.file_3_data_path = "";
        file_data_choose_info.file_4_data_path = "";
        file_data_choose_info.file_5_data_path = "";
        file_data_choose_info.file_6_data_path = "";


        file_data_choose_info.panels_states.file_1_data_panel_state = file_choose_panel_state::EMPTY_STATE;
        file_data_choose_info.panels_states.file_2_data_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_data_choose_info.panels_states.file_3_data_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_data_choose_info.panels_states.file_4_data_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_data_choose_info.panels_states.file_5_data_panel_state = file_choose_panel_state::HIDDEN_STATE;
        file_data_choose_info.panels_states.file_6_data_panel_state = file_choose_panel_state::HIDDEN_STATE;

    }


    if (TEST_MODE)
    {
        std::cout << file_choose_info.file_1_path << "\n" << std::endl;
        std::cout << file_choose_info.file_2_path << "\n" << std::endl;
        std::cout << file_choose_info.file_3_path << "\n" << std::endl;
        std::cout << file_choose_info.file_4_path << "\n" << std::endl;
        std::cout << file_choose_info.file_5_path << "\n" << std::endl;
        std::cout << file_choose_info.file_6_path << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_1_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_2_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_3_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_4_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_5_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_6_panel_state) << "\n" << std::endl;
    }
}



std::string utf16_to_utf8(const std::wstring& wstr)
{
    if (wstr.empty())
        return "";

    int size_needed = WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr.c_str(),
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    std::string result(size_needed - 1, '\0');

    WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr.c_str(),
        -1,
        result.data(),
        size_needed,
        nullptr,
        nullptr
    );

    return result;
}


std::string file_name_from_path(std::string path)
{


    // Find the last slash (Windows '\' or Unix '/')
    size_t last_slash_idx = path.find_last_of("\\/");
    
    // Return the substring before slash
    // std::string::npos - special constant, which returned if there is no slashes 
    // inside the link
    if (std::string::npos != last_slash_idx)
    {
        return path.substr(last_slash_idx + 1);
    }
    
    // No slashes or EMPTY_STATE case
    return path;

    // Same by library
    // return std::filesystem::path(path).filename().string();
}



std::string get_path_by_file_manager()
{
    // For cyrillic
    wchar_t file_name[MAX_PATH] = L"";

    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);

    // We may pass SDL-window descriptor, but NULL works as well
    ofn.hwndOwner = NULL;
    
    // File filter
    ofn.lpstrFilter =
        L"All files (*.*)\0*.*\0"
        L"Txt files (*.txt)\0*.txt\0";

    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    
    // Flags: file should exists, path must be valid

    ofn.Flags =
        OFN_EXPLORER |
        OFN_FILEMUSTEXIST |
        OFN_HIDEREADONLY;


    // Calls the window, returns the path, if user choosed the path
    if (GetOpenFileNameW(&ofn))
    {
        return utf16_to_utf8(file_name);
    }

    return ""; // User closed the window or error occures
}


void add_file_path(unsigned int file_number)
{

    // Check if rebuild is needed
    if (file_number == 0) return;

    unsigned int add_index = file_number - 1;

    // Massive of variables links
    std::string* paths[] = {

        &file_choose_info.file_1_path,
        &file_choose_info.file_2_path,
        &file_choose_info.file_3_path,
        &file_choose_info.file_4_path,
        &file_choose_info.file_5_path,
        &file_choose_info.file_6_path

    };

    file_choose_panel_state* panel_states[] = {

        &file_choose_info.panels_states.file_1_panel_state,
        &file_choose_info.panels_states.file_2_panel_state,
        &file_choose_info.panels_states.file_3_panel_state,
        &file_choose_info.panels_states.file_4_panel_state,
        &file_choose_info.panels_states.file_5_panel_state,
        &file_choose_info.panels_states.file_6_panel_state

    };


    file_choose_panel_state* data_panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };


    if (*panel_states[add_index] != file_choose_panel_state::EMPTY_STATE) std::cerr << "Error on the file pass!";

    std::string selected_path = get_path_by_file_manager();

    if (selected_path.empty())
    {
        return;
    }


    // Check format

    std::string path_ending = ".avi";

    std::string actual_ending = "";
    if (selected_path.length() >= path_ending.length()) 
    {
        actual_ending = selected_path.substr(selected_path.length() - path_ending.length());
        
        std::transform(actual_ending.begin(), actual_ending.end(), actual_ending.begin(), 
                       [](unsigned char c) { return std::tolower(c); });
    }

    if (actual_ending != path_ending) 
    {
        std::cout << "Wrong video format!\n";
        return;
    }


    // Avi pass:

    
    *paths[add_index] = selected_path;
    *panel_states[add_index] = file_choose_panel_state::CHOSEN_STATE;
    
    // Show next panel
    if (add_index < 5)
    {
        // Show file add panel
        *panel_states[add_index + 1] = file_choose_panel_state::EMPTY_STATE;

        // Show file data add panel
        *data_panel_states[add_index + 1] = file_choose_panel_state::EMPTY_STATE;
    }
    
    // Reset panels textboxes content according to the new list
    File_1_textbox->set_content(file_name_from_path(file_choose_info.file_1_path));
    File_2_textbox->set_content(file_name_from_path(file_choose_info.file_2_path));
    File_3_textbox->set_content(file_name_from_path(file_choose_info.file_3_path));
    File_4_textbox->set_content(file_name_from_path(file_choose_info.file_4_path));
    File_5_textbox->set_content(file_name_from_path(file_choose_info.file_5_path));
    File_6_textbox->set_content(file_name_from_path(file_choose_info.file_6_path));


    if (TEST_MODE) std::cout << "Added path for file: " << file_number << ". Current path: \"" << *paths[add_index] << "\".\n" << std::endl;
}


void clear_file_path(unsigned int file_number)
{
    // Clear file index
    int clear_index = file_number - 1;

    // Massive of variables links
    std::string* paths[] = {

        &file_choose_info.file_1_path,
        &file_choose_info.file_2_path,
        &file_choose_info.file_3_path,
        &file_choose_info.file_4_path,
        &file_choose_info.file_5_path,
        &file_choose_info.file_6_path

    };


    std::string* data_paths[] = {

        &file_data_choose_info.file_1_data_path,
        &file_data_choose_info.file_2_data_path,
        &file_data_choose_info.file_3_data_path,
        &file_data_choose_info.file_4_data_path,
        &file_data_choose_info.file_5_data_path,
        &file_data_choose_info.file_6_data_path

    };


    file_choose_panel_state* panel_states[] = {

        &file_choose_info.panels_states.file_1_panel_state,
        &file_choose_info.panels_states.file_2_panel_state,
        &file_choose_info.panels_states.file_3_panel_state,
        &file_choose_info.panels_states.file_4_panel_state,
        &file_choose_info.panels_states.file_5_panel_state,
        &file_choose_info.panels_states.file_6_panel_state

    };


    file_choose_panel_state* data_panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };


    // Drop parsed data

    parsed_video_data* videos_metadata[] = {

        &files_metadata.video_1_data,
        &files_metadata.video_2_data,
        &files_metadata.video_3_data,
        &files_metadata.video_4_data,
        &files_metadata.video_5_data,
        &files_metadata.video_6_data

    };


    // Move the names
    if (clear_index >= 0 && clear_index < 6)
    {
        // From the deleted one
        for (int i = clear_index; i < 5; ++i)
        {
            *paths[i] = *paths[i + 1];
            *data_paths[i] = *data_paths[i + 1];
        }

        // EMPTY_STATE string to the last one (previos becomes EMPTY_STATE automatically
        // during the iteration loop)
        *paths[5] = "";
        *data_paths[5] = "";

    }

    
    // Reset panels textboxes content according to the new list
    File_1_textbox->set_content(file_name_from_path(file_choose_info.file_1_path));
    File_2_textbox->set_content(file_name_from_path(file_choose_info.file_2_path));
    File_3_textbox->set_content(file_name_from_path(file_choose_info.file_3_path));
    File_4_textbox->set_content(file_name_from_path(file_choose_info.file_4_path));
    File_5_textbox->set_content(file_name_from_path(file_choose_info.file_5_path));
    File_6_textbox->set_content(file_name_from_path(file_choose_info.file_6_path));


    File_1_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_1_data_path));
    File_2_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_2_data_path));
    File_3_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_3_data_path));
    File_4_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_4_data_path));
    File_5_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_5_data_path));
    File_6_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_6_data_path));
    



    // Reset statuses
    if ((clear_index >= 0 && clear_index < 6) && panel_states[clear_index + 1])
    {
        // From the deleted one
        for (int i = clear_index; i < 5; ++i)
        {
            *panel_states[i] = *panel_states[i + 1];
            *data_panel_states[i] = *data_panel_states[i + 1];

            *videos_metadata[i] = *videos_metadata[i + 1];
        }

        // HIDE last one (previos becomes HIDDEN_STATE automatically
        // during the iteration loop)
        *panel_states[5] = file_choose_panel_state::HIDDEN_STATE;
        *data_panel_states[5] = file_choose_panel_state::HIDDEN_STATE;

        drop_metadata(*videos_metadata[5]);


        if (*panel_states[0] == file_choose_panel_state::HIDDEN_STATE) *panel_states[0] = file_choose_panel_state::EMPTY_STATE;
        if (*data_panel_states[0] == file_choose_panel_state::HIDDEN_STATE) *data_panel_states[0] = file_choose_panel_state::EMPTY_STATE;

        if (*data_panel_states[0] == file_choose_panel_state::EMPTY_STATE) drop_metadata(*videos_metadata[0]);
    }


    // If we clear file slot number six - set it as empty, not hiden
    if (file_number == 6)
    {
        *panel_states[5] = file_choose_panel_state::EMPTY_STATE;
        *data_panel_states[5] = file_choose_panel_state::EMPTY_STATE;

        if (*data_panel_states[5] == file_choose_panel_state::EMPTY_STATE) drop_metadata(*videos_metadata[5]);
    }

    // Backwards delete error handle 
    if (*data_panel_states[4] == file_choose_panel_state::CHOSEN_STATE)
    {
        *panel_states[5] = file_choose_panel_state::EMPTY_STATE;
        *data_panel_states[5] = file_choose_panel_state::EMPTY_STATE;

        if (*data_panel_states[5] == file_choose_panel_state::EMPTY_STATE) drop_metadata(*videos_metadata[5]);
    }


    if (TEST_MODE) std::cout << "Clear path for file: " << file_number << ". Current path: \"" << *paths[clear_index] << "\".\n" << std::endl;
}



void file_choose_or_clear(int file_number)
{
    // EH
    if (file_number < 1 || file_number > 6) return;


    // Clear file index
    int clear_index = file_number - 1;


    file_choose_panel_state* panel_states[] = {

        &file_choose_info.panels_states.file_1_panel_state,
        &file_choose_info.panels_states.file_2_panel_state,
        &file_choose_info.panels_states.file_3_panel_state,
        &file_choose_info.panels_states.file_4_panel_state,
        &file_choose_info.panels_states.file_5_panel_state,
        &file_choose_info.panels_states.file_6_panel_state

    };


    file_choose_panel_state* data_panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };


    bool add = false;
    bool clear = false;
    

    if (*panel_states[clear_index] == file_choose_panel_state::EMPTY_STATE) add = true;
    else if (*panel_states[clear_index] == file_choose_panel_state::CHOSEN_STATE) clear = true;

    if (add)
    {
        if (TEST_MODE) std::cout << "CHOOSE TO ADD FILE\n\n";
        add_file_path(file_number);
    }

    if (clear)
    {
        if (TEST_MODE) std::cout << "CHOOSE TO CLEAR FILE\n\n";
        clear_file_path(file_number);
    }


    // Set new textboxes

    My_SDL_button* buttons[] = {

        File_1_button,
        File_2_button,
        File_3_button,
        File_4_button,
        File_5_button,
        File_6_button

    };

    My_SDL_panel* panels[] = {

        File_1_panel,
        File_2_panel,
        File_3_panel,
        File_4_panel,
        File_5_panel,
        File_6_panel

    };


    My_SDL_button* data_buttons[] = {

        File_1_data_button,
        File_2_data_button,
        File_3_data_button,
        File_4_data_button,
        File_5_data_button,
        File_6_data_button

    };

    My_SDL_panel* data_panels[] = {

        File_1_data_panel,
        File_2_data_panel,
        File_3_data_panel,
        File_4_data_panel,
        File_5_data_panel,
        File_6_data_panel

    };



    for (int i = 0; i <= 5; i++)
    {
        if (*panel_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            buttons[i]->get_button_content_textbox()->set_content("+");
            
            panels[i]->set_visible_flag(true);

            data_buttons[i]->get_button_content_textbox()->set_content("+");
            
            data_panels[i]->set_visible_flag(true);
        }


        else if (*panel_states[i] == file_choose_panel_state::CHOSEN_STATE)
        {
            buttons[i]->get_button_content_textbox()->set_content("-");

            panels[i]->set_visible_flag(true);

            data_panels[i]->set_visible_flag(true);
        }

        else 
        {
            *data_panel_states[i] = file_choose_panel_state::HIDDEN_STATE;

            panels[i]->set_visible_flag(false);
            data_panels[i]->set_visible_flag(false);
        }



        if (*data_panel_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            data_buttons[i]->get_button_content_textbox()->set_content("+");
            
            data_panels[i]->set_visible_flag(true);
        }

        else if (*data_panel_states[i] == file_choose_panel_state::CHOSEN_STATE)
        {
            data_buttons[i]->get_button_content_textbox()->set_content("-");

            data_panels[i]->set_visible_flag(true);
        }

        else data_panels[i]->set_visible_flag(false);
    }


    // Drop parsed data

    parsed_video_data* videos_metadata[] = {

        &files_metadata.video_1_data,
        &files_metadata.video_2_data,
        &files_metadata.video_3_data,
        &files_metadata.video_4_data,
        &files_metadata.video_5_data,
        &files_metadata.video_6_data

    };
    

    if (*panel_states[5] == file_choose_panel_state::HIDDEN_STATE && *panel_states[4] == file_choose_panel_state::CHOSEN_STATE)
    {
        buttons[5]->get_button_content_textbox()->set_content("+");
            
        panels[5]->set_visible_flag(true);

        data_buttons[5]->get_button_content_textbox()->set_content("+");
        
        data_panels[5]->set_visible_flag(true);

        *panel_states[5] = file_choose_panel_state::EMPTY_STATE;

        *data_panel_states[5] = file_choose_panel_state::EMPTY_STATE;

        drop_metadata(*videos_metadata[5]);

    }


    // Add filler string to the new empty
    
    std::string* paths[] = {

        &file_choose_info.file_1_path,
        &file_choose_info.file_2_path,
        &file_choose_info.file_3_path,
        &file_choose_info.file_4_path,
        &file_choose_info.file_5_path,
        &file_choose_info.file_6_path

    };
    
    My_SDL_textbox* textboxes[] = {

        File_1_textbox,
        File_2_textbox,
        File_3_textbox,
        File_4_textbox,
        File_5_textbox,
        File_6_textbox

    };


    std::string* data_paths[] = {

        &file_data_choose_info.file_1_data_path,
        &file_data_choose_info.file_2_data_path,
        &file_data_choose_info.file_3_data_path,
        &file_data_choose_info.file_4_data_path,
        &file_data_choose_info.file_5_data_path,
        &file_data_choose_info.file_6_data_path

    };
    
    My_SDL_textbox* data_textboxes[] = {

        File_1_data_textbox,
        File_2_data_textbox,
        File_3_data_textbox,
        File_4_data_textbox,
        File_5_data_textbox,
        File_6_data_textbox

    };


    for (int i = 0; i <= 5; i++)
    {
        if (*panel_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            std::string new_file_string = "File " + std::to_string(i + 1);
            textboxes[i]->set_content(new_file_string);
        }

        if (*data_panel_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            std::string new_file_data_string = "File " + std::to_string(i + 1) + " data";
            data_textboxes[i]->set_content(new_file_data_string);
        }
    }
    

    // Check

    if (TEST_MODE)
    {
        std::cout << file_choose_info.file_1_path << "\n" << std::endl;
        std::cout << file_choose_info.file_2_path << "\n" << std::endl;
        std::cout << file_choose_info.file_3_path << "\n" << std::endl;
        std::cout << file_choose_info.file_4_path << "\n" << std::endl;
        std::cout << file_choose_info.file_5_path << "\n" << std::endl;
        std::cout << file_choose_info.file_6_path << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_1_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_2_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_3_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_4_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_5_panel_state) << "\n" << std::endl;
        std::cout << static_cast<int>(file_choose_info.panels_states.file_6_panel_state) << "\n" << std::endl;
    }
}


void clear_file_data_path(unsigned int file_number);


void add_file_data_path(unsigned int file_number)
{

    // Check if rebuild is needed
    if (file_number == 0) return;

    unsigned int add_index = file_number - 1;

    // Massive of variables links
    std::string* paths[] = {

        &file_data_choose_info.file_1_data_path,
        &file_data_choose_info.file_2_data_path,
        &file_data_choose_info.file_3_data_path,
        &file_data_choose_info.file_4_data_path,
        &file_data_choose_info.file_5_data_path,
        &file_data_choose_info.file_6_data_path

    };

    file_choose_panel_state* panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };

    if (*panel_states[add_index] != file_choose_panel_state::EMPTY_STATE) std::cerr << "Error on the file pass!";


    std::string selected_path = get_path_by_file_manager();


    if (selected_path.empty())
    {
        return;
    }
    


    *paths[add_index] = selected_path;
    *panel_states[add_index] = file_choose_panel_state::CHOSEN_STATE;
    

    if (TEST_MODE)
    {
        std::cout << "Added path for file: " << file_number << ". Current path: \"" << *paths[add_index] << "\".\n" << std::endl;
        std::cout << "Start parsing!\n" << std::endl;
    }


    // Parse data from txt

    parsed_video_data* videos_metadata[] = {

        &files_metadata.video_1_data,
        &files_metadata.video_2_data,
        &files_metadata.video_3_data,
        &files_metadata.video_4_data,
        &files_metadata.video_5_data,
        &files_metadata.video_6_data

    };


    parse_metadata_txt(*videos_metadata[add_index], *paths[add_index]);


    std::cout << "Video 1 bit depth: " << videos_metadata[add_index]->bit_depth << ".\n" << std::endl;
    std::cout << "Video 1 frames quantity: " << videos_metadata[add_index]->frames_quantity << ".\n" << std::endl;
    std::cout << "Video 1 FPS: " << videos_metadata[add_index]->frames_per_second << ".\n" << std::endl;
    std::cout << "Video 1 frame time: " << videos_metadata[add_index]->frame_time << ".\n" << std::endl;
    std::cout << "Video 1 exposition time: " << videos_metadata[add_index]->exposition_time << ".\n" << std::endl;
    std::cout << "Video 1 width: " << videos_metadata[add_index]->width << ".\n" << std::endl;
    std::cout << "Video 1 height: " << videos_metadata[add_index]->height << ".\n" << std::endl;
    std::cout << "Video 1 status: " << static_cast<int>(videos_metadata[add_index]->status) << ".\n" << std::endl;


    // Wrong file or parse error
    if (videos_metadata[add_index]->status == txt_parse_status::EMPTY_STATE)
    {
        // Call clear function for element reset
        clear_file_data_path(file_number);

        // Clear
        drop_metadata(*videos_metadata[add_index]);
    }

    // In normal case - update textboxes
    else
    {
        // Reset panels textboxes content according to the new list
        switch (add_index)
        {
            case 0: File_1_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_1_data_path));
            case 1: File_2_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_2_data_path));
            case 2: File_3_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_3_data_path));
            case 3: File_4_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_4_data_path));
            case 4: File_5_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_5_data_path));
            case 5: File_6_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_6_data_path));
        }
    
    }
    
}


void clear_file_data_path(unsigned int file_number)
{
    // Clear file index
    int clear_index = file_number - 1;

    // Massive of variables links
    std::string* paths[] = {

        &file_data_choose_info.file_1_data_path,
        &file_data_choose_info.file_2_data_path,
        &file_data_choose_info.file_3_data_path,
        &file_data_choose_info.file_4_data_path,
        &file_data_choose_info.file_5_data_path,
        &file_data_choose_info.file_6_data_path

    };


    file_choose_panel_state* panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };


    // Move the names
    // From the deleted one
    *paths[clear_index] = "";
    *panel_states[clear_index] = file_choose_panel_state::EMPTY_STATE;


    // Drop parsed data

    parsed_video_data* videos_metadata[] = {

        &files_metadata.video_1_data,
        &files_metadata.video_2_data,
        &files_metadata.video_3_data,
        &files_metadata.video_4_data,
        &files_metadata.video_5_data,
        &files_metadata.video_6_data

    };

    drop_metadata(*videos_metadata[clear_index]);

    std::cout << "Video 1 bit depth: " << videos_metadata[clear_index]->bit_depth << ".\n" << std::endl;
    std::cout << "Video 1 frames quantity: " << videos_metadata[clear_index]->frames_quantity << ".\n" << std::endl;
    std::cout << "Video 1 FPS: " << videos_metadata[clear_index]->frames_per_second << ".\n" << std::endl;
    std::cout << "Video 1 frame time: " << videos_metadata[clear_index]->frame_time << ".\n" << std::endl;
    std::cout << "Video 1 exposition time: " << videos_metadata[clear_index]->exposition_time << ".\n" << std::endl;
    std::cout << "Video 1 width: " << videos_metadata[clear_index]->width << ".\n" << std::endl;
    std::cout << "Video 1 height: " << videos_metadata[clear_index]->height << ".\n" << std::endl;
    std::cout << "Video 1 status: " << static_cast<int>(videos_metadata[clear_index]->status) << ".\n" << std::endl;


    // Reset panels textboxes content according to the new list

    switch (clear_index)
    {
        case 0: File_1_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_1_data_path));
        case 1: File_2_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_2_data_path));
        case 2: File_3_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_3_data_path));
        case 3: File_4_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_4_data_path));
        case 4: File_5_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_5_data_path));
        case 5: File_6_data_textbox->set_content(file_name_from_path(file_data_choose_info.file_6_data_path));
    }
    
}



void file_data_choose_or_clear(int file_number)
{
    // EH
    if (file_number < 1 || file_number > 6) return;


    // Clear file index
    int clear_index = file_number - 1;


    file_choose_panel_state* panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };

    bool add = false;
    bool clear = false;

    if (*panel_states[clear_index] == file_choose_panel_state::EMPTY_STATE) add = true;
    else if (*panel_states[clear_index] == file_choose_panel_state::CHOSEN_STATE) clear = true;

    if (add)
    {
        add_file_data_path(file_number);
    }

    if (clear)
    {
        clear_file_data_path(file_number);
    }


    // Set new textboxes

    My_SDL_button* buttons[] = {

        File_1_data_button,
        File_2_data_button,
        File_3_data_button,
        File_4_data_button,
        File_5_data_button,
        File_6_data_button

    };

    My_SDL_panel* panels[] = {

        File_1_data_panel,
        File_2_data_panel,
        File_3_data_panel,
        File_4_data_panel,
        File_5_data_panel,
        File_6_data_panel

    };


    for (int i = 0; i <= 5; i++)
    {
        if (*panel_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            buttons[i]->get_button_content_textbox()->set_content("+");
            
            panels[i]->set_visible_flag(true);
        }

        else if (*panel_states[i] == file_choose_panel_state::CHOSEN_STATE)
        {
            buttons[i]->get_button_content_textbox()->set_content("-");

            panels[i]->set_visible_flag(true);
        }

        else panels[i]->set_visible_flag(false);
    }


    // Add filler string to the new empty
    
    std::string* paths[] = {

        &file_data_choose_info.file_1_data_path,
        &file_data_choose_info.file_2_data_path,
        &file_data_choose_info.file_3_data_path,
        &file_data_choose_info.file_4_data_path,
        &file_data_choose_info.file_5_data_path,
        &file_data_choose_info.file_6_data_path

    };
    
    My_SDL_textbox* textboxes[] = {

        File_1_data_textbox,
        File_2_data_textbox,
        File_3_data_textbox,
        File_4_data_textbox,
        File_5_data_textbox,
        File_6_data_textbox

    };

    for (int i = 0; i <= 5; i++)
    {
        if (*panel_states[i] == file_choose_panel_state::EMPTY_STATE) 
        {
            std::string new_file_data_string = "File " + std::to_string(i + 1) + " data";
            textboxes[i]->set_content(new_file_data_string);
        }
    }
}



// ===== Main menu button =====

void back_to_main_menu()
{
    // Request state change
    this_app.app_sm.request_state_change(MAIN_MENU_ID);
}


// ===== Study start button =====

void study_start()
{
    // Setup file choose status

    file_choose_panel_state* panel_states[] = {

        &file_data_choose_info.panels_states.file_1_data_panel_state,
        &file_data_choose_info.panels_states.file_2_data_panel_state,
        &file_data_choose_info.panels_states.file_3_data_panel_state,
        &file_data_choose_info.panels_states.file_4_data_panel_state,
        &file_data_choose_info.panels_states.file_5_data_panel_state,
        &file_data_choose_info.panels_states.file_6_data_panel_state

    };


    bool* statuses[] = {

        &files_choose_status.file_1_choosen,
        &files_choose_status.file_2_choosen,
        &files_choose_status.file_3_choosen,
        &files_choose_status.file_4_choosen,
        &files_choose_status.file_5_choosen,
        &files_choose_status.file_6_choosen

    };

    
    for (unsigned int i = 0; i < 6; i++)
    {
        if (*panel_states[i] == file_choose_panel_state::CHOSEN_STATE)
            *statuses[i] = true;
        else    
            *statuses[i] = false;
    }


    // Switch state

    // Request state change
    this_app.app_sm.request_state_change(MASKS_SETUP_ID);
}



bool check_start_study_access()
{
    bool pair_1 = false;
    bool pair_2 = false;
    bool pair_3 = false;
    bool pair_4 = false;
    bool pair_5 = false;
    bool pair_6 = false;


    if (file_choose_info.panels_states.file_1_panel_state == file_choose_panel_state::CHOSEN_STATE
        && file_data_choose_info.panels_states.file_1_data_panel_state == file_choose_panel_state::CHOSEN_STATE) 
    {
        // Always need to be choosen
        pair_1 = true;
    }

    if (
        
        (file_choose_info.panels_states.file_2_panel_state == file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_2_data_panel_state == file_choose_panel_state::CHOSEN_STATE)
        
        ||

        (file_choose_info.panels_states.file_2_panel_state != file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_2_data_panel_state != file_choose_panel_state::CHOSEN_STATE)

    ) 
    {
        // Choosen or not choosen both
        pair_2 = true;
    }


    if (
        
        (file_choose_info.panels_states.file_3_panel_state == file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_3_data_panel_state == file_choose_panel_state::CHOSEN_STATE)
        
        ||

        (file_choose_info.panels_states.file_3_panel_state != file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_3_data_panel_state != file_choose_panel_state::CHOSEN_STATE)

    ) 
    {
        // Choosen or not choosen both
        pair_3 = true;
    }


    if (
        
        (file_choose_info.panels_states.file_4_panel_state == file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_4_data_panel_state == file_choose_panel_state::CHOSEN_STATE)
        
        ||

        (file_choose_info.panels_states.file_4_panel_state != file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_4_data_panel_state != file_choose_panel_state::CHOSEN_STATE)

    ) 
    {
        // Choosen or not choosen both
        pair_4 = true;
    }


    if (
        
        (file_choose_info.panels_states.file_5_panel_state == file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_5_data_panel_state == file_choose_panel_state::CHOSEN_STATE)
        
        ||

        (file_choose_info.panels_states.file_5_panel_state != file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_5_data_panel_state != file_choose_panel_state::CHOSEN_STATE)

    ) 
    {
        // Choosen or not choosen both
        pair_5 = true;
    }


    if (
        
        (file_choose_info.panels_states.file_6_panel_state == file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_6_data_panel_state == file_choose_panel_state::CHOSEN_STATE)
        
        ||

        (file_choose_info.panels_states.file_6_panel_state != file_choose_panel_state::CHOSEN_STATE
            && file_data_choose_info.panels_states.file_6_data_panel_state != file_choose_panel_state::CHOSEN_STATE)

    ) 
    {
        // Choosen or not choosen both
        pair_6 = true;
    }



    if (pair_1 && pair_2 && pair_3 && pair_4 && pair_5 && pair_6) return true;

    else return false;
}


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS



// =========================================================================================== OPENCV PART OF THE STATE FUNCTIONS

void opencv_setup_fc()
{
    test_cv_mat = new cv::Mat();

    translated_opencv_mat_texture = SDL_CreateTexture(

        this_app.renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        500, 
        500

    );

    if (TEST_MODE) std::cout << "Mat and texture created\n" << std::endl;
}


void opencv_update_fc()
{
    if (file_choose_info.panels_states.file_1_panel_state == file_choose_panel_state::EMPTY_STATE)
    {
        reset_opencv_data = true;

        // Show PSMVA textbox instead
        File_preview_texture->set_visible_flag(false);
    }

    // Check if the 1st file is choosen
    if (file_choose_info.panels_states.file_1_panel_state == file_choose_panel_state::CHOSEN_STATE)
    {
        
        if (TEST_MODE) std::cout << "FILE CHOOSEN! Reset the texture to the one from OPENCV!\n" << std::endl;

        // Obtain the file by mat
        std::string file_path = file_choose_info.file_1_path;


        if (video_capture_device == nullptr) 
        {
            video_capture_device = new cv::VideoCapture(file_path);

            
            if (TEST_MODE) std::cout << "New video capture!\n" << std::endl;
        } 
        
        else
        {
            // Open only new files
            if (reset_opencv_data)
            {
                video_capture_device->open(file_path);
                reset_opencv_data = false;


                // Start showing the videofile
                File_preview_texture->set_visible_flag(true);

                
                if (TEST_MODE) std::cout << "Capture reset!\n" << std::endl;
            }
        }

        if (!video_capture_device->isOpened())
        {
            SDL_Log("Error: can't open videofile: %s", file_path.c_str());
            return;
        }

        else
        {

            *video_capture_device >> *test_cv_mat;

            if (TEST_MODE) std::cout << "Capture passed to MAT!\n" << std::endl;


            if (test_cv_mat->empty())
            {
                SDL_Log("Rewind to the start of the video.");
                
                // Rewind
                video_capture_device->set(cv::CAP_PROP_POS_FRAMES, 0);
                
                // Read first frame
                *video_capture_device >> *test_cv_mat;
            }

            // Renew basic texture sizes
            if (translated_opencv_mat_texture != nullptr) 
            {
                
                if (TEST_MODE) std::cout << "OPENCV - SDL translation started!\n" << std::endl;

                translate_cv_mat_to_sdl_texture(test_cv_mat, translated_opencv_mat_texture, this_app.renderer);

                if (TEST_MODE) std::cout << "OPENCV - SDL translation ended!\n" << std::endl;


                
                if (TEST_MODE) std::cout << "Start texture pass to My_SDL_Texture!\n" << std::endl;

                int target_width = File_preview_texture->get_width_size();
                int target_height = File_preview_texture->get_height_size();

                // Renew my_SDL_texture by link to curr texture with target resize
                File_preview_texture->set_texture(translated_opencv_mat_texture, false);

                if (TEST_MODE) std::cout << "Texture pass to My_SDL_Texture complete!\n" << std::endl;


                File_preview_texture->set_size(target_width, target_height);

                if (TEST_MODE) std::cout << "Texture sizes changed to target inside My_SDL_Texture!\n" << std::endl;


                SDL_SetTextureBlendMode(translated_opencv_mat_texture, SDL_BLENDMODE_NONE);
            }
        }

    }
}


void opencv_render_by_translator_fc(SDL_Renderer* renderer)
{
    // Pass

    // Data rendered by the Video_panel->render(renderer); 
    // inside the basic render function

    // Preview panel

    Video_panel->render(renderer);

}


void file_choose_opencv_free_and_nullptr()
{

    // 1. Clear mat
    if (test_cv_mat != nullptr)
    {
        delete test_cv_mat;

        test_cv_mat = nullptr; 
    }

    // 2. Clear texture SDL: by inner destroy function
    if (translated_opencv_mat_texture != nullptr)
    {
        SDL_DestroyTexture(translated_opencv_mat_texture); // Clear VRAM и RAM

        translated_opencv_mat_texture = nullptr; 
    }

}

// =========================================================================================== OPENCV PART OF THE STATE FUNCTIONS