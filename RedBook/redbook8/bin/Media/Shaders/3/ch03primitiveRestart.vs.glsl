#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

// out vec4 vs_ps_clr; // 错误原因,ps z中的输入变量名称，要和vs中的输出变量名称一致，不能随便取名，否则无法关联
   out vec4 vs_fs_color;
void main()
{
	//vs_ps_clr = color;

	gl_Position = projection_matrix * (model_matrix * position);
	vs_fs_color = color; // vs_fs_color 在vs中是out类型

}