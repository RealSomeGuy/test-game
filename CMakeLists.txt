cmake_minimum_required(VERSION 3.30)

project(game_engine VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)

add_subdirectory(glfw-3.4)
add_subdirectory(glad)

add_executable(game)

add_subdirectory(src/main)

target_link_libraries(game PRIVATE glfw glad)
