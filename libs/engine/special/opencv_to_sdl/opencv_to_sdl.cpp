// opencv_to_sdl.cpp

// =========================================================================================== IMPORT

#include "opencv_to_sdl.h"

// =========================================================================================== IMPORT


// Basic translator of the cv::Mat type to the SDL_Texture
// rescale passed sdl_texture and copy bitmap of Mat to the bitmam of SDL_Texture
void translate_cv_mat_to_sdl_texture(cv::Mat* cv_mat, SDL_Texture*& sdl_texture, SDL_Renderer* renderer)
{
    
    /**
     *
     * OPENCV cv::Mat -> SDL_Texture TRANSLATION PIPELINE
     * ============================================================================
     *
     * This function uploads image data stored inside an OpenCV cv::Mat into a
     * streaming SDL_Texture suitable for hardware rendering.
     *
     * The function owns the complete translation pipeline:
     *
     * --------------------------------------------------------------------------
     * 1. INPUT VALIDATION
     * --------------------------------------------------------------------------
     *
     * - Verifies that the passed cv::Mat exists and contains valid image data.
     * - Prevents processing of null or empty frames.
     *
     *
     * --------------------------------------------------------------------------
     * 2. IMAGE GEOMETRY
     * --------------------------------------------------------------------------
     *
     * - Reads image width and height from the OpenCV matrix.
     * - These dimensions are used to validate or recreate the destination texture.
     *
     *
     * --------------------------------------------------------------------------
     * 3. TEXTURE LIFECYCLE
     * --------------------------------------------------------------------------
     *
     * The destination texture is reused whenever possible.
     *
     * A new texture is created only when:
     *
     *   • the texture pointer is null;
     *   • the frame resolution has changed.
     *
     * Old textures are destroyed before recreation, preventing VRAM leaks.
     *
     * The texture is created as:
     *
     *   SDL_PIXELFORMAT_RGBA8888
     *   SDL_TEXTUREACCESS_STREAMING
     *
     * which matches the update pattern of continuously changing video frames.
     *
     *
     * --------------------------------------------------------------------------
     * 4. PIXEL FORMAT TRANSLATION
     * --------------------------------------------------------------------------
     *
     * OpenCV stores images in different native layouts depending on the number
     * of channels:
     *
     *   1 channel -> grayscale
     *   3 channels -> BGR
     *   4 channels -> BGRA
     *
     * The corresponding SDL pixel format is selected dynamically.
     *
     * SDL_ConvertPixels() performs:
     *
     *   • channel reordering;
     *   • pixel format conversion;
     *   • row pitch handling;
     *   • copying into the locked texture memory.
     *
     *
     * --------------------------------------------------------------------------
     * 5. TEXTURE UPDATE
     * --------------------------------------------------------------------------
     *
     * SDL_LockTexture() provides writable memory for the texture.
     *
     * SDL_ConvertPixels() writes the converted image into that memory.
     *
     * SDL_UnlockTexture() completes the update, making the new pixel data
     * available for subsequent rendering.
     *
     * ============================================================================
     *
     * Notes
     *
     * - No additional OpenCV image copies are created.
     * - Pixel conversion is delegated entirely to SDL.
     * - The caller remains the owner of cv::Mat.
     * - This function may recreate the SDL_Texture when required, therefore the
     *   texture pointer is passed by reference.
     *
     */

     
    // 1. Check input

    if (cv_mat == nullptr || cv_mat->empty())
    {
        return; 
    }


    // 2. Get the sizes

    int width = cv_mat->cols;
    int height = cv_mat->rows;


    // 3. Check sdl_texture - if there is no sdl_texture, or frame size have been changed - reallocate

    bool need_recreate = false;

    if (sdl_texture == nullptr)
    {
        need_recreate = true;
    } 
    else
    {
        float tex_w = 0.0f, tex_h = 0.0f;

        // Compare sizes
        if (SDL_GetTextureSize(sdl_texture, &tex_w, &tex_h)) 
        {
            // Need recreate
            if (static_cast<int>(tex_w) != width || static_cast<int>(tex_h) != height) 
            {
                SDL_DestroyTexture(sdl_texture);

                sdl_texture = nullptr;
                need_recreate = true;
            }
        }
    }

    if (need_recreate) 
    {
        sdl_texture = SDL_CreateTexture(

            renderer,
            SDL_PIXELFORMAT_RGBA8888,       // For format: cv::COLOR_BGR2BGRA
            SDL_TEXTUREACCESS_STREAMING,    // For texture renew with high frequency
            width,
            height

        );
        
        if (sdl_texture == nullptr)
        {
            SDL_Log("Can't create SDL texture: %s", SDL_GetError());
            return;
        }
    }

    if (!sdl_texture)
    {
        std::cout << SDL_GetError() << std::endl;
    }


    // 4. Copy pixels from cv::Mat to SDL_Texture

    void* texture_pixels = nullptr;
    int texture_pitch = 0;


    if (sdl_texture == nullptr)
    {
        std::cout << "Texture for translation is nullptr!" << std::endl;
    }


    // Block texture
    if (SDL_LockTexture(sdl_texture, nullptr, &texture_pixels, &texture_pitch))
    {
        // Dynamic format check
        SDL_PixelFormat src_format = SDL_PIXELFORMAT_UNKNOWN;
        
        if (cv_mat->channels() == 3) src_format = SDL_PIXELFORMAT_BGR24;          // Default OpenCV color
        else if (cv_mat->channels() == 1) src_format = SDL_PIXELFORMAT_INDEX8;    // Specific (grayscale)
        else if (cv_mat->channels() == 4) src_format = SDL_PIXELFORMAT_BGRA32;    // Format with alpha


        if (src_format == SDL_PIXELFORMAT_UNKNOWN)
        {
            SDL_Log("Unsupported OpenCV image format.");
            SDL_UnlockTexture(sdl_texture);
            return;
        }

        
        SDL_ConvertPixels(
            
            width,
            height,
            
            src_format,                                  // SDL format equal, which equal to current OpenCV format
            cv_mat->data,                           // Original data
            cv_mat->step,                     // Original step
            
            SDL_PIXELFORMAT_RGBA8888,        // Target format
            texture_pixels,                         // Target pixels
            texture_pitch                     // Target pitch
            
        );
    
        SDL_UnlockTexture(sdl_texture);
    }

    else 
    {
        SDL_Log("Can't block the texture: %s", SDL_GetError());
    }

}
