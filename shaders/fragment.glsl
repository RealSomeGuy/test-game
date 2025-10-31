#version 430 core

in vec4 col;
out vec4 outcol;

in vec2 tex_xy;

layout(location = 0) uniform sampler2D e_tex;
void main()
{
	outcol = texture(e_tex, tex_xy);
}
