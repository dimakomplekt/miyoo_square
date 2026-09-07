cmake_minimum_required(VERSION 3.15)

project(MIYOO_SQUARE LANGUAGES C CXX)


# ============================================================
# PROJECT
# ============================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ============================================================
# PLATFORM
# ============================================================

if(NOT PROJECT_PLATFORM)

    if(WIN32)

        set(PROJECT_PLATFORM WINDOWS)

    else()

        set(PROJECT_PLATFORM LINUX)

    endif()

endif()


message(STATUS "========================================")
message(STATUS "Project:  ${PROJECT_NAME}")
message(STATUS "Platform: ${PROJECT_PLATFORM}")
message(STATUS "========================================")


# ============================================================
# SOURCES
# ============================================================

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS

    "src/*.cpp"
    "libs/**/*.cpp"
    "libs/engine/special_modules/absolute_by_relative/**/*.cpp"

)


# ============================================================
# SOURCE EXCLUSIONS
# ============================================================

# The asset subsystem is still under active development and is not part of
# the SDL renderer smoke-test path. Keep it out of the build until it is ready.
list(FILTER PROJECT_SOURCES EXCLUDE
    REGEX "libs/engine/logic_modules/asset/.*"
)

list(FILTER PROJECT_SOURCES EXCLUDE
    REGEX "libs/engine/logic_modules/program_logic/asset/.*"
)


# ============================================================
# TARGET
# ============================================================

add_executable(
    MIYOO_SQUARE
    ${PROJECT_SOURCES}
)


# ============================================================
# PLATFORM DEFINES
# ============================================================

if(PROJECT_PLATFORM STREQUAL "WINDOWS")

    target_compile_definitions(
        MIYOO_SQUARE
        PRIVATE
        PLATFORM_WINDOWS
    )

elseif(PROJECT_PLATFORM STREQUAL "MIYOO")

    target_compile_definitions(
        MIYOO_SQUARE
        PRIVATE
        PLATFORM_MIYOO
    )

elseif(PROJECT_PLATFORM STREQUAL "LINUX")

    target_compile_definitions(
        MIYOO_SQUARE
        PRIVATE
        PLATFORM_LINUX
    )

else()

    message(FATAL_ERROR
        "Unknown PROJECT_PLATFORM: ${PROJECT_PLATFORM}"
    )

endif()


# ============================================================
# WINDOWS
# ============================================================

