// program_states.h

#pragma once

// =========================================================================================== IMPORT

#include <vector>
#include <string>
#include <utility> // for std::pair


#include "../../engine/engine.h"
#include "../../program_gui/basic_elements/global_palette/global_palette.h"
#include "../../program_gui/basic_elements/global_fonts/global_fonts.h"
#include "../global_data/global_data.h"

// =========================================================================================== IMPORT


// =========================================================================================== STATE IDS

/**
 * @brief Unique hierarchical identifiers for all game states.
 *
 * Each ID defines the position of a state in the state machine hierarchy.
 * This makes it easy to determine parent-child relationships and manage
 * nested states.
 */

const State_ID START_ID                             = {0};           // Initial boot/start state


const State_ID MAIN_MENU_ID                         = {1};           // Main functions basic state

const State_ID FILE_CHOOSE_ID                       = {1, 1};        // File choose state

const State_ID MASKS_SETUP_ID                       = {1, 2};        // Masks setup state

const State_ID MASKS_SETUP_ID_1                     = {1, 2, 1};     // Masks setup state
const State_ID MASKS_SETUP_ID_2                     = {1, 2, 2};     // Masks setup state
const State_ID MASKS_SETUP_ID_3                     = {1, 2, 3};     // Masks setup state
const State_ID MASKS_SETUP_ID_4                     = {1, 2, 4};     // Masks setup state
const State_ID MASKS_SETUP_ID_5                     = {1, 2, 5};     // Masks setup state
const State_ID MASKS_SETUP_ID_6                     = {1, 2, 6};     // Masks setup state


const State_ID FLOW_PARAMETERS_CALCULATION_ID       = {1, 3};        // Flow parameters calculation state


const State_ID REPORT_FORMING_ID                    = {1, 4};        // Report forming state


const State_ID PROGRAM_END_ID                       = {2};           // Program exit state

/**
 * @brief Mapping of State_IDs to human-readable names.
 *
 * Useful for debugging, logging, or automatic state registration.
 */
const std::vector<std::pair<State_ID, std::string>> state_defs = {

    {{0},               "START"},                           // Просто запуск программы, загрузка чего-то, подготовка к работе
    {{1},               "MAIN_MENU"},                       // Дать возможность начать работу с программой или выйти
    {{1, 1},            "FILE_CHOOSE"},                     // Меню выбора файлов для обработки    
    {{1, 2},            "MASKS_SETUP"},                     // Меню выбора параметров масок для обработки видео
    {{1, 2, 1},         "MASKS_SETUP_1"},                   // Меню выбора параметров масок для обработки видео
    {{1, 2, 2},         "MASKS_SETUP_2"},                   // Меню выбора параметров масок для обработки видео
    {{1, 2, 3},         "MASKS_SETUP_3"},                   // Меню выбора параметров масок для обработки видео
    {{1, 2, 4},         "MASKS_SETUP_4"},                   // Меню выбора параметров масок для обработки видео
    {{1, 2, 5},         "MASKS_SETUP_5"},                   // Меню выбора параметров масок для обработки видео
    {{1, 2, 6},         "MASKS_SETUP_6"},                   // Меню выбора параметров масок для обработки видео  
    {{1, 3},            "FLOW_PARAMETERS_CALCULATION"},     // Расчёт + статус бар + прогресс бар по каждому видео + сигнализация по успеху (с переходом вперед) неудаче (переход назад + лог)
    {{1, 4},            "REPORT_FORMING"},                  // Формирование отчёта + выбор места сохранения + сигнализация об успехе + переход к следующему этапу
    {{2},               "PROGRAM_END"}                      // Сообщение о завершении работы + переход на этап MAIN_MENU при нажатии любой клавиши

};

// =========================================================================================== STATE IDS


// =========================================================================================== CALLBACKS


// =========================================================================================== CALLBACKS


// =========================================================================================== INITIALIZATION

/**
 * @brief Initializes all game states and adds them to the state machine.
 *
 * - Creates State objects for each defined ID and name.
 * - Assigns the corresponding enter and exit callbacks.
 * - Automatically links parent and child states based on hierarchical IDs.
 *
 * @param app_state_machine Reference to the State_machine to populate.
 */
void init_program_states(State_machine& app_state_machine);

// =========================================================================================== INITIALIZATION
