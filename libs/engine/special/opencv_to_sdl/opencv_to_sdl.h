// opencv_to_sdl.h

#pragma once

// =========================================================================================== IMPORT

#include <opencv2/opencv.hpp>
#include <opencv2/geometry.hpp>
#include <SDL3/SDL.h>

#include <vector>

// =========================================================================================== IMPORT


// Basic translator of the cv::Mat type to the SDL_Texture
// rescale passed texture and copy bitmap of Mat to the bitmam of SDL_Texture
void translate_cv_mat_to_sdl_texture(cv::Mat* cv_mat, SDL_Texture*& sdl_texture, SDL_Renderer* renderer);