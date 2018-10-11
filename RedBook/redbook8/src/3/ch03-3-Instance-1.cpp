// ch03-3-Instance_1

#include <vapp.h>
#include <vutils.h>
#include <vmath.h>
#include "vbm.h"
#include <loadShaders.h>
#include <stdio.h>

using namespace vmath;

#define INSTANCE_COUNT 100

BEGIN_APP_DECLARATION(Instance_1)

// override functions from base class 
virtual void Initialize(const char* title);
virtual void Display(bool auto_redraw);
virtual void Finalize();
virtual void Reshape(int width, int height);

private:

	float m_aspect;
	GLuint m_prog;
	
	GLuint m_color_buffer;
	GLuint m_model_matrix_buffer; 
	
	GLint m_view_matrix_loc;
    GLint m_projection_matrix_loc;
	
	VBObject 	m_object;
	
END_APP_DECLARATION()

DEFINE_APP(Instance_1 ,"ch03-2-Instance_1")

void Instance_1::Initialize(const char* title)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	base::Initialize(title);
	 
	static ShaderInfo shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/3/ch03-3-Instance-1.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/3/ch03-3-Instance-1.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_prog = LoadShaders(shader_info);
	glUseProgram(m_prog);
	
	m_view_matrix_loc = glGetUniformLocation(m_prog,"view_matrix");
	//m_model_matrix_loc = glGetUniformLocation(m_prog,"model_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_prog,"projection_matrix");
    m_object.LoadFromVBM("I:\\co\\LearnOpenGL\\RedBook\\redbook8\\oglpg-8th-edition\/media/armadillo_low.vbm", 0, 1, 2);
	m_object.BindVertexArray();
	

	int color_loc = glGetAttribLocation(m_prog,"color");
	int model_matrix_loc = glGetAttribLocation(m_prog,"model_matrix");

	vec4 colors[INSTANCE_COUNT];
	
	for(int n = 0; n < INSTANCE_COUNT; ++n)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		
		colors[n][0] = 0.5f + 0.25f * (sinf(a + 1.0f) + 1.0f);
        colors[n][1] = 0.5f + 0.25f * (sinf(b + 2.0f) + 1.0f);
        colors[n][2] = 0.5f + 0.25f * (sinf(c + 3.0f) + 1.0f);
        colors[n][3] = 1.0f;
	}
	
	// genaration color buffer
	glGenBuffers(1,&m_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER,m_color_buffer);
	// 从GPU申请内存并初始化
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_DYNAMIC_DRAW);
	// 告诉顶点着色器，顶点数据访问索引
	// slot, size,type,normalize,strip,base
	glVertexAttribPointer(color_loc,4,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(color_loc);
	
	// This is the important bit... set the divisor for the color array to
    // 1 to get OpenGL to give us a new value of 'color' per-instance
    // rather than per-vertex.
	glVertexAttribDivisor(color_loc,1);
//	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	// model_matrix 
	glGenBuffers(1,&m_model_matrix_buffer);
	glBindBuffer(GL_ARRAY_BUFFER,m_model_matrix_buffer);
	glBufferData(GL_ARRAY_BUFFER,INSTANCE_COUNT * sizeof(mat4),NULL,GL_DYNAMIC_DRAW);
	
	for(int i = 0; i < 4; i++)
	{
		// void glVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,const GLvoid * pointer);
		glVertexAttribPointer(model_matrix_loc + i, // index
				4, 							// size
				GL_FLOAT,					// type
				GL_FALSE,					// normal
				sizeof(mat4),				// stride
				(void*)(sizeof(vec4)*i)); // start offset
				
		glEnableVertexAttribArray(model_matrix_loc + i);
		glVertexAttribDivisor(model_matrix_loc + i,1);
	}
	
//	glBindBuffer(GL_ARRAY_BUFFER,0);
	  // Done (unbind the object's VAO)
    glBindVertexArray(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Instance_1::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	m_aspect = float(height)/float(width);
}

void Instance_1::Display(bool auto_redraw)
{
	float t = float(GetTickCount() & 0x3FFF) / float(0x3FFF);
    static float q = 0.0f;
    static const vec3 X(1.0f, 0.0f, 0.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    static const vec3 Z(0.0f, 0.0f, 1.0f);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	
	// Bind the weight VBO and change its data
	glBindBuffer(GL_ARRAY_BUFFER,m_model_matrix_buffer);
	mat4* matrices = (mat4*)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
	for(int n = 0; n < INSTANCE_COUNT; ++n)
	{
		float a = 50.0f * float(n) / 4.0f;
        float b = 50.0f * float(n) / 5.0f;
        float c = 50.0f * float(n) / 6.0f;

        matrices[n] = rotate(a + t * 360.0f, 1.0f, 0.0f, 0.0f) *
                      rotate(b + t * 360.0f, 0.0f, 1.0f, 0.0f) *
                      rotate(c + t * 360.0f, 0.0f, 0.0f, 1.0f) *
                      translate(10.0f + a, 40.0f + b, 50.0f + c);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	 
	
	// Active instancing program
	glUseProgram(m_prog);
	
	// set modelview 
	mat4 view_matrix(translate(0.0f, 0.0f, -1500.0f) * rotate(t * 360.0f * 2.0f, 0.0f, 1.0f, 0.0f));
	mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -m_aspect, m_aspect, 1.0f, 5000.0f));
	mat4 transform;
	glUniformMatrix4fv(m_view_matrix_loc,1,GL_FALSE,view_matrix);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	
	//glBindBuffer(GL_ARRAY_BUFFER,m_color_buffer);
	
	m_object.Render(0,INSTANCE_COUNT);
	
	// lookat(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	 
	//glBindBuffer(GL_ARRAY_BUFFER,0);
 
	//glUseProgram(0);
	
	base::Display();
}
void Instance_1::Finalize()
{
	glUseProgram(0);
	glDeleteBuffers(1, &m_color_buffer);
    glDeleteBuffers(1, &m_model_matrix_buffer);
	
}
 