#version 430 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_norm;
layout(location = 2) in vec2 v_tex;

layout(location = 0) in mat4 projection;
layout(location = 1) in  mat4 view;
layout(location = 2) in  mat4 model;

out vec2 tex_coord;

void main()
{
	gl_Position = projection * view * model * vec4(v_pos, 1.0f);

	tex_coord = v_tex;
}
