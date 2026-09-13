// instance.h

#pragma once


// =========================================================================================== IMPORT

#include "asset_manager.h"

// SDL
#include "../../engine.h"


// =========================================================================================== IMPORT



// =========================================================================================== INSTANCE BASIC CLASS



// Predeclare for friendship

class Instance_manager;
class Image_instance;

/**
 * 
 * @brief Instance basic class
 * 
 * Contains a lifetime routine and asset handle.
 * 
 * Declares the friendship with Instance manager class
 * 
 */
class Instance
{
    // ===== Friendship ===== 

    friend Instance_manager;

    // ===== Friendship ===== 


    protected:

        // ===== LIFETIME =====

        /**
         * @brief Construct a generic asset instance.
         * 
         * @param asset_handle Handle of the basic image asset.
         * @param r_instance_manager Responsible instance manager
         * @param r_asset_manager Responsible asset manager
         * 
         */
        Instance(handle_ctx asset_handle, Instance_manager* r_instance_manager, Asset_manager* r_asset_manager);


        // Virtual destructor.
        // Ensures proper basic class data cleanup with subclasses ~ calls.
        virtual ~Instance();

        // ===== LIFETIME =====


        // ===== DATA =====

        // Main asset handle, passed during the construction of the instance
        const handle_ctx asset_handle;

        const Instance_manager* instance_manager;
        const Asset_manager* asset_manager;

        // ===== DATA =====

};

// =========================================================================================== INSTANCE BASIC CLASS


/*

            ┌──────────────────────────────┐
            │          Asset               │
            │                              │
            │  original image / texture    │
            └──────────────┬───────────────┘
                        │
                        │ handle
                        ▼
            ┌──────────────────────────────┐
            │        Image_instance        │
            │                              │
            │  asset_handle                │
            │  scaler                      │
            │  crop_map                    │
            │  crop_width / height         │
            │  current_width / height      │
            │  anchors                     │
            │  instance texture            │
            └──────────────┬───────────────┘
                        │
                        │ used by renderer
                        ▼
            ┌──────────────────────────────┐
            │          Renderer            │
            │                              │
            │  position                    │
            │  rotation                    │
            │  flip                        │
            │  destination                 │
            └──────────────────────────────┘

*/


// =========================================================================================== HELPERS



namespace asset_instance_geometry
{

    // Descartes coordinate for 2D space.
    struct desc_c_2D
    {

        unsigned int x;    // Coordinate by x-axes (width).
        unsigned int y;    // Coordinate by y-axes (height).

    };


    // Signed descartes coordinate for 2D space.
    struct s_desc_c_2D
    {

        int x;    // Coordinate by x-axes (width).
        int y;    // Coordinate by y-axes (height).

    };


    // Crop map for 2D space
    struct crop_map_2D 
    {

        // Points choosen for width - height / x - y values coincidence

        desc_c_2D point_1;
        desc_c_2D point_2;
        

        desc_c_2D crop_center;

    };


    /**
     * @brief Nine key anchor points of the image in local (not rotated) space.
     *
     * These points allow flexible alignment:
     *
     * 
     *  [TL]---[TC]---[TR]
     * 
     * 
     *  [CL]---[CC]---[CR]
     * 
     * 
     *  [BL]---[BC]---[BR]
     *
     * 
     * This is useful for positioning sprites relative to
     * characters, physics bodies, or UI layout.
     * 
     * 
     * [CC] is always = 0,0 in this engine logic
     * 
     */
    struct anchor_points {

        s_desc_c_2D top_left;
        s_desc_c_2D top_center;
        s_desc_c_2D top_right;
        s_desc_c_2D center_left;
        s_desc_c_2D center_center;
        s_desc_c_2D center_right;
        s_desc_c_2D bottom_left;
        s_desc_c_2D bottom_center;
        s_desc_c_2D bottom_right;

    };

}


// ===== HITBOX and COLLISION =====

struct hitbox_points
{

    asset_instance_geometry::s_desc_c_2D top_left;
    asset_instance_geometry::s_desc_c_2D bottom_right;

};

struct collision_result
{

    bool has_collision = false;

    // Signed minimum translation direction: positive means the penetrating
    // object is on the negative side of the penetrated object.
    int penetration_x = 0;
    int penetration_y = 0;
    
};


collision_result check_collision(

    const hitbox_points& penetrating,
    const hitbox_points& penetrated

);


// ===== HITBOX and COLLISION ======



// ===== CUSTOM SURFACE GENERATION =====

enum custom_surface_gen_mode
{
    
    PATTERN_CSGM,
    RESCALE_CSGM

};


/**
 * @brief Generates a custom surface from a basic surface.
 *
 * @param basic_surface Source surface. Not modified by this function. Can't be announced as const(((
 * @param target_surface Destination surface. Modified by this function.
 * @param mode Surface generation mode.
 * 
 */
void custom_surface_generation(SDL_Surface* basic_surface, SDL_Surface* target_surface, custom_surface_gen_mode mode);

// ===== CUSTOM SURFACE GENERATION =====

// =========================================================================================== HELPERS



// =========================================================================================== IMAGE INSTANCE 


// ===== Class =====

class Image_instance : public Instance
{
    // ===== Friendship =====

    friend Instance_manager;

    // ===== Friendship =====


