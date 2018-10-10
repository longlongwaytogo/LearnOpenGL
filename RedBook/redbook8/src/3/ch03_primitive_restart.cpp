// ch03 图元重绘

#include <vapp.h>
#include <vutils.h>
#include <vmath.h>
#include <loadShaders.h>
#include <stdio.h>

using namespace vmath;

// Define USE_PRIMITIVE_RESTART 
#define USE_PRIMITIVE_RESTART 1

BEGIN_APP_DECLARATION(PrimitiveRestartExample)

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

DEFINE_APP(PrimitiveRestartExample,"PrimitiveRestartExample")

void PrimitiveRestartExample::Initialize(const char* title)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	base::Initialize(title);
	
	static ShaderInfo shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/3/ch03primitiveRestart.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/3/ch03primitive_restart.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_prog = LoadShaders(shader_info);
	glUseProgram(m_prog);
	
	m_model_matrix_loc = glGetUniformLocation(m_prog,"model_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_prog,"projection_matrix");
	
	// 8 顶点， 
	static const GLfloat cube_position[] = 
	{
		-1.0f, -1.0f, -1.0f, 1.0f, // left_botton_front
		-1.0f, -1.0f,  1.0f, 1.0f, // left_botton_back
		-1.0f,  1.0f, -1.0f, 1.0f, // left_top_front
		-1.0f,  1.0f,  1.0f, 1.0f, // left_top_back
		 1.0f, -1.0f, -1.0f, 1.0f, // right_botton_front
		 1.0f, -1.0f,  1.0f, 1.0f, // right_botton_back
		 1.0f,  1.0f, -1.0f, 1.0f, // right_top_front
		 1.0f,  1.0f,  1.0f, 1.0f, // right_top_back
	};
	
	// color
	static const GLfloat cube_colors[] = 
	{
	 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f
	};
	
	// indices
	static const GLushort cube_indices[] = 
	{
		0,1,2,3,6,7,4,5, // First strip
		0xFFFF,			 // restart index
		2,6,0,4,1,5,3,7
	};
	
	glGenBuffers(1,m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cube_indices),cube_indices,GL_STATIC_DRAW);

	// setup the elment array buffer
	glGenVertexArrays(1,m_vao);
	glBindVertexArray(m_vao[0]);
	
	glGenBuffers(1,m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cube_position)+sizeof(cube_colors),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(cube_position),cube_position);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(cube_position),sizeof(cube_colors),cube_colors);
	
	
	 
	// location,size, type,renormalize,strip,data
	glVertexAttribPointer(0,4,GL_FLOAT,FALSE,0,NULL);
	glVertexAttribPointer(1,4,GL_FLOAT,FALSE,0,(const GLvoid*)sizeof(cube_position));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void PrimitiveRestartExample::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	m_aspect = float(height)/float(width);
}

void PrimitiveRestartExample::Display(bool auto_redraw)
{
	float t = float(GetTickCount()&0x1FFF)/float(0x1FFF);
	static float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	// setup
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_prog);
	
	/*mat4 model_matrix(translate(0.0f,0.0f, -5.0f) * rotate(t * 720.0f,Y) * rotate(t * 360.0f,Z));
	mat4 projection_matrix(frustum(-1.0f,1.0f,-m_aspect, m_aspect,1.0f,500.0f));*/
	   mat4 model_matrix(translate(0.0f, 0.0f, -5.0f) * rotate(t * 720.0f, Y) * rotate(t * 360.0f, Z));
    vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -m_aspect, m_aspect, 1.0f, 500.0f));
	
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,model_matrix);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	
	//set up for a glDrawElements calloc
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo[0]);
	
#if USE_PRIMITIVE_RESTART
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP,17,GL_UNSIGNED_SHORT,NULL);
#else
	glDrawElements(GL_TRIANGLE_STRIP,8,GL_UNSIGNED_SHORT,NULL);
	glDrawElements(GL_TRIANGLE_STRIP,8,GL_UNSIGNED_SHORT,(const GLvoid*)(9*sizeof(GLushort)));
#endif 

	base::Display();
}
void PrimitiveRestartExample::Finalize()
{
	
	glUseProgram(0);
	glDeleteVertexArrays(1,m_vao);
	glDeleteBuffers(1,m_vbo);
	glDeleteBuffers(1,m_ebo);
	
}
 