// 1.2_MS_HELPER.h

#pragma once


// =========================================================================================== IMPORT

#include <string>
#include <vector>

// =========================================================================================== IMPORT


// =========================================================================================== HELPERS



// ===== Save and load presets =====


// Txt-file preset rows
const std::vector<std::string> masks_preset_rows =
{

    "NOZZLE_MASK",
    "",
    "X1:",          // Should be int in ranges of [0, files_metadata.video_X_data.width]
    "Y1:",          // Should be int in ranges of [0, files_metadata.video_X_data.height]
    "X2:",          // Should be int in ranges of [0, files_metadata.video_X_data.width]
    "Y2:",          // Should be int in ranges of [0, files_metadata.video_X_data.height]
    "Dn:",          // Should be int in ranges of [nozzle_diameters[0], nozzle_diameters[nozzle_diameters.size() - 1]]
    ""

};


void save_mask_preset(int file_number);


void load_mask_preset(int file_number);


// ===== Save and load presets =====



// =========================================================================================== HELPERS