#include "../renderer/renderer.h"
#include "../renderer/predef_models.h"
#include "../renderer/cam.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>
#include <cglm/util.h>
#include <cglm/cam.h>

int main()
{
	GLFWwindow *window = init_and_create_window(800, 300, "test");
	
	sg_cam camera = SG_CAM_INIT;

	glfwSetWindowUserPointer(window, &camera);

	glfwSetCursorPosCallback(window, sg_cam_dir);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat cube[] = SG_CUBE_VERTICES_INIT;
	GLuint indices[] = SG_CUBE_INDICES_INIT;

	GLuint vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) (SG_CUBE_NORMAL_OFFSET));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) (SG_CUBE_TEXTURE_OFFSET));
	glEnableVertexAttribArray(2);

	GLuint program = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

	glActiveTexture(GL_TEXTURE1);
	GLuint g_tex = create_texture("assets/container2.png");

	glUseProgram(program);

	glUniform1i(0, 1);

	
	glEnable(GL_DEPTH_TEST);
	
	float last_frame = 0.0f;
	while(!glfwWindowShouldClose(window))
	{
		float current_frame = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sg_cam_move(window, &camera, 3.0f, current_frame - last_frame);

		mat4 projection = GLM_MAT4_IDENTITY_INIT;
		
		int x, y;
		glfwGetFramebufferSize(window, &x, &y);
		glm_perspective(glm_rad(45.0f), (float)x / y, 0.1f, 100.0f, projection);

		mat4 view = GLM_MAT4_IDENTITY_INIT;
		sg_cam_lookat(&camera, view);

		glUniformMatrix4fv(1, 1, GL_FALSE, (float *)projection);
		glUniformMatrix4fv(2, 1, GL_FALSE, (float *)view);

		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				mat4 model = GLM_MAT4_IDENTITY_INIT;
				glm_translate(model, (vec3){i, -2.0f, j});

				glUniformMatrix4fv(3, 1, GL_FALSE, (float *)model);

				glDrawElements(GL_TRIANGLES, SG_CUBE_INDEX_COUNT, GL_UNSIGNED_INT, 0);
				glfwPollEvents();
			}
		}
		glfwSwapBuffers(window);
		
		last_frame = current_frame;
	}

	glfwTerminate();
	return 0;
}

