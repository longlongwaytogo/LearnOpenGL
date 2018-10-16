///////////////////////////////////////////////////////////////////////\
//
// VolumeTexture.cpp
//
/////////////////////////////////////////////////////////////////

//#define USE_GL3W

//#ifdef USSE_GL3W
//#include <GL3/gl3w.h> 
//#else
//#include <GL/glew.h>
//#endif 

#include <vermilion.h>

#include "vapp.h"
#include "vutils.h"
 
#include "LoadShaders.h"
#include "vmath.h"

#include <vbm.h>

#include <stdio.h>

BEGIN_APP_DECLARATION(VolumeTexture)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);

    // Member variables
    float  m_aspect;
    GLuint m_program;
	 
    GLuint m_vao;
	GLuint m_quad_vbo;
	 
    GLuint m_tex;
	GLuint m_tc_rotate_loc;

END_APP_DECLARATION()

#define TEST 0
DEFINE_APP(VolumeTexture, "VolumeTexture")

void VolumeTexture::Initialize(const char * title)
{
	base::Initialize(title);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/6/VolumeTexture.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/6/VolumeTexture.fs.glsl"},
		{GL_NONE,NULL}
	};

	m_program = LoadShaders(shaders);
 
	m_tc_rotate_loc = glGetUniformLocation(m_program, "tc_rotate");
 
	glGenBuffers(1,&m_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
	
   static const GLfloat quad_data[] =
    {
         1.0f, -1.0f, // vertex
        -1.0f, -1.0f,
        -1.0f, 1.0f,
         1.0f, 1.0f,

         0.0f, 0.0f, // color
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
    };
	
	glBufferData(GL_ARRAY_BUFFER,sizeof(quad_data),quad_data,GL_STATIC_DRAW);

	// bind vertexarray buffer
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);	
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*8));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	 
	vglImageData image;
	std::string imageFile = Utils::instance()->getMediaPath() + "Media/textures/cloud.dds";
	m_tex = vglLoadTexture(imageFile.c_str(),0,&image);
	
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	vglUnloadImage(&image);
	 
}

void VolumeTexture::Finalize()
{
	glUseProgram(0);
	glDeleteTextures(1,&m_tex);
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_quad_vbo);
 
}

void VolumeTexture::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void VolumeTexture::Display(bool auto_redraw)
{
	static const unsigned int start_time = GetTickCount();
	float t = float(GetTickCount()- start_time)/float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	vmath::mat4 tc_matrix(vmath::mat4::identity());
	
	//glClearColor(0.0f, 0.0f ,0.2f,1.0f);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	
	glUseProgram(m_program);

	tc_matrix = vmath::rotate(t * 170.0f, X) *
				vmath::rotate(t * 137.0f, Y) *
				vmath::rotate(t * 93.0f, Z);
				
	glUniformMatrix4fv(m_tc_rotate_loc,1,GL_FALSE,tc_matrix);
	
	glBindVertexArray(m_vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_cube_ebo);
	//glBindTexture(GL_TEXTURE_2D,m_tex);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
	
	base::Display();
}