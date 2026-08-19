// my_sdl_element.h


// =========================================================================================== IMPORT

#include "my_sdl_element.h"

#include "../my_sdl_panel/my_sdl_panel.h"       // Methods pass

// =========================================================================================== IMPORT


// =========================================================================================== HELPER-FUNCTIONS

bool hover_check_by_boundaries(const element_rect_boundaries& boundaries)
{
    int curr_x = static_cast<int>(std::round(App_mouse.get_x()));
    int curr_y = static_cast<int>(std::round(App_mouse.get_y()));


    if (curr_x >= boundaries.left_boundary - DELTA_FOR_HOVER_CLICK_CHECKS &&
        curr_x <= boundaries.right_boundary + DELTA_FOR_HOVER_CLICK_CHECKS &&
        curr_y <= boundaries.bottom_boundary + DELTA_FOR_HOVER_CLICK_CHECKS &&
        curr_y >= boundaries.top_boundary - DELTA_FOR_HOVER_CLICK_CHECKS) 
        
        // Mouse inside the element zone 
        return true;
    
    else 

        // Mouse outside the element zone
        return false;
}


// =========================================================================================== HELPER-FUNCTIONS



// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR


My_SDL_element::My_SDL_element()
{
    // Default settings

    this->gui_type = STATIC_ELEMENT_GUI;


    this->x_render_point = 0;
    this->y_render_point = 0;

    // Movement type
    this->element_movement.easing = LOGARITHMIC;

    this->opacity = 255;
    this->basic_opacity = 255;


    // Basic GUI elements colors setted by palette (default)
    this->passed_by_palette = true;

    // Basically visible
    this->visible_flag = true;
}


// =========================================================================================== CONSTRUCTOR AND DESTRUCTOR


// =========================================================================================== MAIN LOGIC

My_SDL_panel* My_SDL_element::get_element_container() const
{
    return this->element_container;
}

void My_SDL_element::set_visible_flag(bool new_flag)
{   
    this->visible_flag = new_flag;
}



// =========================================================================================== MAIN LOGIC


// =========================================================================================== GUI

void My_SDL_element::move_to_point(
            
    int new_x_cc_rp, 
    int new_y_cc_rp,

    movement_easing easing,

    Uint64 movement_time

)
{
    // Set the data inside the movement context
    this->element_movement.active = true;

    this->element_movement.start_pos.x = this->x_render_point;
    this->element_movement.start_pos.y = this->y_render_point;

    this->element_movement.end_pos.x = new_x_cc_rp;
    this->element_movement.end_pos.y = new_y_cc_rp;

    this->element_movement.start_time = SDL_GetTicks();
    this->element_movement.duration = movement_time;

    this->element_movement.progress = 0.0f;

    this->element_movement.easing = easing;

}


void My_SDL_element::movement_reset()
{
    this->element_movement.active = false;
    this->element_movement.progress = 0.0f;
}


