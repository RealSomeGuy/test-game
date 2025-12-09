#version 430 core


layout(location = 0) uniform sampler2D tex;
layout(location = 1) uniform vec2 scr_size;

in vec2 tex_xy;
out vec4 col;
void main()
{
	const vec2 pixel_size = vec2(1.0f / scr_size.x, 1.0f / scr_size.y);

	vec2 pixels[9] = 
	{
		vec2(-pixel_size.x, pixel_size.y),
		vec2(0.0f, pixel_size.y),
		vec2(pixel_size.x, pixel_size.y),
		vec2(-pixel_size.x, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(pixel_size.x, 0.0f),
		vec2(-pixel_size.x, -pixel_size.y),
		vec2(0.0f, -pixel_size.y),
		vec2(pixel_size.x, -pixel_size.y)
	};

	float kernel[9] =
	{
		1.0/10, 2.0/10, 1.0/10,
		2.0/10, 0.0/10, 2.0/10,
		1.0/10, 2.0/10, 1.0/10
	};

	vec3 sample_tex = vec3(0.0f);

	for(int i = 0; i < 9; i++)
		sample_tex += texture(tex, tex_xy + pixels[i]).rgb * kernel[i];

	col = vec4(sample_tex, 1.0f);
}
