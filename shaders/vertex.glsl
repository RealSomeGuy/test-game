#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

out vec2 tex_xy;

layout(location = 1) uniform mat4 projection;
layout(location = 2) uniform mat4 view;
layout(location = 3) uniform mat4 model;

layout(location = 4) uniform vec3 light_pos[4];
layout(location = 8) uniform mat4 norm_mat;

out vec3 obj_pos;
out vec3 normal_vec;
out vec3 light_vec[4];
void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);

	tex_xy = tex_coord;

	obj_pos = vec3(view * model * vec4(pos, 1.0f));

	for(int i = 0; i < 4; i++)
		light_vec[i] = vec3(view * vec4(light_pos[i], 1.0f));

	normal_vec = normalize(mat3(norm_mat) * normal);
}
