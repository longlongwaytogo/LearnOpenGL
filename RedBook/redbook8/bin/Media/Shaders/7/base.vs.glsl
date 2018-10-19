// base.vs.glsl

#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 shadow_matrix;

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
	info.normal = mat3(view_matrix * model_matrix) * normal;
	info.shadow_coord = shadow_matrix * world_pos; 
	
	gl_Position = projection_matrix * model_matrix *view_matrix * position;
	
}
