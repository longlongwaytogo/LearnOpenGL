// ch03-1-BufferSubData

#include <vapp.h>
#include <vutils.h>
#include <vmath.h>
#include <loadShaders.h>
#include <stdio.h>

using namespace vmath;

BEGIN_APP_DECLARATION(BufferSubData)

// override functions from base class 
virtual void Initialize(const char* title);
virtual void Display(bool auto_redraw);
virtual void Finalize();
virtual void Reshape(int width, int height);

private:

	float m_aspect;
	GLuint m_prog;
	GLuint m_vao[1];
	GLuint m_vbo[1];

	GLint m_model_matrix_loc;
    GLint m_projection_matrix_loc;
END_APP_DECLARATION()

DEFINE_APP(BufferSubData,"ch03-1-BufferSubData")

void BufferSubData::Initialize(const char* title)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	base::Initialize(title);
	
	static ShaderInfo shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/3/ch03-1-BufferSubData.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/3/ch03-1-BufferSubData.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_prog = LoadShaders(shader_info);
	glUseProgram(m_prog);
	
	m_model_matrix_loc = glGetUniformLocation(m_prog,"model_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_prog,"projection_matrix");
	
	///////////////////////// Data /////////////////////////////////////
	static const GLfloat VertexData[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f,
		 0.0f,  1.0f, 0.0f, 1.0f,
	};
	static const GLsizei VertexSize = sizeof(VertexData);

	static const GLfloat ColorData[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f, // red
		0.0f, 1.0f, 0.0f, 1.0f, //green
		0.0f, 0.0f, 1.0f, 1.0f  //blue                                                      
	};

	// setup the elment array buffer
	glGenVertexArrays(1,m_vao);
	glBindVertexArray(m_vao[0]);
	
	glGenBuffers(1,m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexData) + sizeof(ColorData),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(VertexData),VertexData);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(VertexData),sizeof(ColorData),ColorData);
	
	// location,size, type,renormalize,strip,data
	glVertexAttribPointer(0,4,GL_FLOAT,FALSE,0,NULL);
	glVertexAttribPointer(1,4,GL_FLOAT,FALSE,0,(const GLvoid*)sizeof(VertexData));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void BufferSubData::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	m_aspect = float(height)/float(width);
}

void BufferSubData::Display(bool auto_redraw)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_prog);
	
	//set up for a glDrawElements calloc
	glBindVertexArray(m_vao[0]); 
	glDrawArrays(GL_TRIANGLES, 0, 3);

	base::Display();
}
void BufferSubData::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(1,m_vao);
	glDeleteBuffers(1,m_vbo);
}
 