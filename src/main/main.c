#include "../renderer/renderer.h"
#include "../renderer/predef_models.h"
#include "../renderer/cam.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>
#include <cglm/util.h>
#include <cglm/cam.h>
#include <cglm/io.h>

int main()
{
	GLFWwindow *window = init_and_create_window(800, 300, "test");
	
	sg_cam camera = SG_CAM_INIT;

	glfwSetWindowUserPointer(window, &camera);

	glfwSetCursorPosCallback(window, sg_cam_dir);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(0);

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) (SG_CUBE_NORMAL_OFFSET));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) (SG_CUBE_TEXTURE_OFFSET));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint color_attach;
	glGenTextures(1, &color_attach);

	glBindTexture(GL_TEXTURE_2D, color_attach);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint rbo;

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attach, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR - FRAMEBUFFER NOT COMPLETE\n");

	GLuint mirror_fbo;
	glGenFramebuffers(1, &mirror_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mirror_fbo);

	GLuint mirror_tex;
	glGenTextures(1, &mirror_tex);
	glBindTexture(GL_TEXTURE_2D, mirror_tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint m_rbo;

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirror_tex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("ERROR - FRAMEBUFFER NOT COMPLETE\n");

	GLuint prog = create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");
	GLuint fbo_prog = create_shader_program("shaders/outvert.glsl", "shaders/outfrag.glsl");

	GLuint tex = create_texture("assets/container2.png");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	float current_frame, delta, last_frame = 0.0f;
	while(!glfwWindowShouldClose(window))
	{
		current_frame = glfwGetTime();
		delta = current_frame - last_frame;
		last_frame = current_frame; 

		char fps[24];
		snprintf(fps, 24, "%.4fms %.2ffps", delta * 1000, 1.0f / delta);
		glfwSetWindowTitle(window, fps);

		glBindTexture(GL_TEXTURE_2D, tex);
		glUseProgram(prog);
		glEnable(GL_DEPTH_TEST);

		mat4 projection = GLM_MAT4_IDENTITY_INIT;

		sg_cam_move(window, &camera, 3.0f, delta);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glm_perspective(glm_rad(45.0f), (float)width / height, 0.1f, 100.0f, projection);

		glUniformMatrix4fv(0, 1, GL_FALSE, (float *) projection);
		
		glBindFramebuffer(GL_FRAMEBUFFER, mirror_fbo);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		mat4 view;
		camera.yaw += 180.0f;
		sg_process_cam(&camera, 0, 0, 0);
		sg_cam_lookat(&camera, view);

		glUniformMatrix4fv(1, 1, GL_FALSE, (float *) view);
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				mat4 model = GLM_MAT4_IDENTITY_INIT;
				glm_translate(model, (vec3){i, -2.0f, j});

				glUniformMatrix4fv(2, 1, GL_FALSE, (float *) model);
				glDrawElements(GL_TRIANGLES, SG_CUBE_INDEX_COUNT, GL_UNSIGNED_INT, 0);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		camera.yaw -= 180.0f;
		sg_process_cam(&camera, 0, 0, 1);
		sg_cam_lookat(&camera, view);
		glUniformMatrix4fv(1, 1, GL_FALSE, (float *) view);
		for(int i = 0; i < 10; i++)
		{
			for(int j = 0; j < 10; j++)
			{
				mat4 model = GLM_MAT4_IDENTITY_INIT;
				glm_translate(model, (vec3){i, -1.0f, j});

				glUniformMatrix4fv(2, 1, GL_FALSE, (float *) model);
				glDrawElements(GL_TRIANGLES, SG_CUBE_INDEX_COUNT, GL_UNSIGNED_INT, 0);
			}
		}

		glBindTexture(GL_TEXTURE_2D, mirror_tex);

		mat4 model = GLM_MAT4_IDENTITY_INIT;
		glm_translate(model, (vec3){5.0f, 0.0f, 5.0f});

		glUniformMatrix4fv(2, 1, GL_FALSE, (float *) model);
		glDrawElements(GL_TRIANGLES, SG_CUBE_INDEX_COUNT, GL_UNSIGNED_INT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		
		glBindTexture(GL_TEXTURE_2D, color_attach);
		glUseProgram(fbo_prog);
		
		glUniform2fv(1, 1, (vec2){width, height});

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