    public:

        // ========== METHODS ==========

        // === SIZE METHODS ===

        /*
                Image_asset
                    original_width
                    original_height
                        │
                        ▼
                Image_instance
                    crop
                    scaler
                    current size
                
        */


        // === CROP METHODS ===

        /**
         * @brief Image crop_map setter 3 
         * 
         * !!! Always setup by the initial asset sizes (scaled size drifting prevention) !!!
         * 
         * Setup the image crop_map by 2 points.
         * Automatically updates the current width and height,
         * then recalculates the anchor points.
         * 
         * Any points order!
         * 
         * @param x_1 1st crop point x
         * @param y_1 1st crop point y
         * @param x_2 2nd crop point x
         * @param y_2 2nd crop point y
         * 
         * Use like:
         * 
         * set_new_crop_map(x_1, y_1, x_2, y_2);
         * 
         */
        void set_new_crop_map(unsigned int x_1, unsigned int y_1, unsigned int x_2, unsigned int y_2);


        // === CROP METHODS ===


        /**
         * @brief Change image scale.
         *
         * Recomputes current width and height based on the original size.
         *
         * @param new_x_scaler Scale factor x-axes (1.0 = original size).
         * @param new_y_scaler Scale factor y-axes (1.0 = original size).
         * 
         */
        void set_scaler(float new_x_scaler, float new_y_scaler);


        // Current x-axes scaler getter
        float get_x_scaler() const;

        // Current y-axes scaler getter
        float get_y_scaler() const;


        /**
         * @brief Change image instance basic (not cropped) width size.
         *
         * Recalculates the instance parameters by the new width data.
         * Linked with scaler
         * 
         * @param new_width New width value
         * 
         */
        void set_width(unsigned int new_width);

        
        /**
         * @brief Change image width basic (not cropped) size.
         *
         * Recalculates the instance parameters by the new height data.
         * Linked with scaler
         *
         * @param new_height New height value
         * 
         */
        void set_height(unsigned int new_height);


        
        /**
         * @brief Get image instance width size (!!! cropped !!!).
         *
         * @return Current width (with scaling and crop)
         * 
         */
        unsigned int get_width() const;


        /**
         * @brief Get image instance width size (!!! cropped !!!).
         *
         * @return Current height (with scaling and crop)
         * 
         */
        unsigned int get_height() const;


        // === SCALER METHODS ===


        // === ANCHORS METHODS ===

        /**
         *  @brief Get current anchor points of the image instance (with scale and crop)
         * 
         *  @return Current anchor points struct copy
         * 
         */
        asset_instance_geometry::anchor_points get_anchor_points() const;

        /**
         * @brief Get the current image representation for rendering.
         *
         * The surface is borrowed from the instance and must not be freed by
         * the caller. It is valid until the instance is changed or deleted.
         */
        SDL_Surface* get_surface() const;

        // === ANCHORS METHODS ===


        // ========== METHODS ==========


    protected:

        // ===== LIFETIME =====

        /**
         * @brief Constructor - load an image asset instance.
         * 
         * Called only by the instance manager and receives the Image_asset handle of the main asset.
         * The manager registers the instance in its slot and increments the asset reference count.
         * After that it initializes
         * the scale factors to 1.0 (original size) and calculates the current_width, current_height
         * and anchor points.
         *
         * @param asset_handle Handle of the basic image asset.
         * @param r_instance_manager Responsible instance manager
         * @param r_asset_manager Responsible asset manager
         * 
         */
        explicit Image_instance(handle_ctx asset_handle, Instance_manager* r_instance_manager, Asset_manager* r_asset_manager);



        /**
         * @brief Image_instance destructor
         * 
         * Deletes the object data - calls only inside the instance manager
         * 
         */
        ~Image_instance();

        // ===== LIFETIME =====


    private:


        // ===== METHODS =====

        /**
         * @brief Main asset getter
         * 
         * Provides the access to main asset by it's handle
         * Uses to get the data from main asset inside instance
         * methods (only constructor)
         * 
         * @return Non changeable Asset pointer
         * 
         */
        const Image_asset* get_main_asset() const;



        // Inner recalculation

        // NOTE:

        // Geometry-related setters currently call recalculation methods manually.
        // Can be unified later into a single rebuild step if dependencies grow.

        /*
            CROP -> SCALE -> SIZES
        */



        // Recalculate the anchor points, based on the current width and height
        // Calls at the constructor and inside the set_scaler() method;
        void reset_anchor_points();


        // Surface renewer - calls by the instance manager
        // during the instance initialization, and by the 
        // image instance metadata control methods
        void renew_surface();


        // ===== METHODS =====



        // ===== DATA =====

        // Current crop map by 2 points
        asset_instance_geometry::crop_map_2D crop_map;


        // Current image scale factor x-axes
        float x_scaler;

        // Current image scale factor y-axes
        float y_scaler;


        // Current width (with scale and crop)
        unsigned int current_width;

        // Current height (with scale and crop)
        unsigned int current_height;


        // Nine key anchor points of the image in local space
        // with scale and crop
        asset_instance_geometry::anchor_points anchors;


        // Ready instance surface for rendering
        SDL_Surface* surface = nullptr;


        // ===== DATA =====

};


// =========================================================================================== IMAGE INSTANCE 