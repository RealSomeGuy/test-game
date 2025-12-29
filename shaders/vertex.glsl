#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;
layout(location = 4) uniform mat4 norm_mat;

out vec2 tex_xy;
out vec4 obj_pos;
out vec3 norm;
void main()
{
	obj_pos = model * vec4(pos, 1.0f);
	gl_Position = projection * view * obj_pos;
	tex_xy = tex_coord;

	norm = normalize(mat3(norm_mat) * normal);
}
