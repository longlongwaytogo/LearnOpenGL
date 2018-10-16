// pointsprite.fs.glsl

#version 330

uniform sampler2D tex;

layout(location = 0) out vec4 color;
void main()
{
	color = texture(tex,vec2(gl_PointCoord.x,1-gl_PointCoord.y));
	//color = texture(tex,vec2(gl_PointCoord.x,gl_PointCoord.y));
}