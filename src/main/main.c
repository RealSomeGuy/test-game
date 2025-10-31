#include "../renderer/renderer.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>
#include <cglm/util.h>
#include <cglm/cam.h>

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
			 0.0f,	 0.0f, 	 1.0f,
		//	s	t
			1.0f,	0.0f,
			0.0f,	0.0f,
			0.5f,	0.5f
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

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(float) * 18));
	glEnableVertexAttribArray(2);

	GLuint program = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

	glActiveTexture(GL_TEXTURE1);
	GLuint g_tex = create_texture("assets/grass.png");

	glUseProgram(program);

	glUniform1i(0, 1);

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	
	glm_perspective(glm_rad(45.0f), 2, 0.1f, 100.0f, projection);
	glUniformMatrix4fv(1, 1, GL_FALSE, (float *)projection);
	
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		mat4 view = GLM_MAT4_IDENTITY_INIT;
		mat4 model = GLM_MAT4_IDENTITY_INIT;

		glm_translate(view, (vec3){0.0f, 0.0f, -2.0f});

		glm_scale_uni(model, 0.5);
		glm_rotate(model, glfwGetTime(), (vec3){0.0f, 0.0f, 1.0f});

		glUniformMatrix4fv(2, 1, GL_FALSE, (float *)view);
		glUniformMatrix4fv(3, 1, GL_FALSE, (float *)model);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

