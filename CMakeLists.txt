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
add_subdirectory(src/renderer)
add_subdirectory(src/utils)
add_subdirectory(stb_image)

target_link_libraries(game PRIVATE glfw glad stb_image)
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(shaders ALL COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${CMAKE_SOURCE_DIR}/shaders
			${CMAKE_BINARY_DIR}/shaders
			COMMENT "copying shaders to build dir")

add_custom_target(assets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${CMAKE_SOURCE_DIR}/assets
			${CMAKE_BINARY_DIR}/assets
			COMMENT "copying assets to build dir")

add_dependencies(game shaders assets)
