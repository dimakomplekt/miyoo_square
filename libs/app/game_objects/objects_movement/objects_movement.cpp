// objects_movement.cpp


// =========================================================================================== IMPORT

#include "objects_movement.h"

// =========================================================================================== IMPORT


// =========================================================================================== MOVEMENT CLASS


// ===== Lifetime =====

Movement::Movement(

    const movement_settings& settings,
    App_timer& timer,
    position_setter setter

)

: 

used_timer(timer)

{
    this->settings = settings;
    this->position_setter_function = setter;
}


Movement::~Movement() {}


// ===== Lifetime =====




// ===== Methods =====


void Movement::move_x_request(int direction, movement_regime regime)
{
    if (direction != 0 && abs(direction) != 1) return;
    if (regime != MOVEMENT_MR && regime != JUMP_MR) return;


    this->x_request.direction = direction;
    this->x_request.regime = regime;
}


void Movement::move_y_request(int direction, movement_regime regime)
{
    if (direction != 0 && abs(direction) != 1) return;
    if (regime != MOVEMENT_MR && regime != JUMP_MR) return;


    this->y_request.direction = direction;
    this->y_request.regime = regime;
}


float Movement::get_velocity_x() const
{
    return this->movement.x.velocity;
}


float Movement::get_velocity_y() const
{
    return this->movement.y.velocity;
}


movement_stage Movement::get_x_stage() const
{
    return this->movement.x.stage;
}


movement_stage Movement::get_y_stage() const
{
    return this->movement.y.stage;
}


movement_request Movement::get_x_request() const
{
    return this->x_request;
}  


movement_request Movement::get_y_request() const
{
    return this->y_request;
}  


    

void Movement::switch_settings(movement_settings& new_settings)
{
    this->settings = new_settings;
}


// Process the current movement
void Movement::update()
{
    this->update_delta_time();

    this->update_stage();

    this->update_velocity();
}


void Movement::update_delta_time()
{
    // Seconds
    this->delta_time = this->used_timer.get_delta_time();
}


void Movement::update_stage()
{
    // X-axis update

    if (this->x_request.regime == MOVEMENT_MR)
    {
        if (this->x_request.direction == 0)
        {
            if (this->movement.x.velocity != 0)
            {
                this->movement.x.stage = MOVEMENT_MOVE_RELEASE;
            }
            else
            {
                this->movement.x.stage = MOVEMENT_IDLE;
            }
        }   

        else if (this->x_request.direction != 0)
        {
            if (this->movement.x.stage == MOVEMENT_IDLE)
            {
                this->movement.x.stage = MOVEMENT_MOVE_ATTACK;
            }
            else
            {
                if (this->movement.x.velocity >= this->settings.max_speed)
                {
                    this->movement.x.stage = MOVEMENT_MOVE_SUSTAIN;
                }
                else
                {
                    this->movement.x.stage = MOVEMENT_MOVE_ATTACK;
                }
            }
        }
    }
    else
    {
        if (this->x_request.direction != 0)
        {
            if (this->movement.x.stage == MOVEMENT_JUMP)
            {
                if (this->movement.x.velocity >= this->settings.jump_max_speed)
                {
                    this->movement.x.stage = MOVEMENT_FALL;
                }
                else
                {
                    this->movement.x.stage = MOVEMENT_JUMP;
                }
            }
            else
            {
                if (this->movement.x.velocity == 0)
                    this->movement.x.stage = MOVEMENT_JUMP;
            }
        }
        else
        {
            if (this->movement.x.stage == MOVEMENT_JUMP)
            {
                if (this->movement.x.velocity >= this->settings.jump_max_speed)
                {
                    this->movement.x.stage = MOVEMENT_FALL;
                }
                else
                {
                    this->movement.x.stage = MOVEMENT_JUMP;
                }
            }
            else
            {
                if (this->movement.x.velocity == 0)
                    this->movement.x.stage = MOVEMENT_IDLE;
            }
        }
    }


    // Y-axis update

    if (this->y_request.regime == MOVEMENT_MR)
    {
        if (this->y_request.direction == 0)
        {
            if (this->movement.y.velocity != 0)
            {
                this->movement.y.stage = MOVEMENT_MOVE_RELEASE;
            }
            else
            {
                this->movement.y.stage = MOVEMENT_IDLE;
            }
        }   

        else if (this->y_request.direction != 0)
        {
            if (this->movement.y.stage == MOVEMENT_IDLE)
            {
                this->movement.y.stage = MOVEMENT_MOVE_ATTACK;
            }
            else
            {
                if (this->movement.y.velocity >= this->settings.max_speed)
                {
                    this->movement.y.stage = MOVEMENT_MOVE_SUSTAIN;
                }
                else
                {
                    this->movement.y.stage = MOVEMENT_MOVE_ATTACK;
                }
            }
        }
    }
    else
    {
        if (this->y_request.direction != 0)
        {
            if (this->movement.y.stage == MOVEMENT_JUMP)
            {
                if (this->movement.y.velocity >= this->settings.jump_max_speed)
                {
                    this->movement.y.stage = MOVEMENT_FALL;
                }
                else
                {
                    this->movement.y.stage = MOVEMENT_JUMP;
                }
            }
            else
            {
                if (this->movement.y.velocity == 0)
                    this->movement.y.stage = MOVEMENT_JUMP;
            }
        }
        else
        {
            if (this->movement.y.stage == MOVEMENT_JUMP)
            {
                if (this->movement.y.velocity >= this->settings.jump_max_speed)
                {
                    this->movement.y.stage = MOVEMENT_FALL;
                }
                else
                {
                    this->movement.y.stage = MOVEMENT_JUMP;
                }
            }
            else
            {
                if (this->movement.y.velocity == 0)
                    this->movement.y.stage = MOVEMENT_IDLE;
            }
        }
    }


    this->movement.x.direction = this->x_request.direction;
    this->movement.y.direction = this->y_request.direction;

    if (this->movement.y.stage == MOVEMENT_FALL)
    {
        // Falling is gravity-driven and must keep moving down after the
        // upward jump phase has ended, even while the jump key is held.
        this->movement.y.direction = 1;
    }
}


