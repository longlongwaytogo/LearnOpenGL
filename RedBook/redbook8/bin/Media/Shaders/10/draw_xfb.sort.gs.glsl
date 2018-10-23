// draw_xfb.gs.glsl

#version 430 core

layout(triangles) in;
layout(points, max_vertices = 3) out;

uniform mat4 projection_matrix;

in vec3 vs_normal[];

layout(stream = 0) out vec4 rf_position;
layout(stream = 0) out vec3 rf_normal;

layout(stream = 1) out vec4 lf_position;
layout(stream = 1) out vec3 lf_normal;

void main()
{
	vec4 A = gl_in[0].gl_Position;
	vec4 B = gl_in[1].gl_Position;
	vec4 C = gl_in[2].gl_Position;
	
	vec3 AB = (B - A).xyz;
	vec3 AC = (C - A).xyz;
	vec3 face_normal = cross(AB,AC);
	
	int i;
	if(face_normal.x < 0.0) // right 
	{
		for(i = 0; i < gl_in.length; ++i)
		{
			rf_position = projection_matrix * gl_in[i].gl_Position - vec4(30.0,0.0, 0.0, 0.0); // x方向偏移30
			rf_normal = vs_normal[i];
			EmitStreamVertex(0);// 发送到Stream 0
		}
		EndStreamPrimitive(0);
	}
	else // left
	{
		for(i = 0; i< gl_in.length(); ++i)
		{
			lf_position = projection_matrix * gl_in[i].gl_Position + vec4(30.0, 0.0, 0.0, 0.0); // x方向偏移30
			lf_normal = vs_normal[i];
			EmitStreamVertex(1);
		}
		EndStreamPrimitive(1);
	}
}