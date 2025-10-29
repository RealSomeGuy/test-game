#include "../renderer/renderer.h"

int main()
{
	GLFWwindow *window = init_and_create_window(800, 300, "test");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

