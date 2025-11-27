#version 430 core


layout(location = 4) uniform sampler2D tex;

in vec2 tex_xy;
out vec4 col;
void main()
{
	col = texture(tex, tex_xy);
}
