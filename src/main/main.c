#include "../renderer/renderer.h"
#include "../renderer/predef_models.h"
#include "../renderer/cam.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>
#include <cglm/util.h>
#include <cglm/cam.h>
#include <cglm/io.h>

vec3 *cam_vec = NULL;
int compare_vec(const void *a, const void *b)
{
	
	float mag = glm_vec3_distance(a, *cam_vec);
	float mag2 = glm_vec3_distance(b, *cam_vec);

	return (mag < mag2) - (mag > mag2);
}

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

	GLfloat quad[] = 
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5,	0.5,  0.0f,

		 0.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f
	};

	GLuint quad_ind[] =
	{
		0, 1, 2, 1, 3, 2
	};

	GLuint qvao, qvbo, qebo;
	glGenVertexArrays(1, &qvao);
	glGenBuffers(1, &qvbo);
	glGenBuffers(1, &qebo);

	glBindVertexArray(qvao);
	glBindBuffer(GL_ARRAY_BUFFER, qvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, qebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_ind), quad_ind, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(float) * 12));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint prog = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");
	GLuint tshader = create_shader_program("shaders/outvert.glsl", "shaders/outfrag.glsl");

	GLuint tex = create_texture("assets/container2.png");
	glActiveTexture(GL_TEXTURE1);

	GLuint tex2 = create_texture("assets/transparent_window.png");

	vec3 glass_pos[5];

	for(int i = 0; i < 5; i++)
	{
		glass_pos[i][0] = 0.0f;
		glass_pos[i][1] = -0.9f;
		glass_pos[i][2] = i + 0.5f;
	}

	cam_vec = &camera.camera_pos;

	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float last_frame = 0.0f;
	while(!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		float current_frame = glfwGetTime();

		glBindVertexArray(vao);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(prog);
		sg_cam_move(window, &camera, 3.0f, current_frame - last_frame);

		mat4 projection = GLM_MAT4_IDENTITY_INIT;
		mat4 view;
		mat4 model = GLM_MAT4_IDENTITY_INIT;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glm_perspective(glm_rad(45.0f), (float)width / height, 0.1f, 100.0f, projection);
		
		sg_cam_lookat(&camera, view);
		
		glUniformMatrix4fv(1, 1, GL_FALSE, (float *)projection);
		glUniformMatrix4fv(2, 1, GL_FALSE, (float *)view);

		glm_translate(model, (vec3){0.0f, -2.0f, 0.0f});
		glm_scale(model, (vec3){15.0f, 0.2f, 15.0f});
		glUniformMatrix4fv(3, 1, GL_FALSE, (float *)model);


		glDrawElements(GL_TRIANGLES, SG_CUBE_INDEX_COUNT, GL_UNSIGNED_INT, 0);

		glBindVertexArray(qvao);
		glUseProgram(tshader);

		glUniformMatrix4fv(1, 1, GL_FALSE, (float *)projection);
		glUniformMatrix4fv(2, 1, GL_FALSE, (float *)view);
		glUniform1i(4, 1);

		glDisable(GL_DEPTH_TEST);
		qsort(glass_pos, 5, 3 * sizeof(float), compare_vec);

		for(int i = 0; i < 5; i++)
		{
			glm_mat4_identity(model);

			glm_translate(model, glass_pos[i]);
			glm_scale_uni(model, 2.0f);

			glUniformMatrix4fv(3, 1, GL_FALSE, (float *) model);
			
			glDrawElements(GL_TRIANGLES, sizeof(quad_ind) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		}

		last_frame = current_frame;

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

