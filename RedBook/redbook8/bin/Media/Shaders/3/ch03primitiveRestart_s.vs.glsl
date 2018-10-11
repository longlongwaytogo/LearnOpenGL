#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

// out vec4 vs_ps_clr; // 错误原因,ps 中的输入的常规数据类型变量名称，要和vs中的输出变量名称一致，不能随便取名，否则无法关联
// 对于结构体，则需要vs中的out 与 ps中的in类型一直，变量名可以不同
// out vec4 vs_fs_color;
out vs_out
{
	vec4 color;
} vs_out_clr;

void main()
{
	//vs_ps_clr = color;

	gl_Position = projection_matrix * (model_matrix * position);
	// vs_fs_color = color; // vs_fs_color 在vs中是out类型
	vs_out_clr.color = color;

}