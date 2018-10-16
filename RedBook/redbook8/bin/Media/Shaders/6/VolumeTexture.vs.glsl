// VolumeTexture.vs.glsl
#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 coord;

uniform mat4 tc_rotate;

out vec3 vs_fs_coord;

void main()
{
	gl_Position =  vec4(position,0.5,1.0);
	vs_fs_coord = (vec4(coord,0.0,1.0) * tc_rotate).stp;
}