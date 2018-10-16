#version 330

uniform sampler2D tex;

layout(location = 0) out vec4 color;

void main()
{
	const vec4 color1 = vec4(0.6,0.0,0.0,1.0);
	const vec4 color2 = vec4(0.9,0.7,1.0,0.0);
	
	vec2 temp = gl_PointCoord - vec2(0.5);
	float f = dot(temp,temp); // 平方
	if(f > 0.25)
	   discard;
	   
	   color = mix(color1, color2,smoothstep(0.1,0.25,f));
}