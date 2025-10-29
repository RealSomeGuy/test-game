#include "../renderer/renderer.h"
#include "../utils/utils.h"
#include <stdlib.h>

int main()
{
	GLFWwindow *window = init_and_create_window(800, 300, "test");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float trig[] = 
	{
		//	x	y	z
			 1.0f,	-1.0f,	0.0f,
			-1.0f,	-1.0f,	0.0f,
			 0.0f,	 1.0f,	0.0f,
		//	r	g	b
			 1.0f,	 0.0f,	 0.0f,
			 0.0f,	 1.0f,	 0.0f,
			 0.0f,	 0.0f, 	 1.0f
	};

	GLuint vao, vbo;
	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(trig), trig, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(float) * 9));
	glEnableVertexAttribArray(1);

	GLuint program = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

	glUseProgram(program);
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

