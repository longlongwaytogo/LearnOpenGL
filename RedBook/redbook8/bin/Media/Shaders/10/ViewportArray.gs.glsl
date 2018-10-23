// ViewportArray.gs.glsl

#version 430 core

// 输入三角形， 4个请求(实例) ，即：几何着色器输入4个实例的数据
layout(triangles, invocations = 4) in;

// 输出三角形带, 每个请求3个顶点
layout(triangle_strip, max_vertices = 3) out;

// 矩阵
uniform mat4 model_view_matrix[4];
uniform mat4 projection_matrix;

// 来自顶点着色器的通常输入（个数为多实例个法线） invocations
in vec3 vs_normal[];  

// 将颜色和发线输出到片元着色器
out vec4 gs_color;
out vec3 gs_normal;

// instance colors

const vec4 colors[4] =  
{
	vec4(1.0, 0.7, 0.3, 1.0),
	vec4(1.0, 0.2, 0.3, 1.0),
	vec4(0.1, 0.6, 1.0, 1.0),
	vec4(0.3, 0.7, 0.5, 1.0),
};

void main()
{
	for( int i = 0; i < gl_in.length; ++i) // gl_in.length = 4, 会执行4次调用
	{
		// 设置几何图元输出到那个视口中
		gl_ViewportIndex = gl_InvocationID;
		
		// 从索引中获取颜色值
		gs_color = colors[gl_ViewportIndex];
		
		// 获取法线
		gs_normal = (model_view_matrix[gl_InvocationID] * vec4(vs_normal[i],0.0)).xyz;
		
		// position
		gl_Position = projection_matrix * model_view_matrix[gl_InvocationID] * gl_in[i].gl_Position;
		
		EmitVertex();
	}
	
}