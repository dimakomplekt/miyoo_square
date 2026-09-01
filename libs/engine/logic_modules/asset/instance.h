// instance.h


// =========================================================================================== IMPORT

#include "asset_manager.h"

// =========================================================================================== IMPORT



// =========================================================================================== INSTANCE BASIC CLASS



// Predeclare for friendship

class Instance_manager;


/**
 * 
 * @brief Instance basic class
 * 
 * Contains a lifetime routine and asset pointer.
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
         */
        Instance();


        // Virtual destructor.
        // Ensures proper basic class data cleanup with subclasses ~ calls.
        virtual ~Instance();

        // ===== LIFETIME =====


        // ===== DATA =====

        // Asset of the instance pointer
        Asset* asset;

        // ===== DATA =====

};

// =========================================================================================== INSTANCE BASIC CLASS


// =========================================================================================== IMAGE INSTANCE 


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



// ===== Helpers =====



// ===== Class =====

class Image_instance : public Instance
{

    friend Asset_manager;


    public:

        // ========== METHODS ==========

        // === SIZE METHODS ===

        /**
         * @brief Get image width size.
         *
         * @return Current width 
         * 
         */
        const int get_width();

        /**
         * @brief Get image width size.
         *
         * @return Current height
         * 
         */
        const int get_height();

        
        /**
         * @brief Change image width size.
         *
         * Recalculates the instance parameters by the new width data.
         *
         * @param new_width New width value
         * 
         */
        void set_width(unsigned int new_width);

        // No need for getter - player works only with cropped size
        
        /**
         * @brief Change image width size.
         *
         * Recalculates the instance parameters by the new height data.
         *
         * @param new_height New height value
         * 
         */
        void set_height(unsigned int new_height);

        // === SIZE METHODS ===


        // === SCALER METHODS ===

        // Current x-axes scaler getter
        float get_x_scaler() const;

        // Current y-axes scaler getter
        float get_y_scaler() const;


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

        // === SCALER METHODS ===



        // ========== METHODS ==========


    protected:

        // ===== LIFETIME =====

        /**
         * @brief Constructor - load an image asset instance.
         * 
         * Calls only by the instance manager and passes the Image_asset pointer to the main_asset link, 
         * then registers itself in the asset's internal list of active instances. After that it initializes
         * the scale factors to 1.0 (original size) and calculates the current_width, current_height
         * and anchor points.
         *
         * @param asset Pointer on the basic image asset.
         * 
         */
        explicit Image_instance(Image_asset* asset);


        /**
         * @brief Image_instance destructor
         * 
         * Deletes the object data - calls only inside the instance manager
         * 
         */
        ~Image_instance();

        // ===== LIFETIME =====


        // ===== METHODS =====
        

        /**
         * 
         * @brief Ready texture getter
         * 
         * Provides the access to precalculated texture
         * 
         * @return Non changeable SDL texture pointer
         * 
         */
        const SDL_Texture* get_texture();


        // Texture renewer - calls by the instance manager
        // during the instance initialization, and by the 
        // image instance metadata control methods
        void renew_texture();

        // ===== METHODS =====



    private:


        // ===== METHODS =====


        // ===== METHODS =====



        // ===== DATA =====

        // Ready translated texture to use
        SDL_Texture* texture;




        // ===== DATA =====



};


// =========================================================================================== IMAGE INSTANCE 