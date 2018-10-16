// ch06-1-TextureData
// TextureData.fs.glsl

#version 330

layout(location = 0) out vec4 fs_color;
in vec2 fs_tex_coords;

uniform sampler2D tex;

void main()
{
	fs_color = texture(tex,fs_tex_coords);
}