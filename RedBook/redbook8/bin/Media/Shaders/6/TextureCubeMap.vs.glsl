// ch06-1-TextureData
// TextureData.vs.glsl

#version 330

layout(location = 0) in vec3 position;
out vec3 tex_coord;

uniform mat4 tc_rotate;

void main()
{
	gl_Position = tc_rotate * vec4(position,1.0); // tc_rotate 为投影矩阵
	tex_coord = position;
}