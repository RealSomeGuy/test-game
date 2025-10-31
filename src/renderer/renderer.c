#include "renderer.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <stb_image.h>

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

static const char *type_to_str(GLenum type)
{
	switch(type)
	{
	case GL_VERTEX_SHADER:
		return "VERTEX_SHADER";
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
	default:
		return "UNKNOWN SHADER";
	}
}

static GLuint compile_shader(char *src, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);

	glShaderSource(shader, 1, (const char **) &src, NULL);

	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		char log[1024];
		glGetShaderInfoLog(shader, 1024, NULL, log);

		LOG("[%s COMPILE ERROR] - %s\n", type_to_str(shader_type), log);
	}

	return shader;
}


GLuint create_shader_program(const char *vertex_path, const char *fragment_path)
{
	FILE *fp = fopen(vertex_path, "r");

	char *shader_src = read_file(fp);
	
	GLuint vertex = compile_shader(shader_src, GL_VERTEX_SHADER);
	free(shader_src);

	fclose(fp);
	fp = fopen(fragment_path, "r");

	shader_src = read_file(fp);

	GLuint fragment = compile_shader(shader_src, GL_FRAGMENT_SHADER);
	free(shader_src);

	fclose(fp);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if(!success)
	{
		char log[1024];

		glGetProgramInfoLog(program, 1024, NULL, log);
		LOG("[PROGRAM LINK ERROR] - %s\n", log);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

GLuint create_texture(const char *texpath)
{
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				GL_LINEAR_MIPMAP_LINEAR);

	stbi_set_flip_vertically_on_load(1);
	
	int x, y, n;
	uint8_t *data = stbi_load(texpath, &x, &y, &n, 4);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

