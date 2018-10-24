// fur.gs.glsl
#version 430 core

layout(triangles) in;
layout(triangle_strip,max_vertices = 140) out;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

uniform int fur_layers = 30;
uniform float fur_depth  = 5.0;

// 可能有多个输入，所以要是用数组形式
in VS_GS_VERTEX
{
	vec3 normal;
	vec2 tex_coord;
} vertex_in[];

out GS_FS_VERTEX
{
	vec3 normal;
	vec2 tex_coord;
	flat float fur_strength;  //  flat: 不对片段输入插值
} vertex_out;

void main()
{
	int i, layer;
	float disp_delta = 1.0 / float(fur_layers);
	float d = 0.0;
	vec4 position;
	for(layer = 0; layer < fur_layers; ++layer)
	{
		for(i = 0; i < gl_in.length(); ++i)
		{
			vec3 n = vertex_in[i].normal;
			vertex_out.normal = mat3(model_matrix) * n;
			vertex_out.tex_coord = vertex_in[i].tex_coord;
			vertex_out.fur_strength = 1.0 - d;
			position = gl_in[i].gl_Position + vec4(n * d * fur_depth, 0.0);
			gl_Position = projection_matrix * model_matrix * position;
			EmitVertex();
		}
		d += disp_delta;
		EndPrimitive();
	}
}