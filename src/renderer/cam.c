#include "cam.h"
#include <math.h>
#include <cglm/util.h>
#include <cglm/vec3-ext.h>

void sg_cam_dir(GLFWwindow *window, double xpos, double ypos)
{
	sg_cam *cam = glfwGetWindowUserPointer(window);
	float xoffset = xpos - cam->lastx;
	float yoffset = cam->lasty - ypos;

	cam->lastx = xpos;
	cam->lasty = ypos;

	cam->pitch += yoffset * cam->sensitivity; 
	cam->yaw += xoffset * cam->sensitivity;

	if(cam->pitch > 89.0f)
		cam->pitch = 89.0f;
	else if(cam->pitch < -89.0f)
		cam->pitch = -89.0f;

	cam->camera_front[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
	cam->camera_front[1] = sin(glm_rad(cam->pitch));
	cam->camera_front[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));


	glm_vec3_normalize(cam->camera_front);
}

void sg_cam_move(GLFWwindow *window, sg_cam *cam, float speed, float delta_time)
{
	vec3 direction = GLM_VEC3_ZERO_INIT;
	vec3 right;

	glm_vec3_cross(cam->camera_front, cam->world_up, right);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		glm_vec3_add(direction, cam->camera_front, direction);
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		glm_vec3_sub(direction, cam->camera_front, direction);

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		glm_vec3_add(direction, right, direction);
	else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		glm_vec3_sub(direction, right, direction);

	direction[1] = 0.0f;

	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glm_vec3_add(direction, cam->world_up, direction);
	else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		glm_vec3_sub(direction, cam->world_up, direction);
	
	if(!glm_vec3_eqv(direction, GLM_VEC3_ZERO))
	{
		glm_vec3_normalize(direction);
		glm_vec3_scale(direction, speed * delta_time, direction);

		glm_vec3_add(cam->camera_pos, direction, cam->camera_pos);
	}
}
