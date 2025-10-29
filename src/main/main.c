#define GLFW_INCLUDE_NONE
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuf_callback(GLFWwindow *window, int x, int y)
{
	glViewport(0, 0, x, y);
}

int main()
{
	glfwInit();

	glfwTerminate();
	return 0;
}

