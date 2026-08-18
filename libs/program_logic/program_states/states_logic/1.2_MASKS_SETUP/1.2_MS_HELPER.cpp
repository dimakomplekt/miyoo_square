// 1.2_MS_HELPER.cpp

// =========================================================================================== IMPORT

#include "1.2_MS_HELPER.h"

// For parsing

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>



// Files metadata
#include "../1.1_FILE_CHOOSE/1.1_FILE_CHOOSE.h"

// Controlled masks ctx and nozzle diameters
#include "1.2_MASKS_SETUP.h"

// =========================================================================================== IMPORT


// =========================================================================================== HELPERS


// =========================================================================================== FILE SELECTOR

file_masks_data* get_file_masks(int file_number)
{

    switch (file_number)
    {

        case 1:
            return &masks_data.file_1_masks;

        case 2:
            return &masks_data.file_2_masks;

        case 3:
            return &masks_data.file_3_masks;

        case 4:
            return &masks_data.file_4_masks;

        case 5:
            return &masks_data.file_5_masks;

        case 6:
            return &masks_data.file_6_masks;

        default:
            return nullptr;

    }

}


parsed_video_data* get_file_metadata(int file_number)
{

    switch (file_number)
    {

        case 1:
            return &files_metadata.video_1_data;

        case 2:
            return &files_metadata.video_2_data;

        case 3:
            return &files_metadata.video_3_data;

        case 4:
            return &files_metadata.video_4_data;

        case 5:
            return &files_metadata.video_5_data;

        case 6:
            return &files_metadata.video_6_data;

        default:
            return nullptr;

    }

}


// =========================================================================================== FILE SELECTOR


// =========================================================================================== FILESYSTEM

std::string save_file_dialog()
{
    char file_path[MAX_PATH] = { 0 };

    OPENFILENAMEA dialog = {};

    dialog.lStructSize = sizeof(OPENFILENAMEA);
    dialog.lpstrFile = file_path;
    dialog.nMaxFile = MAX_PATH;

    dialog.lpstrFilter =
        "Mask preset (*.txt)\0*.txt\0"
        "All files (*.*)\0*.*\0";

    dialog.nFilterIndex = 1;

    dialog.lpstrDefExt = "txt";

    dialog.Flags =
        OFN_PATHMUSTEXIST |
        OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&dialog))
    {
        return std::string(file_path);
    }

    return "";
}


std::string load_file_dialog()
{
    char file_path[MAX_PATH] = { 0 };

    OPENFILENAMEA dialog = {};

    dialog.lStructSize = sizeof(OPENFILENAMEA);
    dialog.lpstrFile = file_path;
    dialog.nMaxFile = MAX_PATH;

    dialog.lpstrFilter =
        "Mask preset (*.txt)\0*.txt\0"
        "All files (*.*)\0*.*\0";

    dialog.nFilterIndex = 1;

    dialog.Flags =
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&dialog))
    {
        return std::string(file_path);
    }

    return "";
}

// =========================================================================================== FILESYSTEM



// =========================================================================================== SAVE


void save_mask_preset(int file_number)
{

    // TEST
    std::cout << "CALL SAVE FUNCTION TO FILE " << file_number << ".\n\n";


    // ===== Get selected file =====

    file_masks_data* used_file = get_file_masks(file_number);

    if (used_file == nullptr)
    {
        std::cout << "Wrong file number!\n" << std::endl;
        return;
    }


    // ===== Get save path =====

    std::string file_path = save_file_dialog();

    if (file_path.empty())
    {
        std::cout << "Save cancelled!\n" << std::endl;
        return;
    }


    // ===== Open =====

    std::ofstream file(file_path);

    if (!file.is_open())
    {
        std::cout << "Can't open preset file for writing!\n"
                  << std::endl;
        return;
    }



    // ===== Write NOZZLE MASK =====

    file << "NOZZLE_MASK\n";
    file << "\n";

    file << "X1: " << used_file->nozzle_mask.x_1 << "\n";
    file << "Y1: " << used_file->nozzle_mask.y_1 << "\n";
    file << "X2: " << used_file->nozzle_mask.x_2 << "\n";
    file << "Y2: " << used_file->nozzle_mask.y_2 << "\n";
    file << "Dn: " << used_file->nozzle_mask.d_n << "\n";

    file << "\n";

    // ===== Write NOZZLE MASK =====

    
    // ===== Write =====

    file.close();

}


// =========================================================================================== SAVE


// =========================================================================================== LOAD

// =========================================================================================== LOAD

