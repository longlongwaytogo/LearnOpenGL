// VolumeTexture.fs.glsl

#version 330 core

in vec3 vs_fs_coord;

layout(location = 0) out vec4 color;
uniform sampler3D tex;

void main()
{
	color = texture(tex,vs_fs_coord).rrrr;
}
