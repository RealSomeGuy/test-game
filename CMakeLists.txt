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

add_custom_command(TARGET game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory 
		   ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:game>/shaders
		   COMMENT "copying shaders to output")

add_custom_command(TARGET game POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory 
		   ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game>/assets
		   COMMENT "copying assets to output")
