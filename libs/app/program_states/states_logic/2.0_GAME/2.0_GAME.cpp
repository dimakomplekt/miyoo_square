
// 2.0_GAME.cpp


// =========================================================================================== IMPORT

#include "2.0_GAME.h"


#include "../../../../engine/logic_modules/program_gui/basic_elements/UI_elements/my_sdl_texture/my_sdl_texture.h"


// Predeclare for switching states
#include "../../program_states.h"

#include "../../../app.h"


// Log
#include <iostream>


#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <functional>

// =========================================================================================== IMPORT


// =========================================================================================== STATE DATA

// RAII + lifecycle management


/*

handle_ctx background_1_ah;
handle_ctx background_2_ah;

handle_ctx floor_1_1_ah;
handle_ctx floor_1_2_ah;
handle_ctx floor_1_3_ah;
handle_ctx floor_2_1_ah;
handle_ctx floor_2_2_ah;
handle_ctx floor_2_3_ah;

handle_ctx wall_1_ah;
handle_ctx wall_2_ah;

handle_ctx square_1_ah;
handle_ctx square_2_ah;


*/

Background_sgo* scene_background = nullptr;

Floor_sgo* scene_floor = nullptr;

Wall_sgo* scene_wall_1 = nullptr;
Wall_sgo* scene_wall_2 = nullptr;

Square_sgo* square_1 = nullptr;

// =========================================================================================== STATE DATA


// =========================================================================================== HELPERS

void state_game_objects_create()
{
    background_assets_init();

    scene_background = new Background_sgo();


    floor_assets_init();

    scene_floor = new Floor_sgo();


    wall_assets_init();

    scene_wall_1 = new Wall_sgo();
    scene_wall_2 = new Wall_sgo(*scene_wall_1);


    square_assets_init();

    square_1 = new Square_sgo();

}


void state_game_objects_clear()
{
    delete scene_background;
    scene_background = nullptr;

    delete scene_floor;
    scene_floor = nullptr;

    
    delete scene_wall_1;
    scene_wall_1 = nullptr;


    delete scene_wall_2;
    scene_wall_2 = nullptr;


    delete square_1;
    square_1 = nullptr;


    background_assets_clear();

    floor_assets_clear();

    wall_assets_clear();

    square_assets_clear();
}


// =========================================================================================== HELPERS


// =========================================================================================== CALLBACKS


void exit_to_menu()
{
    // Exit to main menu call
    this_app.app_sm.request_state_change(MAIN_MENU_ID);
}

// =========================================================================================== CALLBACKS


// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


void game_elements_create();

void game_elements_setup();

void game_elements_free_and_nullptr();


void scene_create();
void scene_setup();
void scene_update();
void scene_actions();
void scene_render(SDL_Renderer* renderer);
void scene_destroy();

void game_elements_update();

void reset_passed_by_dictionary_textboxes_if_language_switched_g();

void game_actions();

void game_elements_render(SDL_Renderer* renderer);

// =========================================================================================== STATE INNER FUNCTIONS PREDECLARATION


// =========================================================================================== MAIN STATE API


void game_enter()
{
    // Log the enter in console
    std::cout << "Entering MAIN_MENU\n"; 

    // ===== State GUI allocation =====

    game_elements_create();

    scene_create();

    // ===== State GUI allocation =====


    // Elements setup

    game_elements_setup();

    scene_setup();

}


void game_exit()
{
    // ===== State deallocation =====

    game_elements_free_and_nullptr();

    scene_destroy();

    // ===== State deallocation =====


    // Log the exit in console

    std::cout << "Exiting MAIN_MENU\n"; 

}


void game_update()
{
    // Update inputs
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_1000))
    {
        App_inputs.update();
    }

    if (App_timer_1.can_execute(Execute_zone_ID::HZ_240))
    {
        game_elements_update();

        game_actions();

        scene_actions();
    }
}


void game_render(SDL_Renderer* renderer)
{
    if (App_timer_1.can_execute(Execute_zone_ID::HZ_120))
    {
        game_elements_render(renderer);


        scene_render(renderer);
    }
}

// =========================================================================================== MAIN STATE API


// =========================================================================================== STATE INNER FUNCTIONS REALIZATION

void game_elements_create()
{


}


// ===== SETUP DATA =====

// ===== Main sizes =====

// Static-size GUI

const int BACKGROUND_WIDTH  = MAIN_WINDOW_H_SIZE;
const int BACKGROUND_HEIGHT  = MAIN_WINDOW_V_SIZE;

// ===== SETUP DATA =====

void game_elements_setup()
{     


}



void scene_create()
{
    state_game_objects_create();
}


void scene_setup()
{

    // Background

    scene_background->set_render_point(BACKGROUND_WIDTH / 2, BACKGROUND_HEIGHT / 2);

    scene_floor->set_render_point(BACKGROUND_WIDTH / 2, BACKGROUND_HEIGHT - scene_floor->get_height() / 2); // SDL LOGIC - to center-center render


    scene_wall_1->set_render_point(scene_wall_1->get_width() / 2,scene_wall_1->get_height() / 2); // SDL LOGIC - to center-center render

    scene_wall_2->set_render_point(BACKGROUND_WIDTH - scene_wall_2->get_width() / 2, scene_wall_2->get_height() / 2); // SDL LOGIC - to center-center render


    square_1->set_render_point(BACKGROUND_WIDTH / 2, BACKGROUND_HEIGHT / 2);

    // Floor init


    // Walls init


    // Square inti

}