void My_SDL_element::movement_logic_in_update_loop()
{
    // ============================================================
    // 1. EARLY EXIT (performance gate)
    // ============================================================
    // If no active movement is running, we skip all computations.
    // This keeps update cost minimal for static elements.
    if (!element_movement.active)
        return;


    // ============================================================
    // 2. TIME-BASED NORMALIZATION
    // ============================================================
    //
    // We compute normalized time parameter t in range [0, 1]:
    //
    //      t = (current_time - start_time) / duration
    //
    // This removes dependency on FPS and makes animation
    // deterministic across all hardware.
    //
    Uint64 now = SDL_GetTicks();

    float t =
        (float)(now - element_movement.start_time) /
        (float)element_movement.duration;


    // ============================================================
    // 3. CLAMPING (bounded interpolation domain)
    // ============================================================
    //
    // We restrict t to [0, 1] to prevent overshooting
    // beyond the intended animation interval.
    //
    if (t >= 1.0f)
    {
        t = 1.0f;
        element_movement.active = false;
    }


    // ============================================================
    // 4. EASING FUNCTION (time remapping)
    // ============================================================
    //
    // We transform linear time into nonlinear motion curves:
    //
    //      t' = f(t)
    //
    // This affects velocity perception, not geometry directly.
    //
    switch (element_movement.easing)
    {
        // --------------------------------------------------------
        // LINEAR MOTION
        // --------------------------------------------------------
        // f(t) = t
        // Constant velocity motion.
        case LINEAR:
            // no modification needed
            break;


        // --------------------------------------------------------
        // EXPONENTIAL ACCELERATION
        // --------------------------------------------------------
        // f(t) = t²
        // Slow start, fast finish (accelerating motion).
        case EXPONENTIAL:
            t = t * t;
            break;


        // --------------------------------------------------------
        // LOGARITHMIC DECELERATION
        // --------------------------------------------------------
        // f(t) = log10(1 + 9t)
        // Fast start, slow end (decelerating motion).
        case LOGARITHMIC:
            t = std::log10(1.0f + 9.0f * t);
            break;


        // --------------------------------------------------------
        // BACKWARD LOGARITHMIC
        // --------------------------------------------------------
        // f(t) = 1 - log10(1 + 9(1 - t))
        // Smooth start, easing into stop.
        case BACKWARD_LOGARITHMIC:
            t = 1.0f - std::log10(1.0f + 9.0f * (1.0f - t));
            break;
    }


    // ============================================================
    // 5. LINEAR INTERPOLATION (LERP)
    // ============================================================
    //
    // Motion is defined in GLOBAL space to avoid coordinate
    // ambiguity between standalone elements and container-bound
    // elements.
    //
    // The interpolation is performed only once in absolute space:
    //
    //      P(t) = P0 + (P1 - P0) * t
    //
    // This guarantees that movement is independent of:
    //  - parent container transforms
    //  - current render state
    //  - previous frame accumulated errors
    //
    // Only the movement definition (start/end) defines the result,
    // which makes motion deterministic and frame-rate independent.
    //
    // ============================================================
    // WHY GLOBAL SPACE FIRST:
    // ============================================================
    //
    // Every element has two possible coordinate contexts:
    //
    //  1. Root space (no container)
    //  2. Local-to-parent space (inside panel)
    //
    // If interpolation were done directly in local space,
    // the mapping would depend on intermediate transformed values,
    // causing drift and feedback artifacts.
    //
    // By computing GLOBAL position first, we establish a single
    // source of truth for motion.
    //
    // ============================================================
    // PROJECTION STEP (CONTAINER CASE):
    // ============================================================
    //
    // When element belongs to a container, global position is
    // projected into local space using inverse transform:
    //
    //      local = panel^{-1}(global)
    //
    // This ensures that container layout rules remain valid while
    // preserving global motion semantics.
    //
    // Importantly, the container is NOT involved in the motion
    // calculation itself — it only maps coordinates.
    //
    // ============================================================
    // STATE UPDATE STRATEGY:
    // ============================================================
    //
    // Even in container mode, global render position is still stored
    // to preserve consistency of movement system.
    //
    // Container receives only the derived local coordinates via:
    // change_element_local_coordinate()
    //
    // This keeps:
    //  - movement system (global) decoupled
    //  - layout system (local) consistent
    //  - rendering system deterministic
    //
    // ============================================================
    // RESULT:
    // ============================================================
    //
    // - No accumulation errors
    // - No coordinate feedback loops
    // - No dependency on previous frame state
    // - Stable cross-container animation behavior
    //
    // ============================================================

    float global_x =
        element_movement.start_pos.x +
        (element_movement.end_pos.x - element_movement.start_pos.x) * t;

    float global_y =
        element_movement.start_pos.y +
        (element_movement.end_pos.y - element_movement.start_pos.y) * t;

    if (this->element_container == nullptr)
    {
        // Root-space element: direct assignment in global space
        // Set only by set method (cause it will call anchors reset and other things)

        this->set_render_point((int)global_x, (int)global_y);
    }
    else
    {
        // Container-bound element:
        // 1. Project global position into local space
        My_SDL_panel* panel = this->element_container;

        int local_x = panel->local_x_by_global_x((int)global_x);
        int local_y = panel->local_y_by_global_y((int)global_y);

        // 2. Store global render state (for consistency/debug/render reuse)
        this->x_render_point = (int)global_x;
        this->y_render_point = (int)global_y;

        // 3. Update container-local representation
        panel->change_element_local_coordinate(this, local_x, local_y, 1);
    }

    if (this->element_movement.progress == 1.0) this->movement_reset();
}


// Render point getters 

int My_SDL_element::get_x_render_point() const { return this->x_render_point; }

int My_SDL_element::get_y_render_point() const { return this->y_render_point; }


// Anchor points getter

anchor_points My_SDL_element::get_anchor_points() const
{
    return this->element_anchor_points;
}


// Opacity setter and getter

void My_SDL_element::set_opacity(Uint8 new_opacity) 
{
    this->basic_opacity = new_opacity;
    
    // In case where we got the container
    if (this->element_container != nullptr)
    {
        this->recalculate_opacity_by_container();
    }
    else
    {
        // Equal opacity in case without container
        this->opacity = new_opacity;
    }
}


void My_SDL_element::recalculate_opacity_by_container()
{
    My_SDL_panel* container = this->get_element_container();

    if (!container)
    {
        this->opacity = this->basic_opacity;
        return;
    }

    float result =
        static_cast<float>(this->basic_opacity) *
        static_cast<float>(container->get_opacity()) / 255.0f;

    if (result < 0.0f) result = 0.0f;
    if (result > 255.0f) result = 255.0f;

    this->opacity = static_cast<Uint8>(std::lround(result));
}
 


Uint8 My_SDL_element::get_opacity() const 
{
    return this->opacity;
}


Uint8 My_SDL_element::get_basic_opacity() const 
{
    return this->basic_opacity;
}


// GUI type setter 

void My_SDL_element::set_gui_type(element_gui_type new_gui_type)
{
    // Error handling for invalid GUI type
    if (new_gui_type != STATIC_ELEMENT_GUI && new_gui_type != DYNAMIC_ELEMENT_GUI)
    {
        std::cerr << "Invalid element GUI type. GUI type not changed." << std::endl;
        return;
    }

    // New type setting
    else this->gui_type = new_gui_type;
}


void My_SDL_element::switch_passed_by_palette_flag(bool new_status)
{
    this->passed_by_palette = new_status;
}

// =========================================================================================== GUI
