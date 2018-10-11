#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 vs_fs_color;
void main()
{
	gl_Position = position;
	vs_fs_color = color;
}