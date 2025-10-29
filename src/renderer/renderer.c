#include "renderer.h"
#include "../utils/utils.h"
#include <stdlib.h>

static void error_callback(int error_code, const char *msg)
{
	LOG("[GLFW ERROR] code - %d, msg - %s\n", error_code);
}

static void framebuffer_callback(GLFWwindow *window, int x, int y)
{
	glViewport(0, 0, x, y);
}

GLFWwindow *init_and_create_window(int x, int y, const char *title)
{
	glfwSetErrorCallback(error_callback);
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


	GLFWwindow *window = glfwCreateWindow(x, y, title, NULL, NULL);
	
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		LOG("FAILED TO LOAD GLAD\n");
		return NULL;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_callback);

	glViewport(0, 0, x, y);

	return window;
}

