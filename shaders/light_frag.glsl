#version 430 core

layout(location = 3) uniform vec4 color;

out vec4 col;

void main()
{
	col = color;
}
