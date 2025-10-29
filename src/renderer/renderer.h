#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *init_and_create_window(int x, int y, const char *title);

GLuint create_shader_program(const char *vertex_path, const char *fragment_path);

#endif
