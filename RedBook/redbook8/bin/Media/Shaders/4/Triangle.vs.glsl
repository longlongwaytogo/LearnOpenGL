// Triangle.vs.glsl
#version 330
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 frag_color;
void main()
{
	gl_Position = vec4(position,0.0f,1.0f);
	frag_color = color;
}