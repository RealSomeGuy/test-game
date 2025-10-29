#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec4 col;

void main()
{
	gl_Position = vec4(pos, 1.0f);

	col = vec4(color, 1.0f);
}
