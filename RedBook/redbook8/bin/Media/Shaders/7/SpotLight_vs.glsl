// 聚光灯 SpotLight_vs.glsl
#version 330 core

uniform mat4 model;
uniform mat4 proj;

//in vec4 vertexColor;
in vec4 vertexPosition;
in vec3 vertexNormal;

//out vec4 color;
out vec3 normal;
out vec4 position;

void main()
{
	//color = vertexColor;
	normal = normalize(mat3(model) * vertexNormal);
	position =  model * vertexPosition;
	gl_Position = proj * position;
}
