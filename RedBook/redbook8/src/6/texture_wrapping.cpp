// ch06-02-Texture-wrapping  纹理截断
///////////////////////////////////////////////////////////////////////\
//
// Texture_wrapping.cpp
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

#include <stdio.h>

BEGIN_APP_DECLARATION(TextureWrapp)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);

    // Member variables
    float m_aspect;
    GLuint m_base_prog;
    GLuint m_vao;
    GLuint m_quad_vbo;
    GLuint m_tex;
	GLuint m_program;
	
END_APP_DECLARATION()

	
DEFINE_APP(TextureWrapp, "TextureWrapp")

void TextureWrapp::Initialize(const char * title)
{
	base::Initialize(title);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/6/TextureData.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/6/TextureData.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_program = LoadShaders(shaders);
	glGenBuffers(1,&m_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
	
	static const GLfloat quad_data[] =
    {
        -0.75f, -0.75f,
         0.75f, -0.75f,
         0.75f, 0.75f,
        -0.75f, 0.75f,

         0.0f, 0.0f,
         4.0f, 0.0f,
         4.0f, 4.0f,
         0.0f, 4.0f
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
	
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D,m_tex);
	/*glTexStorage(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	*/
	glTexStorage2D(GL_TEXTURE_2D,4,GL_RGBA8,8,8);
	
	 static const unsigned char texture_data[] =
    {
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
    };
	/*glTexSubImage(GLenum target, GLint level, GLint xoffset, 
	GLint yoffset, GLsizei width, GLsizei height, GLenum format, 
	GLenum type, const GLvoid *pixels)*/
	glTexSubImage2D(GL_TEXTURE_2D,
	0,
	0,0,
	8,8,
	GL_RED,GL_UNSIGNED_BYTE,
	texture_data);
	static const GLint swizzles[] = {GL_RED,GL_RED,GL_RED,GL_ONE};
	glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_SWIZZLE_RGBA,swizzles);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenerateMipmap(GL_TEXTURE_2D);
}

void TextureWrapp::Finalize()
{
	glUseProgram(0);
	glDeleteTextures(1,&m_tex);
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_quad_vbo);
}

void TextureWrapp::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void TextureWrapp::Display(bool auto_redraw)
{
	unsigned int ticks = GetTickCount();
	float t = float(ticks&0x3FF)/float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	glClearColor(0.03f, 0.03f ,0.2f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_CULL_FACE);
	glUseProgram(m_program);
	
	glBindTexture(GL_TEXTURE_2D,m_tex);
	static const GLenum wrap_mode[] = { GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER,GL_REPEAT, GL_MIRRORED_REPEAT };
	int index = (ticks >>11) & 0x3 ;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode[index]);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
	
	base::Display();
}