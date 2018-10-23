// draw_xfb.vs.glsl
#version 430 core

uniform mat4 model_matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 vs_normal;

void main()
{
	vs_normal = (model_matrix * vec4(normal,0.0f)).xyz;
	gl_Position = model_matrix * position;
}
