// ch07-1 light_shader-1.vs.glsl
// 方向光模拟 (如太阳光)
// 在片元着色中计算方向光照
#version 330 core

uniform mat4 mvp_matrix;
uniform mat4 normal_matrix; //  用于变换法线 



layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;     // 传入表面法线 
const vec4 vertexColor = vec4(0.0,0.8,0.5,1.0);
out vec4 vs_fs_color;
out vec3 normal;  // 输出归一化后的发线 


void main()
{
	gl_Position = mvp_matrix * vertexPosition;
	normal = normalize(mat3(normal_matrix) * vertexNormal);
	vs_fs_color = vertexColor;
}