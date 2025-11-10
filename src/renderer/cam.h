#ifndef CAM_H
#define CAM_H
#include <GLFW/glfw3.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <cglm/cam.h>

#include "../utils/utils.h"

#define SG_CAM_INIT { {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, -90.0f\
		      ,0.0f, 0.0f, 0.0f, 0.5f}

typedef struct
{
	vec3 world_up;
	vec3 camera_pos;

	vec3 camera_front;

	float pitch;
	float yaw;

	float lastx;
	float lasty;

	float sensitivity;
}sg_cam;

void sg_cam_dir(GLFWwindow *window, double xpos, double ypos);
void sg_cam_move(GLFWwindow *window, sg_cam *cam, float speed, float delta_time);

static FORCE_INLINE void sg_cam_lookat(sg_cam *cam, mat4 view)
{
	vec3 dir;
	glm_vec3_add(cam->camera_pos, cam->camera_front, dir);

	glm_lookat(cam->camera_pos, dir, cam->world_up, view);
}

#endif
