// global_inputs.h

#pragma once


// =========================================================================================== IMPORT

#include <SDL3/SDL.h>



#include <array>
#include <vector>
#include <unordered_map>

// =========================================================================================== IMPORT


// =========================================================================================== GLOBAL INPUTS


// =========================================================================================== MOUSE

// SINGLETON class for mouse input state tracking - API wrapper for SDL mouse state functions and event 
// handling for mouse data and logic centralization
class GI_mouse
{
    public:

        // ===== SINGLETON CREATE =====

        /**
         * @brief Returns the singleton instance.
         *
         * Guarantees a single global instance of GI_mouse.
         */
        static GI_mouse& Instance();
        
        // ===== SINGLETON CREATE =====


        // ===== Basic getters =====
        
        // Values

        float get_x() const;
        float get_y() const;

        // States

        bool lb_clicked() const;
        bool rb_clicked() const;

        // Frames logic

        bool lb_is_pressed() const { return lb_state && !lb_prev; };
        bool lb_is_held() const { return lb_state && lb_prev; };
        bool lb_is_just_released() const { return !lb_state && lb_prev; };
        bool lb_is_released() const { return !lb_state && !lb_prev; };

        bool rb_is_pressed() const { return rb_state && !rb_prev; };
        bool rb_is_held() const { return rb_state && rb_prev; };
        bool rb_is_just_released() const { return !rb_state && rb_prev; };
        bool rb_is_released() const { return !rb_state && !rb_prev; };


        // ===== Basic getters =====


        friend class GI_input_manager;

    protected: 

        // ===== Main logic =====

        // Main mouse data update method - should be called once per frame to refresh 
        // mouse state information
        void update();

        // ===== Main logic =====


    private:

        // ===== Singleton pattern implementation =====

        // Private constructor ensures no external instances can be created.
        GI_mouse() = default;

        // Default destructor is fine; no dynamic allocation to clean up.
        ~GI_mouse() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        GI_mouse(const GI_mouse&) = delete;


        // Assignment operator is deleted to prevent copying the singleton.
        GI_mouse& operator=(const GI_mouse&) = delete;

        // ===== Singleton pattern implementation =====


        // ===== Cached state =====

        float x_pos = 0.0f;
        float y_pos = 0.0f;

        // Current frame state 

        bool lb_state = false;
        bool rb_state = false;

        // Previous frame state

        bool lb_prev = false;
        bool rb_prev = false;

        // ===== Cached state =====

};


// =========================================================================================== MOUSE


// =========================================================================================== KEYBOARD


enum class Key_actions
{

    ENTER,
    EXIT,
    MENU_FORWARD,
    MENU_BACK,
    SPECIAL_1,
    LEFT,
    UP,
    RIGHT,
    DOWN,

    COUNT

};

static constexpr size_t KEY_ACTION_COUNT = static_cast<size_t>(Key_actions::COUNT);


struct Key_action_hash
{
    size_t operator()(Key_actions a) const
    {
        return static_cast<size_t>(a);
    }
};


// SINGLETON class for mouse input state tracking - API wrapper for SDL keyboard state functions and event 
// handling for keyboard data and logic centralization
class GI_keyboard
{

    public:

        // ===== SINGLETON CREATE =====

        /**
         * @brief Returns the singleton instance.
         *
         * Guarantees a single global instance of GI_mouse.
         */
        static GI_keyboard& Instance();

        // ===== SINGLETON CREATE =====


        // ===== Keyboard getters ===== 

        bool is_pressed(SDL_Scancode key) const;

        bool is_held(SDL_Scancode key) const;

        bool is_just_released(SDL_Scancode key) const;

        bool is_released(SDL_Scancode key) const;


        // ===== Keyboard getters ===== 

        friend class GI_input_manager;

    protected:

        // ===== Main logic =====

        // Main mouse data update method - should be called once per frame to refresh 
        // mouse state information
        void update();

        // ===== Main logic =====

    private:

        // ===== Singleton pattern implementation =====

        // Private constructor ensures no external instances can be created.
        GI_keyboard() = default;

        // Default destructor is fine; no dynamic allocation to clean up.
        ~GI_keyboard() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        GI_keyboard(const GI_keyboard&) = delete;


        // Assignment operator is deleted to prevent copying the singleton.
        GI_keyboard& operator=(const GI_keyboard&) = delete;

        // ===== Singleton pattern implementation =====


        // ===== Data =====

        // SDL3 AND SDL2 CONFLICT

        bool key_state[SDL_SCANCODE_COUNT] = { false };
        bool key_prev[SDL_SCANCODE_COUNT]  = { false };

        // ===== Data =====

};


class GI_input_manager
{

    public:

        // ===== SINGLETON CREATE =====

        static GI_input_manager& Instance();

        // ===== SINGLETON CREATE =====


        // ===== Main logic =====

        // Mouse update and keyboard update (by SINGLETONs instances)
        void update();


        // Bind action to key
        void bind(Key_actions action, SDL_Scancode key);


        // ===== Main logic =====

        // ===== Keyboard getters with actions wrapp ===== 

        bool is_pressed(Key_actions action) const;
        bool is_held(Key_actions action) const;
        bool is_just_released(Key_actions action) const;
        bool is_released(Key_actions action) const;

        // ===== Keyboard getters with actions wrapp ===== 


    private:

        // ===== Singleton pattern implementation =====

        // Private constructor ensures no external instances can be created.
        GI_input_manager() = default;

        // Default destructor is fine; no dynamic allocation to clean up.
        ~GI_input_manager() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        GI_input_manager(const GI_input_manager&) = delete;


        // Assignment operator is deleted to prevent copying the singleton.
        GI_input_manager& operator=(const GI_input_manager&) = delete;

        // ===== Singleton pattern implementation =====

        std::unordered_map<Key_actions, std::vector<SDL_Scancode>, Key_action_hash> keymap;
};


// =========================================================================================== KEYBOARD


// =========================================================================================== GLOBAL INPUTS


// =========================================================================================== SINGLETONS FOR USE

// Global singleton instance of GI_mouse for easy access throughout the program
inline GI_mouse& App_mouse = GI_mouse::Instance();

// Returns the current state of the left mouse button click by checking the GI_mouse singleton
bool lb_click_check();


// Global singleton instance of GI_keyboard with actions easy access throughout the program (easy way for control settings)
inline GI_keyboard& App_keyboard = GI_keyboard::Instance();

inline GI_input_manager& App_inputs = GI_input_manager::Instance();


// =========================================================================================== SINGLETONS FOR USE
