// global_inputs.cpp


// =========================================================================================== IMPORT

#include "global_inputs.h"

// =========================================================================================== IMPORT


// =========================================================================================== MOUSE


GI_mouse& GI_mouse::Instance()
{
    // Local static ensures thread-safe lazy initialization in C++11+
    // Guarantees that only one instance of GI_mouse exists throughout the program.
    static GI_mouse instance;

    return instance;
}


void GI_mouse::update()
{
    // Frame reset
    this->lb_prev = this->lb_state;
    this->rb_prev = this->rb_state;


    // We call SDL_GetMouseState to update the mouse position and get bitmask of button states
    Uint32 buttons = SDL_GetMouseState(&this->x_pos, &this->y_pos);


    // We check the button bitmask to update our buttons pressing states
    this->lb_state = (buttons & SDL_BUTTON_LEFT) != 0;
    this->rb_state = (buttons & SDL_BUTTON_RIGHT) != 0;
}

// Simple returns 

float GI_mouse::get_x() const { return this->x_pos; }
float GI_mouse::get_y() const { return this->y_pos; }

bool GI_mouse::lb_clicked() const { return this->lb_state; }
bool GI_mouse::rb_clicked() const { return this->rb_state; }


bool lb_click_check()
{
    // Click status by GI_mouse singleton
    return App_mouse.lb_clicked();
}


// =========================================================================================== MOUSE


// =========================================================================================== KEYBOARD

GI_keyboard& GI_keyboard::Instance()
{
    static GI_keyboard instance;
    return instance;
}


void GI_keyboard::update()
{
    const bool* curr_sdl_k = SDL_GetKeyboardState(nullptr);

    // SDL3 AND SDL2 CONFLICT
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        key_prev[i]  = key_state[i];
        key_state[i] = curr_sdl_k[i];
    }
}


bool GI_keyboard::is_pressed(SDL_Scancode key) const
{
    return key_state[key] && !key_prev[key];
}

bool GI_keyboard::is_held(SDL_Scancode key) const
{
    return key_state[key] && key_prev[key];
}

bool GI_keyboard::is_just_released(SDL_Scancode key) const
{
    return !key_state[key] && key_prev[key];
}

bool GI_keyboard::is_released(SDL_Scancode key) const
{
    return !key_state[key] && !key_prev[key];
}



GI_input_manager& GI_input_manager::Instance()
{
    static GI_input_manager instance;

    static bool initialized = false;

    if (!initialized)
    {
        // Actions initialization zone

        instance.bind(Key_actions::ENTER, SDL_SCANCODE_RETURN);

        instance.bind(Key_actions::EXIT, SDL_SCANCODE_ESCAPE);

        instance.bind(Key_actions::MENU_FORWARD, SDL_SCANCODE_RIGHT);

        instance.bind(Key_actions::MENU_BACK, SDL_SCANCODE_LEFT);


        instance.bind(Key_actions::SPECIAL_1, SDL_SCANCODE_SPACE);

        instance.bind(Key_actions::LEFT, SDL_SCANCODE_LEFT);
        instance.bind(Key_actions::UP, SDL_SCANCODE_UP);
        instance.bind(Key_actions::RIGHT, SDL_SCANCODE_RIGHT);
        instance.bind(Key_actions::DOWN, SDL_SCANCODE_DOWN);

        initialized = true;
    }



    return instance;
}


void GI_input_manager::update()
{
    App_keyboard.update();
    App_mouse.update();
}


void GI_input_manager::bind(Key_actions action, SDL_Scancode key)
{
    auto& vec = keymap[action];

    for (auto k : vec)
        if (k == key) return;

    vec.push_back(key);
}


bool GI_input_manager::is_pressed(Key_actions action) const
{
    auto it = keymap.find(action);
    if (it == keymap.end()) return false;

    const auto& kb = App_keyboard;

    for (auto key : it->second)
        if (kb.is_pressed(key))
            return true;

    return false;
}

bool GI_input_manager::is_held(Key_actions action) const
{
    auto it = keymap.find(action);
    if (it == keymap.end()) return false;

    const auto& kb = App_keyboard;

    for (auto key : it->second)
        if (kb.is_held(key))
            return true;

    return false;
}

bool GI_input_manager::is_just_released(Key_actions action) const
{
    auto it = keymap.find(action);
    if (it == keymap.end()) return false;

    const auto& kb = App_keyboard;

    for (auto key : it->second)
        if (kb.is_just_released(key))
            return true;

    return false;
}

bool GI_input_manager::is_released(Key_actions action) const
{
    auto it = keymap.find(action);
    if (it == keymap.end()) return false;

    const auto& kb = App_keyboard;

    for (auto key : it->second)
        if (kb.is_released(key))
            return true;

    return false;
}

// =========================================================================================== KEYBOARD
