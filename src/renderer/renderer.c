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
	glfwWindowHint(GLFW_SAMPLES, 8);


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

FORCE_INLINE const char *get_shader_type(GLenum shader_type)
{
	switch(shader_type)
	{
	case GL_VERTEX_SHADER:
		return "VERTEX_SHADER";
	case GL_FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
	case GL_GEOMETRY_SHADER:
		return "GEOMETRY_SHADER";
	default:
		"UNKNOWN SHADER";
	}
}

static GLuint create_shader(GLenum shader_type, const char *shader_source)
{
	GLuint shader = glCreateShader(shader_type);

	FILE *fp = fopen(shader_source, "r");
	char *str = read_file(fp);

	glShaderSource(shader, 1, (const char **)&str, NULL);
	glCompileShader(shader);

	free(str);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if(!success)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);

		LOG("[%s ERROR] %s\n", get_shader_type(shader_type), log);
	}
	
	fclose(fp);
	return shader;
}

GLuint create_program(const char *vertex_path, const char *fragment_path, const char *geo_path)
{
	GLuint program = glCreateProgram();

	GLuint vertex = create_shader(GL_VERTEX_SHADER, vertex_path);
	GLuint fragment = create_shader(GL_FRAGMENT_SHADER, fragment_path);

	GLuint geometry;

	if(geo_path)
	{
		geometry = create_shader(GL_GEOMETRY_SHADER, geo_path);
		glAttachShader(program, geometry);
	}

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if(geo_path)
		glDeleteShader(geometry);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	
	if(!success)
	{
		char log[512];
		glGetProgramInfoLog(program, 512, NULL, log);

		LOG("[SHADER PROGRAM LINK ERROR] %s\n", log);
	}

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

