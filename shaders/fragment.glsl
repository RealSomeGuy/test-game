#version 430 core

struct directional_light
{
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
};

out vec4 outcol;

in vec2 tex_xy;

layout(location = 0) uniform sampler2D e_tex;
layout(location = 9) uniform sampler2D s_tex;
layout(location = 10) uniform directional_light dir_light;
layout(location = 13) uniform sampler2D emission;
layout(location = 2) uniform mat4 view;

in vec3 obj_pos;
in vec3 normal_vec;
in vec3 light_vec[4];

vec3 diffuse = vec3(texture(e_tex, tex_xy));
vec3 specular = vec3(texture(s_tex, tex_xy));

vec3 calc_point_light()
{
	vec3 result = vec3(0.0f);
	for(int i = 0; i < 4; i++)
	{
		vec3 light_dist = light_vec[i] - obj_pos;
		vec3 light_pos = normalize(light_dist);
		vec3 view_dir = normalize(-obj_pos);

		float diff_strength = max(dot(light_pos, normal_vec), 0.0f);
		float spec_strength = pow(max(dot(reflect(-light_pos, normal_vec), 
					  view_dir), 0.0f), 512.0f);

		float d = length(light_dist);
		float attenuation = 1.0f / (0.20 * d * d + 0.22 * d + 1.0f);

		result += attenuation * (0.5 * diffuse + diff_strength * diffuse + 
					 spec_strength * specular);
	}
	return result;
}
		
vec3 calc_dir_light()
{
	vec3 light_pos = normalize(vec3(view * vec4(-dir_light.direction, 0.0f))); 
	vec3 view_dir = normalize(-obj_pos);

	float diff_strength = max(dot(light_pos, normal_vec), 0.0f);
	float spec_strength = pow(max(dot(reflect(-light_pos, normal_vec), view_dir), 0.0f),
				   512.0f);
	
	vec3 diff = dir_light.diffuse * diff_strength * diffuse;
	vec3 spec = dir_light.specular * spec_strength * specular;

	return (0.25f * dir_light.diffuse * diffuse + diff + spec);
}

vec3 calc_spotlight()
{
	//by OGL convention
	vec3 cam_front = {0.0f, 0.0f, -1.0f};

	vec3 light_dir = normalize(-obj_pos);
	float theta = dot(light_dir, -cam_front);
	float intensity = clamp((theta - 0.9063078f) / 0.0596180f, 0.0f, 1.0f);

	float diff_strength = max(dot(light_dir, normal_vec), 0.0f);
	float spec_strength = pow(max(dot(reflect(-light_dir, normal_vec), light_dir), 0.0f), 512.0f);
	float d = length(-obj_pos);

	float attenuation = 1.0f / (0.20 * d * d + 0.22f * d + 1.0f);
	
	return intensity * attenuation * (0.25f * diffuse + diff_strength * texture(emission, tex_xy).rgb * diffuse + spec_strength * specular);
}

void main()
{
	outcol = vec4(calc_point_light() + calc_spotlight() + calc_dir_light(), 1.0f);
}
