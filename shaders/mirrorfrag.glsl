#version 430 core

in vec2 tex_coord;
layout(location = 3) uniform sampler2D tex;

out vec4 color;
void main()
{
	color = texture(tex, tex_coord);
}
