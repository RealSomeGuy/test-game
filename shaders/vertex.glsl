#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex_coord;

out vec4 col;
out vec2 tex_xy;

layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 view;
layout(location = 3) uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);

	col = vec4(color, 1.0f);

	tex_xy = tex_coord;
}
