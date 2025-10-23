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

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	GLFWwindow *window = glfwCreateWindow(800, 300, "my window", NULL, NULL);
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		printf("GLAD COULD NOT LOAD");
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuf_callback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

