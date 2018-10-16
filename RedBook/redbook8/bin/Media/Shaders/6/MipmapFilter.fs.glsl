// MimapFilter.fs.glsl

#version 330 

in vec2 tex_coord;

uniform sampler2D tex;

layout(location = 0) out vec4 color;
void main()
{
	color = texture(tex,tex_coord);
}