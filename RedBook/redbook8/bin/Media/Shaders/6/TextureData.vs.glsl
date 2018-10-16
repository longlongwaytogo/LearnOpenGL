// ch06-1-TextureData
// TextureData.vs.glsl

#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex_coords;

out vec2 fs_tex_coords;
void main()
{
	gl_Position = vec4(position,0.50,1.0);
	fs_tex_coords = tex_coords;
}