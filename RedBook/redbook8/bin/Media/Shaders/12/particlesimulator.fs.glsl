//  particlesimulator.fs.glsl

#version 430 core

layout(location = 0) out vec4 color;

// 来自顶点着色，粒子年龄
in float intensity;

void main()
{
	// 根据粒子年龄，在红色和蓝色间差值
	 // color = mix(vec4(0.0f, 0.2f, 1.0f, 1.0f), vec4(0.2f, 0.05f, 0.0f, 1.0f),intensity);
	  color = mix(vec4(0.0f, 0.2f, 1.0f, 1.0f), vec4(0.2f, 0.05f, 0.0f, 1.0f), intensity);

}