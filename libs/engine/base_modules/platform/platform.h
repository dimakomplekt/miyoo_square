// platform.h

#pragma once

// ===========================================================================================
// PLATFORM VALIDATION

// Comes from MAKE-file
#if !defined(PLATFORM_WINDOWS) && \
    !defined(PLATFORM_LINUX)   && \
    !defined(PLATFORM_MIYOO)

    #error "No platform selected."

#endif


#if defined(PLATFORM_WINDOWS) && \
    defined(PLATFORM_LINUX)

    #error "Multiple platforms selected."

#endif


#if defined(PLATFORM_WINDOWS) && \
    defined(PLATFORM_MIYOO)

    #error "Multiple platforms selected."

#endif


#if defined(PLATFORM_LINUX) && \
    defined(PLATFORM_MIYOO)

    #error "Multiple platforms selected."

#endif

// ===========================================================================================
// PLATFORM VALIDATION


// ===========================================================================================
// PLATFORM ENUM

enum class Platform
{
    Windows,
    Linux,
    Miyoo

};

// ===========================================================================================
// PLATFORM ENUM


// ===========================================================================================
// CURRENT PLATFORM

constexpr Platform CURRENT_PLATFORM =

#ifdef PLATFORM_WINDOWS

    Platform::Windows;

#elif defined(PLATFORM_LINUX)

    Platform::Linux;

#elif defined(PLATFORM_MIYOO)

    Platform::Miyoo;

#else

    #error "Unknown platform mapping"

#endif

// ===========================================================================================
// CURRENT PLATFORM


// ===========================================================================================
// COMMON DEFINES

#define SDL_MAIN_HANDLED

// ===========================================================================================
// COMMON DEFINES
