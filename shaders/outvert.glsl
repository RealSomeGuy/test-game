#version 430 core

out vec2 tex_xy;
void main()
{
	vec4 tri_coord[6] = 
	{
		vec4(-1.0f, -1.0f, 0.0f, 1.0f),
		vec4(1.0f, -1.0f, 0.0f, 1.0f),
		vec4(-1.0f, 1.0f, 0.0f, 1.0f),

		vec4(1.0f, -1.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(-1.0f, 1.0f, 0.0f, 1.0f)
	};

	vec2 tex_coord[6] =
	{
		vec2(0.0f, 0.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 1.0f),
		
		vec2(1.0f, 0.0f),
		vec2(1.0f, 1.0f),
		vec2(0.0f, 1.0f)
	};

	gl_Position = tri_coord[gl_VertexID];

	tex_xy = tex_coord[gl_VertexID];
}
