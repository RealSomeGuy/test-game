#version 430 core

in vec3 cube_tex;

uniform samplerCube tex;

out vec4 col;
void main()
{
	col = texture(tex, cube_tex);
}
