#version 430 core

layout (location = 0) in vec3 pos;

layout (location = 0) uniform mat4 projection;
layout (location = 1) uniform mat4 view;

out vec3 cube_tex;
void main()
{
	vec4 outp = projection * mat4(mat3(view)) * vec4(pos, 1.0f);

	gl_Position = outp.xyww;
	cube_tex = pos;
}
