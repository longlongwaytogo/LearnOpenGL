// ch06-01-LoadTexture
///////////////////////////////////////////////////////////////////////\
//
// LoadTexture.cpp
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

BEGIN_APP_DECLARATION(LoadTexture)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);

    // Member variables
    float m_aspect;
    GLuint m_base_prog;
    GLuint m_vao;
    GLuint m_skybox_vbo;
    GLuint m_tex;
	GLuint m_program;
	
	VBObject m_object;

END_APP_DECLARATION()

	
#define TEST 0
DEFINE_APP(LoadTexture, "LoadTexture")

void LoadTexture::Initialize(const char * title)
{
	base::Initialize(title);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/6/TextureData.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/6/TextureData.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_program = LoadShaders(shaders);

	glUseProgram(m_program);
	GLuint tex_loc = glGetUniformLocation(m_program, "tex");
	glUniform1i(tex_loc, 0);

	glGenBuffers(1,&m_skybox_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_skybox_vbo);
	
	/*static const GLfloat quad_data[] = {
		0.75f, -0.75f,
        -0.75f, -0.75f,
        -0.75f, 0.75f,
         0.75f, 0.75f,

         0.0f, 0.0f,
         1.0f, 0.0f,
         1.0f, 1.0f,
         0.0f, 1.0f
	};*/

	static const GLfloat quad_data[] =
	{
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data),quad_data,GL_STATIC_DRAW);

	// bind vertexarray buffer
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);	
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	//glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, 0, BUFFER_OFFSET(sizeof(quad_data)));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(8 * sizeof(float)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	vglImageData image;
	std::string imageFile = Utils::instance()->getMediaPath() + "Media/textures/test.dds";
	m_tex = vglLoadTexture(imageFile.c_str(),0,&image);
	
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	vglUnloadImage(&image);
}

void LoadTexture::Finalize()
{
	glUseProgram(0);
	glDeleteTextures(1,&m_tex);
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_skybox_vbo);
}

void LoadTexture::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void LoadTexture::Display(bool auto_redraw)
{
	float t = float(GetTickCount()&0x3FF)/float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	glClearColor(0.03f, 0.03f ,0.2f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_CULL_FACE);
	glUseProgram(m_program);
	
	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D,m_tex);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
	
	base::Display();
}