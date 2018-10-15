// ch06-01-Texture-Data 棋盘纹理
///////////////////////////////////////////////////////////////////////\
//
// TextureData.cpp
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

BEGIN_APP_DECLARATION(TextureData)
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

	
#define TEST 0
DEFINE_APP(TextureData, "TextureData")

void TextureData::Initialize(const char * title)
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

	glGenBuffers(1,&m_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
	
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
		0.75f, -0.75f,
		-0.75f, -0.75f,
		-0.75f, 0.75f,
		0.75f, 0.75f,

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
	
	
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D,m_tex);
	/*glTexStorage(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	*/

#if 1
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

#else


		GLfloat test_tex[] =
	{
		1.0f, 0.0f, 0.0f,  //red
		0.0f, 1.0f, 0.0f,  //green
		0.0f, 0.0f, 1.0f,  //blue
		1.0f, 0.0f, 1.0f,  //pink
	};

	//内部颜色格式是显卡内格式，即要显示的颜色，在测试出传入rgb分量，显示时如果设置为GL_RG32F，则考虑RG分量,外部格式中B分量被忽略
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, 2, 2);

	//外部颜色格式是数据传入表示的格式，即规定了数据怎么排列组成颜色分量,由外部格式和数据类型决定
	//在test_tex中传入四个颜色值，每三个float组成一个片元的颜色值，对应rgb分量, 
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		2, 2,
		GL_RGB, GL_FLOAT,
		test_tex);
	/*static const GLint swizzles[] = {GL_RED,GL_GREEN,GL_BLUE,GL_ONE};
	glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_SWIZZLE_RGBA,swizzles);*/
#endif 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void TextureData::Finalize()
{
	glUseProgram(0);
	glDeleteTextures(1,&m_tex);
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_quad_vbo);
}

void TextureData::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void TextureData::Display(bool auto_redraw)
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