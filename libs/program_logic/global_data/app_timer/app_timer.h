// app_timer.h

// =========================================================================================== IMPORT

#include "../../../engine/engine.h"

#include <array>

// =========================================================================================== IMPORT


// =========================================================================================== EXECUTE ZONE STRUCT

struct execute_zone
{

    float frequency = 0.0f; // Frequency in Hz
    
    float start_time = 0.0f;

    bool execute_permission = false;

};


enum class Execute_zone_ID
{

    HZ_10000,
    HZ_1000,
    HZ_240,
    HZ_120,
    HZ_60,
    HZ_30,
    HZ_15,

    COUNT

};


// =========================================================================================== EXECUTE ZONE STRUCT


// =========================================================================================== APP TIMER SINGLETON

/**
 * 
 * App timer get's the time from SDL_GetTicks() at the start of the cycle and stores
 * this value in current_time.
 * 
 * Every time the update() function is called, it checks if the current_time - start_time for each
 * execute_zone is greater than or equal to 1/frequency. If it is, it sets execute_permission to true 
 * and updates start_time to the current_time.
 * 
 * At the end of the cycle, execute_permission is reset to false for all execute_zones.
 * 
 */
class App_timer
{
    public:

        // ===== Instance =====

        /**
         * @brief Returns the singleton instance.
         *
         * Guarantees a single global instance of App_timer.
         * 
         */
        static App_timer& Instance();

        // ===== Instance =====


        // ===== Update and getter =====

        /**
         * @brief Updates the timer and checks if the execute zones are reached.
         *
         * Should be called in the main loop to update the timer state.
         * 
         */
        void update();


        /**
         * @brief Resets the execute_permission for all execute zones at the end of the cycle.
         *
         * Should be called at the end of each main loop cycle to reset permissions.
         * 
         */
        void end_cycle();

        
        /**
         * @brief Returns the current time in seconds since the application started.
         *
         * @return Current time in seconds.
         * 
         */
        float get_current_time() const;

        // ===== Update and getter =====


        // ===== Execute zone management =====

        // In this versuion execute zones can't be removed, during
        // the program execution (hard to implement and not needed for this project)


        bool can_execute(Execute_zone_ID id) const;

        // ===== Execute zone management =====


    private:

        // ===== Constructor and Destuctor =====

        // Private constructor ensures no external instances can be created.
        App_timer();

        // Default destructor is fine; no dynamic allocation to clean up.
        ~App_timer() = default;

        // Copy constructor is deleted to prevent copying the singleton.
        App_timer(const App_timer&) = delete;


        // Assignment operator is deleted to prevent copying the singleton.
        App_timer& operator=(const App_timer&) = delete;

        // ===== Constructor and Destuctor =====


        // ===== Data =====

        float current_time = 0.0f; // Current time in seconds since the application started

        std::array<execute_zone, static_cast<size_t>(Execute_zone_ID::COUNT)> execute_zones;

        // ===== Data =====
};

// =========================================================================================== APP TIMER SINGLETON



// =========================================================================================== App_timer SINGLETON FOR USE

// Global singleton instance of GI_mouse for easy access throughout the program
inline App_timer& App_timer_1 = App_timer::Instance();

// =========================================================================================== App_timer SINGLETON FOR USE
