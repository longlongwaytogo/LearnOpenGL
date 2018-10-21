// simple.cont.glsl
#version 430

layout( vertices = 4) out; // 设置控制细分的输出参数
void main()
{
	gl_TessLevelInner[0] = 8;
	gl_TessLevelInner[1] = 8;
	
	gl_TessLevelOuter[0] = 4;
	gl_TessLevelOuter[1] = 4;
	gl_TessLevelOuter[2] = 4;
	gl_TessLevelOuter[3] = 4;
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}