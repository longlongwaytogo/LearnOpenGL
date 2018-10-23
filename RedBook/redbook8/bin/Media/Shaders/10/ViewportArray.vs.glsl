// ViewportArray.vs.glsl

#version 430 core 

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 vs_normal;

void main()
{
	gl_Position = position;
	vs_normal = normal;
}