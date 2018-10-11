// ch03-2-IndexDraw 

#include <vapp.h>
#include <vutils.h>
#include <vmath.h>
#include <loadShaders.h>
#include <stdio.h>

using namespace vmath;

BEGIN_APP_DECLARATION(IndexDraw )

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
	GLuint m_ebo[1];

	GLint m_model_matrix_loc;
    GLint m_projection_matrix_loc;
END_APP_DECLARATION()

DEFINE_APP(IndexDraw ,"ch03-2-IndexDraw")

void IndexDraw::Initialize(const char* title)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	base::Initialize(title);
	
	static ShaderInfo shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/3/ch03-2-IndexDraw.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/3/ch03-2-IndexDraw.fs.glsl"},
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

	static const GLushort IndexData[] =
	{
		0, 1, 2
	};
	static const GLsizeiptr IndexSize = sizeof(IndexData);


	glGenBuffers(1,m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexData) + sizeof(ColorData),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(VertexData),VertexData);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(VertexData),sizeof(ColorData),ColorData);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	glGenBuffers(1,m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(IndexData),IndexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// setup the elment array buffer
	glGenVertexArrays(1,m_vao);
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo[0]);
	// location,size, type,renormalize,strip,data
	glVertexAttribPointer(0,4,GL_FLOAT,FALSE,0,NULL);
	glVertexAttribPointer(1,4,GL_FLOAT,FALSE,0,(const GLvoid*)sizeof(VertexData));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void IndexDraw::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	m_aspect = float(height)/float(width);
}

void IndexDraw::Display(bool auto_redraw)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// set modelview 
	mat4 model_matrix(translate(0.0f, 0.0f, -5.0f));
	mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -m_aspect, m_aspect, 1.0f, 500.0f));
	mat4 transform;
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,model_matrix);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	
	
	glUseProgram(m_prog);
	
 
	// DrawArrays
	glBindVertexArray(m_vao[0]); 
	transform = model_matrix * translate(-3.5f,0.0f,0.0f);
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,transform);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// DrawElements;
	transform = model_matrix * translate(-1.250f,0.0f,0.0f);
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,transform);
	glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,NULL);

	// DrawElementsBaseVertex
	transform = model_matrix* translate(1.250f,0.0f,0.0f);
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,transform);
	glDrawElementsBaseVertex(GL_TRIANGLES,3,GL_UNSIGNED_SHORT,NULL,0);
	//the indices[i] + base vertex(the last element)

	// DrawArraysInstanced
	transform = model_matrix * translate(3.50f,0.0f,0.0f);
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,transform);
	glDrawArraysInstanced(GL_TRIANGLES,0,3,1);
	
	glBindVertexArray(0);
	glUseProgram(0);
	
	base::Display();
}
void IndexDraw::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(1,m_vao);
	glDeleteBuffers(1,m_vbo);
}
 