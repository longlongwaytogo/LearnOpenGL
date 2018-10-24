// DoubleWrite_blit.fs.glsl

#version 430 core

layout(location = 0) out vec4 color;

layout(binding = 0, rgba32f) uniform image2D output_image;
void main()
{
	color = vec4(imageLoad(output_image,ivec2(gl_FragCoord.xy))).xyxy / 255.0;
}