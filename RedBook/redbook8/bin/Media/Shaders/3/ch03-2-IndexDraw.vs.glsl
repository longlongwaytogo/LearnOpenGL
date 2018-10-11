// ch03-2-IndexDraw.vs.glsl

#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

out vec4 vs_out_clr;

void main()
{
	gl_Position = projection_matrix * model_matrix * position;
	vs_out_clr = color;
}