// txt_parse.h


#pragma once

// =========================================================================================== IMPORT

#include <string.h>

#include <iostream>

// For parsing

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// =========================================================================================== IMPORT


// =========================================================================================== GLOBAL DATA FOR APP



enum class txt_parse_status
{

    EMPTY_STATE,
    PARSED_STATE

};


struct parsed_video_data
{
    unsigned int bit_depth = 0;


    unsigned int frames_quantity = 0;

    unsigned int frames_per_second = 0;

    float frame_time = 0;


    float exposition_time = 0; 


    unsigned int width = 0;
    unsigned int height = 0;


    txt_parse_status status = txt_parse_status::EMPTY_STATE;

};


struct global_parsed_video_data
{

    parsed_video_data video_1_data;
    parsed_video_data video_2_data;
    parsed_video_data video_3_data;
    parsed_video_data video_4_data;
    parsed_video_data video_5_data;
    parsed_video_data video_6_data;

};


// Global variable for access through the all states
inline global_parsed_video_data files_metadata;


// =========================================================================================== GLOBAL DATA FOR APP


// =========================================================================================== MODULE FUNCTION

/*

Default .txt from camera example:


    Модуль Программного Комплекса ВСКАМ версия 1-ВСКАМ-КОНТРОЛЬ [Evercam 2000-16-C] | SN:378 | Rev:12
    Кадров в секунду: 7500
    Экспозиция [с]: 1/15000
    Усиление: 1
    Разрешение [пикс.]: 512x96
    Смещение [пикс.]: 0x0
    Разрядность: 8 bit
    Дебайер: включён
    Калиброванный кадр: Да
    Запись не по циклу
    Номер начального кадра: 14001
    Номер конечного кадра: 16000
    Яркость: -68
    Контраст: 64


Additional format of .txt for work with other cameras:

    BIT_DEPTH: 8
    FRAMES_QUANTITY: 2000
    FRAMES_PER_SECOND: 7500
    EXPOSITION_TIME: 0.000066
    WIDTH: 512
    HEIGHT: 96

*/

enum class txt_format_type
{

    DEFAULT_FORMAT_EVERCAM_200016C,
    CUSTOM_FORMAT_1

};


// Main function of module
void parse_metadata_txt(parsed_video_data& video, std::string txt_link);


// Helper
void drop_metadata(parsed_video_data& video);



// =========================================================================================== MODULE FUNCTION