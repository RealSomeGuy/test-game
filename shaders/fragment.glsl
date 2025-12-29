#version 430 core

layout(location = 3) uniform sampler2D tex;
layout(location = 5) uniform samplerCube cutex;
layout(location = 6) uniform vec3 camera_pos;

in vec2 tex_xy;
out vec4 outcol;

in vec4 obj_pos;
in vec3 norm;
void main()
{
	vec3 view_pos = normalize(obj_pos.xyz - camera_pos);
	outcol = texture(cutex, refract(view_pos, norm, 1.00 / 2.52));
}