if(PROJECT_PLATFORM STREQUAL "WINDOWS")

    message(STATUS "Configuring Windows build...")


    file(MAKE_DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2"
    )


    file(COPY
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/include/"
        DESTINATION
        "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2"
    )

    file(COPY
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_TTF/SDL2_ttf-2.24.0/include/"
        DESTINATION
        "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2"
    )

    file(COPY
        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_IMAGE/SDL2_image-2.8.10/include/"
        DESTINATION
        "${CMAKE_CURRENT_BINARY_DIR}/windows_include/SDL2"
    )


    target_include_directories(
        MIYOO_SQUARE
        PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/windows_include"
    )


    target_link_libraries(
        MIYOO_SQUARE
        PRIVATE

        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/lib/x64/SDL2.lib"

        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/lib/x64/SDL2main.lib"

        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_TTF/SDL2_ttf-2.24.0/lib/x64/SDL2_ttf.lib"

        "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_IMAGE/SDL2_image-2.8.10/lib/x64/SDL2_image.lib"

    )


      # Автоматический сбор системных DLL от конфликтов MinGW
    get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    set(SYS_DLL_NAMES "libstdc++-6.dll" "libgcc_s_seh-1.dll" "libwinpthread-1.dll")

    foreach(DLL_NAME ${SYS_DLL_NAMES})
        find_program(FOUND_DLL_${DLL_NAME} NAMES ${DLL_NAME} HINTS "${MINGW_BIN_DIR}" NO_DEFAULT_PATH)
        if(FOUND_DLL_${DLL_NAME})
            add_custom_command(
                TARGET MIYOO_SQUARE
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${FOUND_DLL_${DLL_NAME}}" "$<TARGET_FILE_DIR:MIYOO_SQUARE>"
                COMMENT "Copying system dependency: ${DLL_NAME}"
            )
        endif()
    endforeach()

    # Копирование библиотек SDL и контента
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL/win_SDL2-2.32.10/lib/x64/SDL2.dll" "$<TARGET_FILE_DIR:MIYOO_SQUARE>")
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_TTF/SDL2_ttf-2.24.0/lib/x64" "$<TARGET_FILE_DIR:MIYOO_SQUARE>")
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/libs/engine/base_modules/SDL/SDL_IMAGE/SDL2_image-2.8.10/lib/x64" "$<TARGET_FILE_DIR:MIYOO_SQUARE>")
    add_custom_command(TARGET MIYOO_SQUARE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/libs/engine/logic_modules/program_gui/basic_content" "$<TARGET_FILE_DIR:MIYOO_SQUARE>/content")



    target_compile_options(
        MIYOO_SQUARE
        PRIVATE

        -fexec-charset=UTF-8
        -finput-charset=UTF-8
    )


# ============================================================
# MIYOO
# ============================================================

elseif(PROJECT_PLATFORM STREQUAL "MIYOO")

    message(STATUS "Configuring Miyoo Mini ARM build...")


    # --------------------------------------------------------
    # SYSROOT
    # --------------------------------------------------------

    set(MIYOO_SYSROOT
        "${MIYOO_TOOLCHAIN_ROOT}/mini/arm-buildroot-linux-gnueabihf/sysroot"
    )

    set(MIYOO_INCLUDE_DIR
        "${MIYOO_SYSROOT}/usr/include"
    )

    set(MIYOO_LIB_DIR
        "${MIYOO_SYSROOT}/usr/lib"
    )


    if(NOT DEFINED MIYOO_SDL2_ROOT)
        set(MIYOO_SDL2_ROOT
            "$ENV{HOME}/miyoo_sdl2/sdl2"
        )
    endif()

    set(MIYOO_SDL2_INCLUDE_DIR
        "${MIYOO_SDL2_ROOT}/include"
    )

    set(MIYOO_SDL2_LIB
        "${MIYOO_SDL2_ROOT}/build/.libs/libSDL2-2.0.so.0.18.2"
    )

    if(NOT DEFINED MIYOO_MINI_LIB_DIR)
        set(MIYOO_MINI_LIB_DIR
            "$ENV{HOME}/miyoo_sdl2/mini/lib"
        )
    endif()

    if(NOT DEFINED MIYOO_EGL_LIB)
        set(MIYOO_EGL_LIB
            "$ENV{HOME}/miyoo_sdl2/prebuilt/640x480/libEGL.so"
        )
    endif()

    if(NOT DEFINED MIYOO_GLES_LIB)
        set(MIYOO_GLES_LIB
            "$ENV{HOME}/miyoo_sdl2/prebuilt/640x480/libGLESv2.so"
        )
    endif()


    # --------------------------------------------------------
    # HEADERS
    # --------------------------------------------------------

    target_include_directories(
        MIYOO_SQUARE
        PRIVATE
        "${MIYOO_SDL2_INCLUDE_DIR}"
        "${MIYOO_INCLUDE_DIR}"
    )


    # --------------------------------------------------------
    # LINK OPTIONS
    # --------------------------------------------------------

    target_link_options(
        MIYOO_SQUARE
        PRIVATE
        -Wl,--no-as-needed
    )


    # --------------------------------------------------------
    # LIBRARIES
    # --------------------------------------------------------

    target_link_libraries(
        MIYOO_SQUARE
        PRIVATE

        "${MIYOO_EGL_LIB}"
        "${MIYOO_GLES_LIB}"

        "${MIYOO_MINI_LIB_DIR}/libmi_ao.so"
        "${MIYOO_MINI_LIB_DIR}/libshmvar.so"
        "${MIYOO_MINI_LIB_DIR}/libmi_common.so"
        "${MIYOO_MINI_LIB_DIR}/libmi_sys.so"
        "${MIYOO_MINI_LIB_DIR}/libmi_gfx.so"

        "${MIYOO_SDL2_LIB}"

        "${MIYOO_LIB_DIR}/libSDL2_image-2.0.so.0.2.3"
        "${MIYOO_LIB_DIR}/libSDL2_ttf-2.0.so.0.14.1"

        stdc++fs
    )

    # --------------------------------------------------------
    # CONTENT
    # --------------------------------------------------------

    add_custom_command(
        TARGET MIYOO_SQUARE
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory

        "${CMAKE_SOURCE_DIR}/libs/engine/logic_modules/program_gui/basic_content"

        "$<TARGET_FILE_DIR:MIYOO_SQUARE>/content"
    )


# ============================================================
# LINUX
# ============================================================

elseif(PROJECT_PLATFORM STREQUAL "LINUX")

    message(STATUS "Configuring Linux build...")


    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)


    target_include_directories(
        MIYOO_SQUARE
        PRIVATE

        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )


    target_link_libraries(
        MIYOO_SQUARE
        PRIVATE

        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
    )

endif()


# ============================================================
# MIYOO PACKAGE
# ============================================================

if(PROJECT_PLATFORM STREQUAL "MIYOO")

    set(MIYOO_PACKAGE_DIR
        "${CMAKE_BINARY_DIR}/package"
    )


    add_custom_command(
        TARGET MIYOO_SQUARE
        POST_BUILD

        # ----------------------------------------------------
        # PACKAGE DIRECTORY
        # ----------------------------------------------------

        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${MIYOO_PACKAGE_DIR}"


        # ----------------------------------------------------
        # EXECUTABLE
        # ----------------------------------------------------

        COMMAND ${CMAKE_COMMAND} -E copy

            "$<TARGET_FILE:MIYOO_SQUARE>"

            "${MIYOO_PACKAGE_DIR}/MIYOO_SQUARE"


        # ----------------------------------------------------
        # CONTENT
        # ----------------------------------------------------

        COMMAND ${CMAKE_COMMAND} -E copy_directory

            "${CMAKE_SOURCE_DIR}/libs/engine/logic_modules/program_gui/basic_content"

            "${MIYOO_PACKAGE_DIR}/content"

    )

endif()
