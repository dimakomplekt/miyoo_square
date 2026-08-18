// 1.2_MASKS_SETUP.h


#pragma once

// =========================================================================================== IMPORT

#include "../../../../engine/engine.h"

#include "../../../../program_gui/basic_elements/global_palette/global_palette.h"
#include "../../../../program_gui/basic_elements/global_fonts/global_fonts.h"

#include "../../../global_data/global_data.h"


#include "../../../../program_gui/basic_elements/UI_elements/my_sdl_texture/my_sdl_texture.h"

#include <array>

// For frame processor
#include <functional>

// =========================================================================================== IMPORT


// =========================================================================================== MAIN STATE API

/**
 * Functions called when entering or exiting a specific state.
 *
 * These are assigned to the `on_enter` and `on_exit` callbacks of the
 * corresponding State objects in the state machine.
 *
 * Example:
 * 
 * app_state_machine.add_state(std::make_unique<State>(START_ID, "START"));
 * state->on_enter = start_enter;
 * 
 */

void masks_setup_enter();
void masks_setup_exit();

void masks_setup_update();
void masks_setup_render(SDL_Renderer* renderer);

// =========================================================================================== MAIN STATE API


// =========================================================================================== DATA


inline constexpr std::array<int, 20> nozzle_diameters = {

    1, 2, 3,
    4, 5, 6,
    7, 8, 9,
    10, 11, 12,
    13, 14, 15,
    16, 17, 18,
    19, 20

};

// =========================================================================================== DATA


// =========================================================================================== GLOBAL DATA

// Mask for the nozzle detection
struct nozzle_detection_mask
{
    // Top point of the nozzle

    int x_1;
    int y_1;

    // Bottom point of the nozzle

    int x_2;
    int y_2;


    // Nozzle diameter
    int d_n;

    // First update as 50% faders at everything
    bool initialized = false;

};


// Mask for the nozzle detection
struct jet_detection_mask
{

};


// Mask for the nozzle detection
struct particle_detection_mask
{

};


// Context for one file
struct file_masks_data
{
    bool* file_choose_state;


    nozzle_detection_mask nozzle_mask;
    jet_detection_mask jet_mask;
    particle_detection_mask particle_mask;

};


struct files_masks_data
{

    file_masks_data file_1_masks;
    file_masks_data file_2_masks;
    file_masks_data file_3_masks;
    file_masks_data file_4_masks;
    file_masks_data file_5_masks;
    file_masks_data file_6_masks;

};

// Data for masks setup
inline files_masks_data masks_data;

// =========================================================================================== GLOBAL DATA


// =========================================================================================== GLOBAL OPENCV PART

// !!! WARNING !!!


// This data is handled using a dynamic memory allocation pattern but is deallocated in a non-standard way
// (relative to the program's overall style).

// INIT - AT THE STATE 1.2
// DELETE - AT THE STATE 1.4 (CAUSE THE DATA WILL BE USED AT THE STATES 1.2, 1.3)

// There could be 6 files with 3 equal masks at each
// so the profitable way to implement them is to
// create pipeline data for 1 and use it to all 6,
// just change the parameters of the masks with different states, 
// but continue to work with 1 video capture, 3 global MATs and 
// 1 translated texture (with passed MAT change with current mask change)

// !!! All this parts will be initiated (with reinit block) at the ENTER of state 1.2 !!!

// !!! And will be destroyed at the ENTER of state 1.4 !!!

// Basic initialization will be performed in state 1.2.
// At the states 1.2.1 - 1.2.6 additional settings (like the choose of the masks parameters 
// for each file, choose of the My_SDL_Texture* which will contain translsated_texture_global) will be performed


// !!! WARNING !!!


// ===== DATA =====

// Global capture
extern cv::VideoCapture* video_capture_device_global;


// Global MAT for capture frameus

// Nozzle mask
extern cv::Mat* video_cv_mat_mask_1_global;

// Jet mask
extern cv::Mat* video_cv_mat_mask_1_global;

// Particle trace mask
extern cv::Mat* video_cv_mat_mask_3_global;


// Global texture for test_cv_mat_translation
extern SDL_Texture* translated_texture_global;

// Global reinit block flag
extern bool opencv_pipeline_reset_global;

// ===== DATA =====


// ===== Functions =====


void opencv_global_setup();



//Callback which takes the Mat and modify it
using frame_processor = std::function<void(cv::Mat*)>;


enum current_file_ms
{

    FILE_1_CF,
    FILE_2_CF,
    FILE_3_CF,
    FILE_4_CF,
    FILE_5_CF,
    FILE_6_CF,

    LIMIT_CF

};


enum current_mask_ms
{

    MASK_1_CM,
    MASK_2_CM,
    MASK_3_CM,

    LIMIT_CM

};


enum video_playback_state
{

    VIDEO_PLAYING_VPS,
    VIDEO_PAUSED_VPS,

    LIMIT_VPS

};


// Global variable for current file check


struct opencv_update_ctx
{

    // Current file (uses for translator setup)
    current_file_ms current_file_for_mask_setup;

    // Current mask (uses for CV::MAT choose)
    current_mask_ms current_mask_for_mask_setup;

    // Uses for translated_texture_global texture pass
    // to texture containers at different states
    My_SDL_texture* current_texture_container;


    // Pointer to the frame_processor
    frame_processor current_frame_processor = nullptr;

    // Play or pause
    video_playback_state playback_state;

    // Current frame number
    int current_frame_index;

    // Total frame number
    // initiates at the switch_videp()
    int total_frame_count;

    // Need reset flag for switch_video call in update function
    bool need_reset;

    // Need to show X3 scaled cv::Mat in other window 
    bool show_kingsize;

    // Flag for other window init logic
    bool kingsize_live_transmission;
    
};


extern opencv_update_ctx opencv_global_update_ctx;


// Helper-function for video switch
void switch_video(const std::string& new_file_path);


// Global update function, which
// works different in depending of current structure 
// (opencv_global_update_ctx) data 
void opencv_global_update();



void opencv_global_free_and_nullptr();

// ===== Functions =====

// =========================================================================================== GLOBAL OPENCV PART



// =========================================================================================== ADDITIONAL STATES API

void masks_setup_1_enter();
void masks_setup_1_exit();

void masks_setup_1_update();
void masks_setup_1_render(SDL_Renderer* renderer);


void masks_setup_2_enter();
void masks_setup_2_exit();

void masks_setup_2_update();
void masks_setup_2_render(SDL_Renderer* renderer);


void masks_setup_3_enter();
void masks_setup_3_exit();

void masks_setup_3_update();
void masks_setup_3_render(SDL_Renderer* renderer);


void masks_setup_4_enter();
void masks_setup_4_exit();

void masks_setup_4_update();
void masks_setup_4_render(SDL_Renderer* renderer);


void masks_setup_5_enter();
void masks_setup_5_exit();

void masks_setup_5_update();
void masks_setup_5_render(SDL_Renderer* renderer);


void masks_setup_6_enter();
void masks_setup_6_exit();

void masks_setup_6_update();
void masks_setup_6_render(SDL_Renderer* renderer);


// =========================================================================================== ADDITIONAL STATES API