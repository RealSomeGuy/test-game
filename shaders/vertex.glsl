#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 tex_coord;

out vec4 col;
out vec2 tex_xy;

void main()
{
	gl_Position = vec4(pos, 1.0f);

	col = vec4(color, 1.0f);

	tex_xy = tex_coord;
}
