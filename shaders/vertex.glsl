#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

out vec2 tex_xy;
void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	tex_xy = tex_coord;
}
