// txt_parse.cpp


// =========================================================================================== IMPORT

#include "txt_parse.h"


// =========================================================================================== IMPORT


// =========================================================================================== MODULE FUNCTION

const std::vector<std::string> evercam_rows =
{
    "Модуль Программного Комплекса",
    "Кадров в секунду:",
    "Экспозиция [с]:",
    "Усиление:",
    "Разрешение [пикс.]:",
    "Смещение [пикс.]:",
    "Разрядность:",
    "Дебайер:",
    "Калиброванный кадр:",
    "Запись",
    "Номер начального кадра:",
    "Номер конечного кадра:",
    "Яркость:",
    "Контраст:"
};


const std::vector<std::string> custom_rows =
{
    "BIT_DEPTH:",
    "FRAMES_QUANTITY:",
    "FRAMES_PER_SECOND:",
    "EXPOSITION_TIME:",
    "WIDTH:",
    "HEIGHT:"
};


// Helper
std::string get_value(const std::string& line);


void parse_metadata_txt(parsed_video_data& video, std::string txt_link)
{
    // Go to file
    // Check format by txt_format_type
    // Error handle
    // Parse by format (just by string -> float / int translator and rows numbers)
    // Error handle data
    // Set new data and new status by txt_parse_status


    // ===== Open =====

    std::ifstream file(txt_link);

    // Check 1
    if (!file.is_open())
    {
        std::cout << "Wrong file pass!\n" << std::endl;

        drop_metadata(video);

        return;
    }

    
    // ===== Open =====


    // ===== Copy rows =====

    
    // Getline could only know where the current row ends, so we need to
    // pass them all to vector, for easier workflow

    std::vector<std::string> txt_lines;

    std::string line;


    while (std::getline(file, line))
    {
        txt_lines.push_back(line);
    }


    // Empty file error handler
    if (txt_lines.empty() || (txt_lines.size() != 14 && txt_lines.size() != 6))
    {
        std::cout << "Wrong type of the passed file!\n" << std::endl;
        drop_metadata(video);
        return;
    }


    // Format pre-check
    if (txt_lines.empty() || (txt_lines.size() != 14 && txt_lines.size() != 6))
    {
        std::cout << "Wrong type of the passed file!\n" << std::endl;

        drop_metadata(video);

        return;
    }

    // ===== Copy rows =====


    // ===== Format check =====

    txt_format_type format;

    // Check format
    
    // Not the best logic, but it's ok
    // for this app version
    switch (txt_lines.size())
    {
        case 14:

            format = txt_format_type::DEFAULT_FORMAT_EVERCAM_200016C;


            break;


        case 6:

            format = txt_format_type::CUSTOM_FORMAT_1;


            break;
        

        default: break;
    }
    

    // 1st error handle of all the rows

    bool format_found = true;

    switch (format)
    {

        case txt_format_type::DEFAULT_FORMAT_EVERCAM_200016C:

            for (size_t i = 0; i < evercam_rows.size(); ++i)
            {
                if (txt_lines[i].find(evercam_rows[i]) == std::string::npos)
                {
                    format_found = false;
                    break;
                }
            }
           
            break;


        case txt_format_type::CUSTOM_FORMAT_1:
            

            for (size_t i = 0; i < custom_rows.size(); ++i)
            {
                if (txt_lines[i].find(custom_rows[i]) == std::string::npos)
                {
                    format_found = false;
                    break;
                }
            }
        
            break;

        
        default: break;
    }

    if (!format_found)
    {
        std::cout << "Wrong type of the passed file!\n" << std::endl;

        drop_metadata(video);

        return;
    }


    // 2nd error handling of all the rows
    // and data fill with good check

    // Parse to temporary strings
    
    switch (format)
    {

        case txt_format_type::DEFAULT_FORMAT_EVERCAM_200016C:
        {
            // Parse

            std::string tmp_bit_depth = get_value(txt_lines[6]);
            std::string tmp_start_frame = get_value(txt_lines[10]);
            std::string tmp_end_frame = get_value(txt_lines[11]);
            std::string tmp_frames_per_second = get_value(txt_lines[1]);
            std::string tmp_exposition_time = get_value(txt_lines[2]); 
            std::string tmp_width_and_height = get_value(txt_lines[4]);
    

// =========================================================================================== CHECK 1
            
            // Preblock the update
            format_found = false; 
            
            // Check
            bool correct_data = true;

            // Check empty pass
            correct_data =

                !tmp_bit_depth.empty() &&
                !tmp_start_frame.empty() &&
                !tmp_end_frame.empty() &&
                !tmp_frames_per_second.empty() &&
                !tmp_exposition_time.empty() &&
                !tmp_width_and_height.empty();


            // Check pass correction


            // ===== Check bit depth =====

            // Value for pass
            unsigned int bit_depth_for_pass;


            // Check " bit"
            size_t bit_pos = tmp_bit_depth.find(" bit");

            if (bit_pos == std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                // Get numeric part
                std::string bit_depth_value = tmp_bit_depth.substr(0, bit_pos);

                // Check allowed values
                if (bit_depth_value != "8"  &&
                    bit_depth_value != "10" &&
                    bit_depth_value != "12" &&
                    bit_depth_value != "16")
                {
                    correct_data = false;
                }
                else bit_depth_for_pass = std::stoul(bit_depth_value);
            }

            if (!correct_data) break;

            // ===== Check bit depth =====


            // ===== Check frames =====

            // Value for pass
            unsigned int start_frame_for_pass;
            unsigned int end_frame_for_pass;


            // Check digits
            if (tmp_start_frame.find_first_not_of("0123456789") != std::string::npos ||
                tmp_end_frame.find_first_not_of("0123456789") != std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                // Check leading zeros
                if ((tmp_start_frame.size() > 1 && tmp_start_frame[0] == '0') ||
                    (tmp_end_frame.size() > 1 && tmp_end_frame[0] == '0'))
                {
                    correct_data = false;
                }
                else
                {
                    unsigned int start_frame = std::stoul(tmp_start_frame);
                    unsigned int end_frame = std::stoul(tmp_end_frame);

                    if (end_frame <= start_frame)
                    {
                        correct_data = false;
                    }

                    if (end_frame < 0 || start_frame < 0) correct_data = false;


                    // Pass case
                    if (correct_data)
                    {
                        start_frame_for_pass = start_frame;
                        end_frame_for_pass = end_frame;
                    }
                }
            }


            if (!correct_data) break;

            // ===== Check frames =====


            // ===== Check frames per second =====

            unsigned int fps_for_pass;


            if (tmp_frames_per_second.find_first_not_of("0123456789") != std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                unsigned int frames_per_second = std::stoul(tmp_frames_per_second);

                if (frames_per_second == 0)
                {
                    correct_data = false;
                }


                if (correct_data) fps_for_pass = frames_per_second;
            }


            if (!correct_data) break;

            // ===== Check frames per second =====


            // ===== Check exposition =====

            // Value for pass
            float exposition_time_for_pass;


            // Check '/'
            size_t slash_pos = tmp_exposition_time.find('/');

            if (slash_pos == std::string::npos ||
                slash_pos != tmp_exposition_time.rfind('/'))
            {
                correct_data = false;
            }
            else
            {
                std::string numerator   = tmp_exposition_time.substr(0, slash_pos);
                std::string denominator = tmp_exposition_time.substr(slash_pos + 1);

                // Check digits
                if (numerator.find_first_not_of("0123456789") != std::string::npos ||
                    denominator.find_first_not_of("0123456789") != std::string::npos)
                {
                    correct_data = false;
                }
                else
                {
                    unsigned int num = std::stoul(numerator);
                    unsigned int den = std::stoul(denominator);

                    if (num == 0 || den == 0)
                    {
                        correct_data = false;
                    }
                    else
                    {
                        if (correct_data) exposition_time_for_pass = static_cast<float>(num) / static_cast<float>(den);
                    }
                }
            }


            if (!correct_data) break;

            // ===== Check exposition =====


            // ===== Check resolution =====

            unsigned int width_for_pass;
            unsigned int height_for_pass;


            // Check 'x'
            size_t x_pos = tmp_width_and_height.find('x');

            if (x_pos == std::string::npos ||
                x_pos != tmp_width_and_height.rfind('x'))
            {
                correct_data = false;
            }
            else
            {
                std::string width  = tmp_width_and_height.substr(0, x_pos);
                std::string height = tmp_width_and_height.substr(x_pos + 1);

                // Check digits
                if (width.find_first_not_of("0123456789") != std::string::npos ||
                    height.find_first_not_of("0123456789") != std::string::npos)
                {
                    correct_data = false;
                }
                else
                {
                    unsigned int width_value  = std::stoul(width);
                    unsigned int height_value = std::stoul(height);

                    if (width_value == 0 ||
                        height_value == 0)
                    {
                        correct_data = false;
                    }

                    if (correct_data)
                    {
                        width_for_pass = width_value;
                        height_for_pass = height_value;
                    }
                }
            }

            // Last check
            // with the fail state here we holding the false value on the format found
            // with correct pass we return format_found value to true
            if (correct_data) format_found = true;

// =========================================================================================== CHECK 1


// =========================================================================================== FILL 1

            // Fill
            if (format_found)
            {
                video.bit_depth = bit_depth_for_pass;
                video.frames_quantity = end_frame_for_pass - start_frame_for_pass + 1;
                video.frames_per_second = fps_for_pass;
                video.frame_time = 1.0f / fps_for_pass;
                video.exposition_time = exposition_time_for_pass;
                video.width = width_for_pass;
                video.height = height_for_pass;
                video.status = txt_parse_status::PARSED_STATE;
            }
            else
            {
                drop_metadata(video);
            }

            break;
        }
        
// =========================================================================================== FILL 1



        case txt_format_type::CUSTOM_FORMAT_1:
        {
            // Parse

            std::string tmp_bit_depth = get_value(txt_lines[0]);
            std::string tmp_frames_quantity = get_value(txt_lines[1]);
            std::string tmp_frames_per_second = get_value(txt_lines[2]);
            std::string tmp_exposition_time = get_value(txt_lines[3]); 
            std::string tmp_width = get_value(txt_lines[4]);
            std::string tmp_height = get_value(txt_lines[5]);

            // Check
            bool correct_data = false;


            correct_data =

                !tmp_bit_depth.empty() &&
                !tmp_frames_quantity.empty() &&
                !tmp_frames_per_second.empty() &&
                !tmp_exposition_time.empty() &&
                !tmp_width.empty() &&
                !tmp_height.empty();


            if (!correct_data) break;

// =========================================================================================== CHECK 2
            
            // ===== Bit depth =====

            unsigned int bit_depth_for_pass;

            if (tmp_bit_depth != "8" &&
                tmp_bit_depth != "10" &&
                tmp_bit_depth != "12" &&
                tmp_bit_depth != "16")
            {
                correct_data = false;
            }
            else
            {
                bit_depth_for_pass = std::stoul(tmp_bit_depth);
            }

            if (!correct_data) break;

            // ===== Bit depth =====

            
            // ===== Frames quantity =====


            unsigned int frames_quantity_for_pass;

            if (tmp_frames_quantity.find_first_not_of("0123456789") != std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                frames_quantity_for_pass = std::stoul(tmp_frames_quantity);

                if (frames_quantity_for_pass == 0)
                    correct_data = false;
            }

            if (!correct_data) break;

            // ===== Frames quantity =====


            // ===== Check frames per second =====

            unsigned int fps_for_pass;


            if (tmp_frames_per_second.find_first_not_of("0123456789") != std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                unsigned int frames_per_second = std::stoul(tmp_frames_per_second);

                if (frames_per_second == 0)
                {
                    correct_data = false;
                }


                if (correct_data) fps_for_pass = frames_per_second;
            }

            if (!correct_data) break;

            // ===== Check frames per second =====


            // ===== Check exposition time =====
            
            float exposition_time_for_pass;


            size_t dot_pos = tmp_exposition_time.find('.');

            if (dot_pos == std::string::npos ||
                dot_pos != tmp_exposition_time.rfind('.'))
            {
                correct_data = false;
            }
            else
            {
                if (tmp_exposition_time.find_first_not_of("0123456789.") != std::string::npos)
                {
                    correct_data = false;
                }
                else
                {
                    float exposition = std::stof(tmp_exposition_time);

                    if (exposition <= 0.0f)
                        correct_data = false;
                    else
                        exposition_time_for_pass = exposition;
                }
            }

            if (!correct_data) break;

            // ===== Check exposition time =====


            // ===== Dimensions check =====

            unsigned int width_for_pass;

            unsigned int height_for_pass;


            if (tmp_width.find_first_not_of("0123456789") != std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                width_for_pass = std::stoul(tmp_width);

                if (width_for_pass == 0)
                    correct_data = false;
            }

            if (!correct_data) break;


            if (tmp_height.find_first_not_of("0123456789") != std::string::npos)
            {
                correct_data = false;
            }
            else
            {
                height_for_pass = std::stoul(tmp_height);

                if (height_for_pass == 0)
                    correct_data = false;
            }

            if (!correct_data) break;

            // ===== Dimensions check =====


            // Last check
            // with the fail state here we holding the false value on the format found
            // with correct pass we return format_found value to true
            if (correct_data) format_found = true;

// =========================================================================================== CHECK 2


// =========================================================================================== FILL 2

            // Fill
            if (format_found)
            {
                video.bit_depth = bit_depth_for_pass;
                video.frames_quantity = frames_quantity_for_pass;
                video.frames_per_second = fps_for_pass;
                video.frame_time = 1.0f / fps_for_pass;
                video.exposition_time = exposition_time_for_pass;
                video.width = width_for_pass;
                video.height = height_for_pass;
                video.status = txt_parse_status::PARSED_STATE;
            }
            else
            {
                drop_metadata(video);
            }

            break;
        }
// =========================================================================================== FILL 2


        default: break;

    }

}


// Helper
std::string get_value(const std::string& line)
{
    size_t pos = line.find(": ");

    if (pos == std::string::npos)
        return "";

    return line.substr(pos + 2);
}



// Helper
void drop_metadata(parsed_video_data& video)
{

    video.bit_depth = 0;
    video.frames_quantity = 0;
    video.frames_per_second = 0;
    video.frame_time = 0;
    video.exposition_time = 0;
    video.width = 0;
    video.height = 0;
    video.status = txt_parse_status::EMPTY_STATE;

}


// =========================================================================================== MODULE FUNCTION