void scene_destroy()
{

    state_game_objects_clear();

}


void game_elements_free_and_nullptr()
{
    // Protection
   
    // Free all elements


    // Info / settings panel


    // Nullptr the pointers


}



void game_elements_update()
{

    // Check if textboxes need content renew
    reset_passed_by_dictionary_textboxes_if_language_switched_g();

    // Update all elements
    

    scene_update();


}

bool square_grounded = false;


void scene_update()
{
    const movement_request y_request = square_1->movement.get_y_request();

    const bool jump_requested = y_request.regime == JUMP_MR && y_request.direction < 0;

    if (square_grounded && !jump_requested)
    {
        square_1->movement.move_y_request(0, MOVEMENT_MR);
    }
    else if (!jump_requested)
    {
        square_1->movement.move_y_request(1, JUMP_MR);
    }

    square_1->movement.update();

    hitbox_points square_hitbox = square_1->get_hitbox();
    hitbox_points floor_hitbox = scene_floor->get_hitbox();
    hitbox_points left_wall_hitbox = scene_wall_1->get_hitbox();
    hitbox_points right_wall_hitbox = scene_wall_2->get_hitbox();


    collision_result square_to_floor = check_collision(square_hitbox, floor_hitbox);
    collision_result square_to_l_wall = check_collision(square_hitbox, left_wall_hitbox);
    collision_result square_to_r_wall = check_collision(square_hitbox, right_wall_hitbox);


    if (square_to_floor.has_collision &&
        square_to_floor.penetration_y > 0)
    {
        square_1->set_render_point_by_delta(
            0, -std::abs(square_to_floor.penetration_y));

        square_1->movement.move_y_request(0, MOVEMENT_MR);

        square_grounded = true;
    }
    else
    {
        square_grounded = false;
    }


    const int horizontal_direction =
        square_1->movement.get_x_request().direction;


    const collision_result& wall_collision =

        horizontal_direction < 0
            ? square_to_l_wall
            : horizontal_direction > 0
                ? square_to_r_wall
                : (square_1->get_x_render_point() <= BACKGROUND_WIDTH / 2
                    ? square_to_l_wall
                    : square_to_r_wall);


    square_1->motion_processing(wall_collision);
    

    // Collision processing
    if (square_1->get_motion_state() == RECOVERING_SMS)
    {
        // 1. Собираем функции в вектор лямбд
        std::vector<std::function<void()>> functions = {
            [&]() { scene_background->switch_used_asset(); },
            [&]() { scene_floor->switch_used_asset(); },
            [&]() { scene_wall_1->switch_used_asset(); },
            [&]() { scene_wall_2->switch_used_asset(); },
            [&]() { square_1->switch_used_asset(); }
        };

        // 2. Настраиваем генератор случайных чисел
        static std::random_device rd;
        static std::mt19937 gen(rd());
        // Бернауллиевское распределение: дает true или false с вероятностью 0.5 (50%)
        std::bernoulli_distribution dist(0.5); 

        int called_count = 0;

        // 3. Пробуем вызвать каждую функцию с вероятностью 50%
        for (auto& func : functions)
        {
            if (dist(gen)) // С вероятностью 50% условие выполнится
            {
                func();
                called_count++;
            }
        }

        // 4. Если за цикл ни одна функция не выпала, вызываем абсолютно ВСЕ
        if (called_count == 0)
        {
            for (auto& func : functions)
            {
                func();
            }
        }
    }

}




void reset_passed_by_dictionary_textboxes_if_language_switched_g()
{
    // Repeat content set if language switched
    if (App_lang.get_lang_reset_flag())
    {
        //
    }
}



void game_actions()
{
    // Main state actions

    if (App_inputs.is_just_released(Key_actions::EXIT_KA))
    {
        exit_to_menu();
    }
}


void scene_actions()
{

    if (App_inputs.is_held(Key_actions::LEFT_KA))
    {
        square_1->movement.move_x_request(-1, MOVEMENT_MR);
    }


    if (App_inputs.is_just_released(Key_actions::LEFT_KA))
    {
        square_1->movement.move_x_request(0, MOVEMENT_MR);
    }


    if (App_inputs.is_held(Key_actions::RIGHT_KA))
    {
        square_1->movement.move_x_request(1, MOVEMENT_MR);
    }


    if (App_inputs.is_just_released(Key_actions::RIGHT_KA))
    {
        square_1->movement.move_x_request(0, MOVEMENT_MR);
    }


    if (App_inputs.is_pressed(Key_actions::UP_KA))
    {
        square_1->movement.move_y_request(-1, JUMP_MR);
    }


    if (App_inputs.is_just_released(Key_actions::UP_KA))
    {
        square_1->movement.move_y_request(0, MOVEMENT_MR);
    }
}


void game_elements_render(SDL_Renderer* renderer)
{

}


void scene_render(SDL_Renderer* renderer)
{
    scene_background->render(renderer);


    scene_floor->render(renderer);


    scene_wall_1->render(renderer);

    scene_wall_2->render(renderer);

    square_1->render(renderer);
}

// =========================================================================================== STATE INNER FUNCTIONS REALIZATION


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS


// =========================================================================================== STATE ELEMENTS INNER FUNCTIONS
