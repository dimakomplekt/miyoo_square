// app_timer.cpp

// =========================================================================================== IMPORT

#include "app_timer.h"

// =========================================================================================== IMPORT


// =========================================================================================== APP TIMER SINGLETON

App_timer& App_timer::Instance()
{
    // Local static ensures thread-safe lazy initialization in C++11+
    // Guarantees that only one instance of App_timer exists throughout the program.
    static App_timer instance;

    return instance;
}

App_timer::App_timer()
{
    this->current_time = 0.0f; // Initialize the current time to zero

    // Constructor is private and only called once by Instance().
    // No dynamic memory allocation needed here.

    // Execute zones initialization
    execute_zones[(size_t)Execute_zone_ID::HZ_10000].frequency = 10000.0f;
    execute_zones[(size_t)Execute_zone_ID::HZ_1000 ].frequency = 1000.0f;
    execute_zones[(size_t)Execute_zone_ID::HZ_240  ].frequency = 240.0f;
    execute_zones[(size_t)Execute_zone_ID::HZ_120  ].frequency = 120.0f;
    execute_zones[(size_t)Execute_zone_ID::HZ_60   ].frequency = 120.0f;
    execute_zones[(size_t)Execute_zone_ID::HZ_30   ].frequency = 120.0f;
    execute_zones[(size_t)Execute_zone_ID::HZ_15   ].frequency = 120.0f;

}


void App_timer::update()
{
    // Update the time

    int current_ms = SDL_GetTicks();

    float current_time_sec =  static_cast<float>(current_ms / 1000.0f); // Convert milliseconds to seconds

    this->current_time = current_time_sec;


    // Update execute zones

    for (auto& zone : execute_zones)
    {
        float elapsed = current_time - zone.start_time;

        if (elapsed >= (1.0f / zone.frequency))
        {
            // Reset permission
            zone.execute_permission = true;

            // Update the start time
            zone.start_time = current_time;
        }
        else
        {
            zone.execute_permission = false;
        }
    }

}


void App_timer::end_cycle()
{
    // We pass the zones with permission and complete the logic for the current cycle, 
    // so we reset the execute_permission for all zones at the end of the cycle.
    for (auto& zone : execute_zones)
    {
        zone.execute_permission = false;
    }
}


float App_timer::get_current_time() const
{
    return this->current_time;
}


bool App_timer::can_execute(Execute_zone_ID id) const
{
    return execute_zones[(size_t)id].execute_permission;
}

// =========================================================================================== APP TIMER SINGLETON