void load_mask_preset(int file_number)
{

    // TEST
    std::cout << "CALL LOAD FUNCTION TO FILE "
              << file_number
              << ".\n\n";


    // ===== GET FILE =====

    file_masks_data* used_file = get_file_masks(file_number);
    

    if (used_file == nullptr)
    {
        std::cout << "Wrong file number!\n" << std::endl;
        return;
    }


    parsed_video_data* used_video = get_file_metadata(file_number);

    if (used_video == nullptr)
    {
        std::cout << "Wrong file number!\n" << std::endl;
        return;
    }


    // ===== GET FILE =====


    // ===== SELECT FILE =====

    std::string file_path = load_file_dialog();

    if (file_path.empty())
    {
        std::cout << "Load cancelled!\n" << std::endl;
        return;
    }


    // ===== SELECT FILE =====


    // ===== OPEN FILE =====

    std::ifstream file(file_path);

    if (!file.is_open())
    {
        std::cout << "Can't open preset file!\n"
                  << std::endl;
        return;
    }


    // ===== OPEN FILE =====


    // ===== COPY ROWS =====

    std::vector<std::string> txt_lines;

    std::string line;

    while (std::getline(file, line))
    {
        txt_lines.push_back(line);
    }

    file.close();

    // ===== COPY ROWS =====


    // ======================================================================================= FORMAT CHECK


    // We don't care about the number or order of empty lines.
    // We search for required fields by their keys.

    bool nozzle_mask_found = false;

    bool x_1_found = false;
    bool y_1_found = false;
    bool x_2_found = false;
    bool y_2_found = false;
    bool d_n_found = false;


    std::string tmp_x_1;
    std::string tmp_y_1;
    std::string tmp_x_2;
    std::string tmp_y_2;
    std::string tmp_d_n;


    for (const std::string& current_line : txt_lines)
    {

        // ===== NOZZLE MASK =====

        if (current_line == "NOZZLE_MASK")
        {
            nozzle_mask_found = true;
        }


        // ===== X1 =====

        else if (current_line.rfind("X1:", 0) == 0)
        {
            if (x_1_found)
            {
                std::cout << "Duplicate X1 field!\n"
                          << std::endl;
                return;
            }

            tmp_x_1 = current_line.substr(3);

            // Remove leading spaces after "X1:"
            size_t first_not_space = tmp_x_1.find_first_not_of(' ');

            if (first_not_space != std::string::npos)
            {
                tmp_x_1 = tmp_x_1.substr(first_not_space);
            }
            else
            {
                tmp_x_1.clear();
            }

            x_1_found = true;
        }


        // ===== Y1 =====

        else if (current_line.rfind("Y1:", 0) == 0)
        {
            if (y_1_found)
            {
                std::cout << "Duplicate Y1 field!\n"
                          << std::endl;
                return;
            }

            tmp_y_1 = current_line.substr(3);

            size_t first_not_space = tmp_y_1.find_first_not_of(' ');

            if (first_not_space != std::string::npos)
            {
                tmp_y_1 = tmp_y_1.substr(first_not_space);
            }
            else
            {
                tmp_y_1.clear();
            }

            y_1_found = true;
        }


        // ===== X2 =====

        else if (current_line.rfind("X2:", 0) == 0)
        {
            if (x_2_found)
            {
                std::cout << "Duplicate X2 field!\n"
                          << std::endl;
                return;
            }

            tmp_x_2 = current_line.substr(3);

            size_t first_not_space = tmp_x_2.find_first_not_of(' ');

            if (first_not_space != std::string::npos)
            {
                tmp_x_2 = tmp_x_2.substr(first_not_space);
            }
            else
            {
                tmp_x_2.clear();
            }

            x_2_found = true;
        }


        // ===== Y2 =====

        else if (current_line.rfind("Y2:", 0) == 0)
        {
            if (y_2_found)
            {
                std::cout << "Duplicate Y2 field!\n"
                          << std::endl;
                return;
            }

            tmp_y_2 = current_line.substr(3);

            size_t first_not_space = tmp_y_2.find_first_not_of(' ');

            if (first_not_space != std::string::npos)
            {
                tmp_y_2 = tmp_y_2.substr(first_not_space);
            }
            else
            {
                tmp_y_2.clear();
            }

            y_2_found = true;
        }


        // ===== Dn =====

        else if (current_line.rfind("Dn:", 0) == 0)
        {
            if (d_n_found)
            {
                std::cout << "Duplicate Dn field!\n"
                          << std::endl;
                return;
            }

            tmp_d_n = current_line.substr(3);

            size_t first_not_space = tmp_d_n.find_first_not_of(' ');

            if (first_not_space != std::string::npos)
            {
                tmp_d_n = tmp_d_n.substr(first_not_space);
            }
            else
            {
                tmp_d_n.clear();
            }

            d_n_found = true;
        }

    }


    // ===== CHECK REQUIRED FIELDS =====

    if (!nozzle_mask_found ||
        !x_1_found ||
        !y_1_found ||
        !x_2_found ||
        !y_2_found ||
        !d_n_found)
    {
        std::cout << "Wrong type of the passed file!\n"
                  << std::endl;

        return;
    }


    // ======================================================================================= FORMAT CHECK


    // ======================================================================================= DATA CHECK

    bool correct_data = true;


    // Temporary values.
    // Real masks_data will be changed only after all checks pass.

    int x_1_for_pass;
    int y_1_for_pass;
    int x_2_for_pass;
    int y_2_for_pass;
    int d_n_for_pass;


    // ======================================================================================= CHECK X1

    if (tmp_x_1.empty())
    {
        correct_data = false;
    }


    if (correct_data &&
        tmp_x_1.find_first_not_of("0123456789") != std::string::npos)
    {
        correct_data = false;
    }


    if (correct_data)
    {

        int value = std::stoi(tmp_x_1);


        if (value < 0 ||
            value > static_cast<int>(used_video->width))
        {
            correct_data = false;
        }
        else
        {
            x_1_for_pass = value;
        }

    }


    if (!correct_data)
    {
        std::cout << "Wrong X1 value!\n"
                  << std::endl;

        return;
    }


    // ======================================================================================= CHECK X1


    // ======================================================================================= CHECK Y1

    correct_data = true;


    if (tmp_y_1.empty())
    {
        correct_data = false;
    }


    if (correct_data &&
        tmp_y_1.find_first_not_of("0123456789") != std::string::npos)
    {
        correct_data = false;
    }


    if (correct_data)
    {

        int value = std::stoi(tmp_y_1);


        if (value < 0 ||
            value > static_cast<int>(used_video->height))
        {
            correct_data = false;
        }
        else
        {
            y_1_for_pass = value;
        }

    }


    if (!correct_data)
    {
        std::cout << "Wrong Y1 value!\n"
                  << std::endl;

        return;
    }


    // ======================================================================================= CHECK Y1


    // ======================================================================================= CHECK X2

    correct_data = true;


    if (tmp_x_2.empty())
    {
        correct_data = false;
    }


    if (correct_data &&
        tmp_x_2.find_first_not_of("0123456789") != std::string::npos)
    {
        correct_data = false;
    }


    if (correct_data)
    {

        int value = std::stoi(tmp_x_2);


        if (value < 0 ||
            value > static_cast<int>(used_video->width))
        {
            correct_data = false;
        }
        else
        {
            x_2_for_pass = value;
        }

    }


    if (!correct_data)
    {
        std::cout << "Wrong X2 value!\n"
                  << std::endl;

        return;
    }


    // ======================================================================================= CHECK X2


    // ======================================================================================= CHECK Y2

    correct_data = true;


    if (tmp_y_2.empty())
    {
        correct_data = false;
    }


    if (correct_data &&
        tmp_y_2.find_first_not_of("0123456789") != std::string::npos)
    {
        correct_data = false;
    }


    if (correct_data)
    {

        int value = std::stoi(tmp_y_2);


        if (value < 0 ||
            value > static_cast<int>(used_video->height))
        {
            correct_data = false;
        }
        else
        {
            y_2_for_pass = value;
        }

    }


    if (!correct_data)
    {
        std::cout << "Wrong Y2 value!\n"
                  << std::endl;

        return;
    }


    // ======================================================================================= CHECK Y2


    // ======================================================================================= CHECK DN

    correct_data = true;


    if (tmp_d_n.empty())
    {
        correct_data = false;
    }


    if (correct_data &&
        tmp_d_n.find_first_not_of("0123456789") != std::string::npos)
    {
        correct_data = false;
    }


    if (correct_data)
    {

        int value = std::stoi(tmp_d_n);


        bool diameter_found = false;


        for (size_t i = 0; i < nozzle_diameters.size(); ++i)
        {

            if (value == nozzle_diameters[i])
            {
                diameter_found = true;
                break;
            }

        }


        if (!diameter_found)
        {
            correct_data = false;
        }
        else
        {
            d_n_for_pass = value;
        }

    }


    if (!correct_data)
    {
        std::cout << "Wrong nozzle diameter!\n"
                  << std::endl;

        return;
    }


    // ======================================================================================= CHECK DN



    // ======================================================================================= FINAL FILL

    // Everything is correct.
    // Now and only now modify global data.

    used_file->nozzle_mask.x_1 = x_1_for_pass;
    used_file->nozzle_mask.y_1 = y_1_for_pass;

    used_file->nozzle_mask.x_2 = x_2_for_pass;
    used_file->nozzle_mask.y_2 = y_2_for_pass;

    used_file->nozzle_mask.d_n = d_n_for_pass;

    used_file->nozzle_mask.initialized = true;


    // ======================================================================================= FINAL FILL


    std::cout << "Mask preset loaded successfully!\n"
              << std::endl;

}

// =========================================================================================== LOAD


// =========================================================================================== HELPERS