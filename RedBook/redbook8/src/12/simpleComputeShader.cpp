#include "vapp.h"
#include "vutils.h"
#include "vmath.h"
#include "LoadShaders.h"
#include <stdio.h>

BEGIN_APP_DECLARATION(SimpleComputeShaderExample)
	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
	GLuint m_compute_program;
	GLuint m_compute_shader;
	
	// Texture for compute shader to write into 
	GLuint m_output_image;
	
	GLuint m_render_program;
	GLuint m_vao;
	GLuint m_vbo;
END_APP_DECLARATION()

DEFINE_APP(SimpleComputeShaderExample, "simple compute shader example")

void SimpleComputeShaderExample::Initialize(const char* title)
{
	base::Initialize(title);

	ShaderInfo renderShaders[] = 
	{
		{GL_VERTEX_SHADER,"Media/shaders/12/simpleComputeShader.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/12/simpleComputeShader.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_render_program = LoadShaders(renderShaders);

	glGenTextures(1,&m_output_image);
	glBindTexture(GL_TEXTURE_2D,m_output_image);
	glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA32F,256,256);
	
#if 1
	m_compute_shader = glCreateShader(GL_COMPUTE_SHADER);
	
	std::string computeShaderSource =  Utils::instance()->getMediaPath() + "Media/Shaders/12/simpleComputeShader.cp.glsl";
	const char* source = ReadShader(computeShaderSource.c_str());
	// glShaderSource(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
	glShaderSource(m_compute_shader, 1, &source, NULL);
	delete[] source;
	glCompileShader(m_compute_shader);
	m_compute_program = glCreateProgram();
	glAttachShader(m_compute_program,m_compute_shader);
	glLinkProgram(m_compute_program);
	glDeleteShader(m_compute_shader);
#else
	ShaderInfo computeShaders[] = 
	{
		{GL_COMPUTE_SHADER,"Media/shaders/12/simpleComputeShader.cp.glsl"},
		{GL_NONE,NULL}
	};
	
	m_compute_program = LoadShaders(computeShaders);
#endif 

	static const float vertices[] = {
	    -1.0f, -1.0f, 0.5f, 1.0f,
         1.0f, -1.0f, 0.5f, 1.0f,
         1.0f,  1.0f, 0.5f, 1.0f,
        -1.0f,  1.0f, 0.5f, 1.0f,
	};
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1,&m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void SimpleComputeShaderExample::Display(bool auto_redraw)
{

	glUseProgram(m_compute_program);
	glBindImageTexture(0,m_output_image,0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F);

	glDispatchCompute(8,16,1);// 使用多少个单元参与运算 8*16 个单元进行计算颜色信息
		
	glBindTexture(GL_TEXTURE_2D,m_output_image);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_render_program);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);

	base::Display();
}
void SimpleComputeShaderExample::Finalize()
{
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_vbo);
	glDeleteProgram(m_compute_program);
}

void SimpleComputeShaderExample::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}
	