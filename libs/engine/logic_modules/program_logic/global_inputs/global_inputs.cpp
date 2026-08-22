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
    // ===== SDL3 AND SDL2 CONFLICT =====

    /*

    const bool* curr_sdl_k = SDL_GetKeyboardState(nullptr);

    // SDL3 AND SDL2 CONFLICT
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        key_prev[i]  = key_state[i];
        key_state[i] = curr_sdl_k[i];
    }

    */

    // SDL2 FIX: SDL_GetKeyboardState in SDL2 returns 'const Uint8*' instead of 'const bool*'
    const Uint8* curr_sdl_k = SDL_GetKeyboardState(nullptr);

    // SDL2 FIX: Loop using SDL_NUM_SCANCODES as the upper boundary
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        key_prev[i]  = key_state[i];
        
        // Implicitly converts Uint8 (0 or 1) from SDL2 array to bool (false or true)
        key_state[i] = curr_sdl_k[i];
    }

    
    // ===== SDL3 AND SDL2 CONFLICT =====
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

        // ===== PC =====

        #ifdef PLATFORM_WINDOWS
        
            // ============================================================ 
            // SYSTEM

            instance.bind(Key_actions::SELECT_KA, SDL_SCANCODE_F);
            instance.bind(Key_actions::SELECT_KA, SDL_SCANCODE_SPACE);

            instance.bind(Key_actions::START_KA, SDL_SCANCODE_RETURN);

            instance.bind(Key_actions::EXIT_KA, SDL_SCANCODE_ESCAPE);


            // ============================================================
            // D-PAD

            instance.bind(Key_actions::LEFT_KA, SDL_SCANCODE_LEFT);
            instance.bind(Key_actions::LEFT_KA, SDL_SCANCODE_A);

            instance.bind(Key_actions::UP_KA, SDL_SCANCODE_UP);
            instance.bind(Key_actions::UP_KA, SDL_SCANCODE_W);

            instance.bind(Key_actions::RIGHT_KA, SDL_SCANCODE_RIGHT);
            instance.bind(Key_actions::RIGHT_KA, SDL_SCANCODE_D);

            instance.bind(Key_actions::DOWN_KA, SDL_SCANCODE_DOWN);
            instance.bind(Key_actions::DOWN_KA, SDL_SCANCODE_S);


            // ============================================================
            // FACE BUTTONS

            // Miyoo Y = PC J
            instance.bind(Key_actions::Y_KA, SDL_SCANCODE_J);

            // Miyoo X = PC I
            instance.bind(Key_actions::X_KA, SDL_SCANCODE_I);

            // Miyoo A = PC L
            instance.bind(Key_actions::A_KA, SDL_SCANCODE_L);

            // Miyoo B = PC K
            instance.bind(Key_actions::B_KA, SDL_SCANCODE_K);


            // ============================================================
            // SHOULDERS

            // Miyoo L1 = PC Z
            instance.bind(Key_actions::L_1_KA, SDL_SCANCODE_Z);

            // Miyoo L2 = PC X
            instance.bind(Key_actions::L_2_KA, SDL_SCANCODE_X);

            // Miyoo R1 = PC C
            instance.bind(Key_actions::R_1_KA, SDL_SCANCODE_C);

            // Miyoo R2 = PC V
            instance.bind(Key_actions::R_2_KA, SDL_SCANCODE_V);


        // ===== PC =====

        
        // ===== MIYOO =====
        #elif defined(PLATFORM_MIYOO)
        

            // ============================================================
            // SYSTEM

            instance.bind(Key_actions::SELECT_KA, SDL_SCANCODE_RCTRL);
            instance.bind(Key_actions::START_KA, SDL_SCANCODE_RETURN);
            instance.bind(Key_actions::EXIT_KA, SDL_SCANCODE_HOME);


            // ============================================================
            // D-PAD

            instance.bind(Key_actions::LEFT_KA, SDL_SCANCODE_LEFT);
            instance.bind(Key_actions::UP_KA, SDL_SCANCODE_UP);
            instance.bind(Key_actions::RIGHT_KA, SDL_SCANCODE_RIGHT);
            instance.bind(Key_actions::DOWN_KA, SDL_SCANCODE_DOWN);


            // ============================================================
            // FACE BUTTONS

            instance.bind(Key_actions::Y_KA, SDL_SCANCODE_LALT);
            instance.bind(Key_actions::X_KA, SDL_SCANCODE_LSHIFT);
            instance.bind(Key_actions::A_KA, SDL_SCANCODE_SPACE);
            instance.bind(Key_actions::B_KA, SDL_SCANCODE_LCTRL);


            // ============================================================
            // SHOULDERS

            instance.bind(Key_actions::L_1_KA, SDL_SCANCODE_E);
            instance.bind(Key_actions::L_2_KA, SDL_SCANCODE_TAB);
            instance.bind(Key_actions::R_1_KA, SDL_SCANCODE_T);
            instance.bind(Key_actions::R_2_KA, SDL_SCANCODE_BACKSPACE);
        

        // ===== MIYOO =====

        #elif defined(PLATFORM_LINUX)

            // Linux bindings

        #endif


        initialized = true;
    }



    return instance;
}


void GI_input_manager::update()
{
    App_keyboard.update();

    // No need to
    #ifndef PLATFORM_MIYOO

        App_mouse.update();

    #endif
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
