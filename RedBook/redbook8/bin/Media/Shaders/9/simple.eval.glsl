// simple.eval.glsl
#version 430 core

uniform mat4 P;
uniform mat4 MV;

#define M_PI 3.14159265358979323846

layout(quads, equal_spacing, ccw) in;

float Hanning(vec2 p)
{
	p -= 0.5; // map unit square to [-0.5,0.5]
	float r = length(p);
	r = cos(M_PI * r / 2.0);
	r *=r;
	return r;
}

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	
#define p(i) gl_in[i].gl_Position
	
	vec4 pos = v*(u*p(0) + (1-u)*p(1)) + (1-v)*(u*p(3) + (1-u)*p(2));
	pos.z = Hanning(gl_TessCoord.xy);
	gl_Position = P * MV * pos;
}

