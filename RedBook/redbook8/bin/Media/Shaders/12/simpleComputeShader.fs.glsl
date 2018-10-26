// simplecomputeshader.fs.glsl
#version 430 core

uniform sampler2D output_image;

layout(location = 0) out vec4 color;
void main()
{
   // texturesize — retrieve the dimensions of a level of a texture
	color = texture(output_image,vec2(gl_FragCoord.xy)/vec2(textureSize(output_image,0)));
}
