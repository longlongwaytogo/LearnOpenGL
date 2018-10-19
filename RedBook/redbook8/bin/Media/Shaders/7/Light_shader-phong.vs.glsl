// Light_Shader-phong.vs.glsl

#version 430 core

uniform mat4 model;
uniform mat4 proj;

layout(location = 0) in vec4 Position;
layout(location = 1) in vec3 normal;

out VS_OUT
{
	vec3 worldPos;
	vec3 normal;
} vs_out;


void main()
{
#if 1
	 vec4 pos = proj * model * Position;
	 vs_out.worldPos = pos.xyz;
	 vs_out.normal = mat3(model) * normal;
	 
	 gl_Position = pos;
#else
	 vec4 pos = model * Position;
	// vs_out.worldPos = pos.xyz /  pos.w;
	
	 vs_out.worldPos = pos.xyz; 
	 vs_out.normal = mat3(model) * normal;
	 
	 gl_Position = proj * pos;
#endif 
}