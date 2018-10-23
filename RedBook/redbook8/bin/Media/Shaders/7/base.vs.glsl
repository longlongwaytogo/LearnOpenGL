// base.vs.glsl

#version 430 core


uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 shadow_matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vs_fs_out
{
	vec4 shadow_coord;
	vec3 world_coord;
	vec3 eye_coord;
	vec3 normal;

} info;

void main()
{
	vec4 world_pos = model_matrix * position;
	vec4 eye_pos = view_matrix * world_pos;
	vec4 clip_pos = projection_matrix * eye_pos;
	
	info.world_coord = world_pos.xyz;
	info.eye_coord = eye_pos.xyz;
	info.shadow_coord = shadow_matrix * world_pos; 
	info.normal = mat3(view_matrix * model_matrix) * normal;
	
	gl_Position =  clip_pos;
	
}