void Movement::update_velocity()
{
    auto update_axis =
        [this](movement_axis_state& axis,
               float max_speed,
               float acceleration,
               float deceleration,
               float& sub_pixel_acc)
        {
            if (axis.stage == MOVEMENT_IDLE)
            {
                axis.velocity = 0.0f;
                sub_pixel_acc = 0.0f;
                return 0;
            }

            if (axis.stage == MOVEMENT_MOVE_ATTACK)
            {
                axis.velocity = std::min(
                    max_speed, axis.velocity + acceleration * this->delta_time);
            }
            else if (axis.stage == MOVEMENT_MOVE_SUSTAIN)
            {
                axis.velocity = max_speed;
            }
            else if (axis.stage == MOVEMENT_MOVE_RELEASE)
            {
                axis.velocity = std::max(
                    0.0f, axis.velocity - deceleration * this->delta_time);
            }
            else if (axis.stage == MOVEMENT_FALL)
            {
                axis.velocity = max_speed;
            }
            else if (axis.stage == MOVEMENT_JUMP)
            {
                axis.velocity = std::min(
                    max_speed, axis.velocity + acceleration * this->delta_time);
            }

            const float desired_delta =
                axis.direction * axis.velocity * this->delta_time +
                sub_pixel_acc;
            const int integer_delta = static_cast<int>(desired_delta);
            sub_pixel_acc = desired_delta - integer_delta;
            return integer_delta;
        };

    const int new_delta_x = update_axis(
        this->movement.x,
        this->movement.x.stage == MOVEMENT_JUMP ||
            this->movement.x.stage == MOVEMENT_FALL
            ? this->settings.jump_max_speed
            : this->settings.max_speed,
        this->movement.x.stage == MOVEMENT_JUMP
            ? this->settings.jump_acceleration
            : this->settings.acceleration,
        this->movement.x.stage == MOVEMENT_FALL
            ? this->settings.jump_deceleration
            : this->settings.deceleration,
        this->movement.x.sub_pixel_ds_acc);

    const int new_delta_y = update_axis(
        this->movement.y,
        this->movement.y.stage == MOVEMENT_JUMP ||
            this->movement.y.stage == MOVEMENT_FALL
            ? this->settings.jump_max_speed
            : this->settings.max_speed,
        this->movement.y.stage == MOVEMENT_JUMP
            ? this->settings.jump_acceleration
            : this->settings.acceleration,
        this->movement.y.stage == MOVEMENT_FALL
            ? this->settings.jump_deceleration
            : this->settings.deceleration,
        this->movement.y.sub_pixel_ds_acc);

    this->position_setter_function(new_delta_x, new_delta_y);
}


void Movement::x_move_accumulate_ds(float new_ds)
{
    this->movement.x.sub_pixel_ds_acc += new_ds;
}


void Movement::x_move_drop_ds()
{
    this->movement.x.sub_pixel_ds_acc = 0.0f;
}


void Movement::y_move_accumulate_ds(float new_ds)
{
    this->movement.y.sub_pixel_ds_acc += new_ds;
}


void Movement::y_move_drop_ds()
{
    this->movement.y.sub_pixel_ds_acc = 0.0f;
}


// ===== Methods =====


// =========================================================================================== MOVEMENT CLASS