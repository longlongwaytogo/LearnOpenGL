#version 330

//in vec4  vs_fs_color;

in vs_out
{
	vec4 color;
} vs_in_clr;

//layout(location = 0) out vec4 color;
 out vec4 color;
void main()
{
	color = vs_in_clr.color;
}