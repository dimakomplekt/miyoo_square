// instance.h


// =========================================================================================== IMPORT

#include "asset_manager.h"

// SDL
#include "../../engine.h"


// =========================================================================================== IMPORT



// =========================================================================================== INSTANCE BASIC CLASS



// Predeclare for friendship

class Instance_manager;


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
         * @param asset_handle Handle of the basic asset.
         * 
         */
        Instance(handle_ctx asset_handle);


        // Virtual destructor.
        // Ensures proper basic class data cleanup with subclasses ~ calls.
        virtual ~Instance();

        // ===== LIFETIME =====


        // ===== DATA =====

        // Main asset handle, passed during the construction of the instance
        const handle_ctx asset_handle;

        // ===== DATA =====

};

// =========================================================================================== INSTANCE BASIC CLASS


// =========================================================================================== IMAGE INSTANCE 


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


// ===== Helpers =====

// Descartes coordinate for 2D space.
struct desc_c_2D
{

    unsigned int x;    // Coordinate by x-axes (width).
    unsigned int y;    // Coordinate by y-axes (height).

};


// Crop map for 2D space
struct crop_map_2D 
{

    // Points choosen for width - height / x - y values coincidence

    desc_c_2D point_1;
    desc_c_2D point_2;

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
 */
struct anchor_points {

    desc_c_2D top_left;
    desc_c_2D top_center;
    desc_c_2D top_right;
    desc_c_2D center_left;
    desc_c_2D center_center;
    desc_c_2D center_right;
    desc_c_2D bottom_left;
    desc_c_2D bottom_center;
    desc_c_2D bottom_right;

};



// ===== Helpers =====



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


        // No need to getters for sizes - user only could use cropped (which could be same as original 
        // without crop)


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
         * @brief Change image scale.
         *
         * Recomputes current width and height based on the original size.
         *
         * @param x_scaler Scale factor x-axes (1.0 = original size).
         * @param y_scaler Scale factor y-axes (1.0 = original size).
         * 
         */
        void set_scaler(float x_scaler, float y_scaler);


        // Current x-axes scaler getter
        float get_x_scaler() const;

        // Current y-axes scaler getter
        float get_y_scaler() const;


        // === SCALER METHODS ===


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


        /**
         * @brief Current original asset instance crop width getter
         * 
         * @return Original (without scaling) asset-instance crop width
         * 
         */
        unsigned int get_crop_width() const;


        /**
         * @brief Current original asset instance crop height getter
         * 
         * @return Original (without scaling) asset-instance crop height
         * 
         */
        unsigned int get_crop_height() const;


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


        // === CROP METHODS ===


        // === ANCHORS METHODS ===

        /**
         *  @brief Get current anchor points of the image instance (with scale and crop)
         * 
         *  @return Current anchor points struct copy
         * 
         */
        anchor_points get_anchor_points() const;

        // === ANCHORS METHODS ===


        // ========== METHODS ==========


    protected:

        // ===== LIFETIME =====

        /**
         * @brief Constructor - load an image asset instance.
         * 
         * Calls only by the instance manager and passes the Image_asset handle of the main_asset, 
         * then registers itself in the asset's internal list of active instances. After that it initializes
         * the scale factors to 1.0 (original size) and calculates the current_width, current_height
         * and anchor points.
         *
         * @param asset_handle Handle of the basic image asset.
         * 
         */
        explicit Image_instance(handle_ctx asset_handle);


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


        /**
         * @brief Ready texture getter
         * 
         * Provides the access to precalculated texture
         * 
         * @return Non changeable SDL texture pointer
         * 
         */
        const SDL_Texture* get_texture() const;


        // Inner recalculation

        // NOTE:

        // Geometry-related setters currently call recalculation methods manually.
        // Can be unified later into a single rebuild step if dependencies grow.

        /*
             ┌─────────┐
             │ scaler  │
             └────┬────┘
                  ↕
             ┌────┴────┐
             │  size   │
             └─────────┘
        */

        // Recalculate the current_width and current_height by scaler
        void reset_size();

        // Recalculate the current scaler by new sizes
        void reset_scaler();

        
        // Recalculate the anchor points, based on the current width and height
        // Calls at the constructor and inside the set_scaler() method;
        void reset_anchor_points();


        // Texture renewer - calls by the instance manager
        // during the instance initialization, and by the 
        // image instance metadata control methods
        void renew_texture();


        // ===== METHODS =====



        // ===== DATA =====

        // Current image scale factor x-axes
        float x_scaler;

        // Current image scale factor y-axes
        float y_scaler;


        // Scaled w-dimension
        unsigned int scaled_width;

        // Scaled h-dimension
        unsigned int scaled_height;


        // Current crop map by 2 points
        crop_map_2D crop_map;

        // Crop horizontal dimension (original size of the cropped image)
        unsigned int crop_width;

        // Crop vertical dimension (original size of the cropped image)
        unsigned int crop_height;


        // Current width (with scale and crop)
        unsigned int current_width;

        // Current height (with scale and crop)
        unsigned int current_height;


        // Nine key anchor points of the image in local space
        // with scale and crop
        anchor_points anchors; 


        // Ready instance texture for rendering
        SDL_Texture* texture;


        // ===== DATA =====

};


// =========================================================================================== IMAGE INSTANCE 