// objects_movement.h


#pragma once


// =========================================================================================== IMPORT

#include "../../../engine/engine.h"

#include "../../global_data/global_data.h"

// =========================================================================================== IMPORT


// =========================================================================================== STRUCTS

struct movement_settings
{

    float max_speed = 0.0f;            // Pixels per second
    float acceleration = 0.0f;         // Pixels per second^2
    float deceleration = 0.0f;         // Pixels per second^2


    float jump_max_speed = 0.0f;       // Pixels per second
    float jump_acceleration = 0.0f;    // Pixels per second^2
    float jump_deceleration = 0.0f;    // Pixels per second^2

};


enum movement_regime
{

    MOVEMENT_MR,
    JUMP_MR

};

enum movement_stage
{
    MOVEMENT_IDLE,

    MOVEMENT_MOVE_ATTACK,
    // MOVEMENT_MOVE_DECAY, // No settings right now
    MOVEMENT_MOVE_SUSTAIN,
    MOVEMENT_MOVE_RELEASE,

    MOVEMENT_JUMP,
    MOVEMENT_FALL

};


struct movement_axis_state
{

    float velocity = 0.0f;
    int direction = 1;

    float sub_pixel_ds_acc = 0.0f;

    movement_stage stage = MOVEMENT_IDLE;

};


struct movement_state
{

    movement_axis_state x;
    movement_axis_state y;

};


struct movement_request
{

    int direction = 0;
    movement_regime regime = MOVEMENT_MR;

};


using position_setter =
    std::function<void(int delta_x, int delta_y)>;


// =========================================================================================== STRUCTS


// =========================================================================================== MOVEMENT CLASS


class Movement
{

public:

    // ===== Lifetime =====

    Movement(
        const movement_settings& settings,
        App_timer& timer,
        position_setter setter
    );


    ~Movement();

    // ===== Lifetime =====


    // ===== Methods =====


    // Process the current movement
    void update();


    /**
     * 
     * Methods would request update() to check the current 
     * movement state data and implement
     * movement, depended on the movement state
     * and movement settings
     * 
     */

    void move_x_request(int direction, movement_regime regime);
    void move_y_request(int direction, movement_regime regime);


    float get_velocity_x() const;
    float get_velocity_y() const;


    movement_stage get_x_stage() const;   
    movement_stage get_y_stage() const;   

    movement_request get_x_request() const;   
    movement_request get_y_request() const;   
    

    void switch_settings(movement_settings& new_settings);

    // ===== Methods =====


private:

    // ===== Methods =====

    void update_delta_time();

    void update_stage();


    void update_velocity();

    
    void x_move_accumulate_ds(float new_ds);
    void x_move_drop_ds();

    void y_move_accumulate_ds(float new_ds);
    void y_move_drop_ds();


    // ===== Methods =====


    // ===== Data =====


    // Callback to the x and y setter of other classes
    position_setter position_setter_function;


    movement_request x_request;
    movement_request y_request;


    movement_settings settings;

    movement_state movement;


    App_timer& used_timer;

    float delta_time;           // Seconds

    // ===== Data =====

};

// =========================================================================================== MOVEMENT CLASS