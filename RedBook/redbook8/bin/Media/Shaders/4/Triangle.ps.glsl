//Triangle.ps.glsl

#version 330

 in vec4 frag_color;
layout(location = 0)out vec4 out_fragColor;
void main()
{
	out_fragColor = frag_color;
	
}