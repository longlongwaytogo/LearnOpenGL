// ch06-01-Texture-Data 棋盘纹理 -2 
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
    GLuint vao;
    GLuint vbo;
    GLuint tex;
	GLuint m_program;
	
END_APP_DECLARATION()

	
#define TEST 0
DEFINE_APP(TextureData, "TextureData-2")

void TextureData::Initialize(const char * title)
{
	base::Initialize(title);

	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/6/TextureData2.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/6/TextureData2.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_program = LoadShaders(shaders);

	glUseProgram(m_program);
	GLuint tex_loc = glGetUniformLocation(m_program, "tex");
	glUniform1i(tex_loc, 0);

	// m_VertexCount = VertexCount;
	static const GLfloat VertexData[] =
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
	static const GLsizei VertexSize = sizeof(VertexData);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8 * sizeof(GLfloat)+NULL));

	// tex init 
		glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	
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
	/*
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 8, 8);
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		8, 8,
		GL_RGBA, GL_UNSIGNED_BYTE,
		tex_data);
		*/
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
	glDeleteTextures(1,&tex);
	glDeleteVertexArrays(1,&vao);
	glDeleteBuffers(1,&vbo);
}

void TextureData::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void TextureData::Display(bool auto_redraw)
{
	
	glClearColor(0.03f, 0.03f ,0.2f,1.0f);
	//glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	base::Display();
}