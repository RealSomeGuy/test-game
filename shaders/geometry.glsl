#version 430 core

layout(location = 1) uniform mat4 projection;
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VTX
{
	vec3 normal_vec;
} gs_in[];

void gen_lines(int idx)
{
	gl_Position = projection * gl_in[idx].gl_Position;
	EmitVertex();

	gl_Position = projection * (gl_in[idx].gl_Position + 0.6f * vec4(gs_in[idx].normal_vec, 0.0f));
	EmitVertex();

	EndPrimitive();
}

void main()
{
	gen_lines(0);
	gen_lines(1);
	gen_lines(2);
}
