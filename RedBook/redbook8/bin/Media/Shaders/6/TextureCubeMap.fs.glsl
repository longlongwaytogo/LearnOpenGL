// ch06-1-TextureCubeMap
// TextureCubeMap.fs.glsl

#version 330 core

layout(location = 0) out vec4 fs_color;
in vec3 tex_coord; 

uniform samplerCube tex;

void main()
{
	fs_color = texture(tex,tex_coord);
}