//ViewportArray.fs.glsl
#version 430 core

layout(location = 0) out vec4 color;

in vec4 gs_color;
in vec3 gs_normal;

void main()
{
	color = gs_color *(0.2 + pow(abs(gs_normal.z),4.0));
}