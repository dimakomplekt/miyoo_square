cmake_minimum_required(VERSION 3.20)

project(miyoo_square VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Paths
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_STATE_MACHINE_DIR "${CMAKE_SOURCE_DIR}/libs/state_machine")
set(LIB_LANG_STATE_DIR "${CMAKE_SOURCE_DIR}/libs/lang_state")
set(LIB_ENGINE_DIR "${CMAKE_SOURCE_DIR}/libs/engine")

# Executable
add_executable(miyoo_square
    ${SRC_DIR}/main.cpp
    ${LIB_STATE_MACHINE_DIR}/state_machine.cpp
    ${LIB_STATE_MACHINE_DIR}/game_states.cpp
    ${LIB_LANG_STATE_DIR}/lang_state.cpp
    ${LIB_ENGINE_DIR}/app.cpp
)

# Includes
target_include_directories(miyoo_square PRIVATE
    ${LIB_STATE_MACHINE_DIR}
    ${LIB_LANG_STATE_DIR}
    ${LIB_ENGINE_DIR}
)

# SDL2 (MSYS2)
find_package(SDL2 REQUIRED)
target_link_libraries(miyoo_square
    SDL2::SDL2   # <- без SDL2main
)
