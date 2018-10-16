//TextureCubeMapObject.vs.glsl

#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 vs_fs_normal;
out vec3 vs_fs_position;

uniform mat4 mat_mvp;
uniform mat4 mat_mv;

void main()
{
	gl_Position =  mat_mvp * position;
	// 变换到观察空间
	vs_fs_normal = mat3(mat_mv) * normal;
	vs_fs_position = (mat_mv * position).xyz; 
}
