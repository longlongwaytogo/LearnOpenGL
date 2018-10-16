// mipmap_filter

#include <vermilion.h>

#include "vapp.h"
#include "vutils.h"
#include "vmath.h"
#include "LoadShaders.h"
#include <stdio.h>
#include <iostream>

BEGIN_APP_DECLARATION(MipMapFilter)
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
	
	float m_aspect;
	GLuint m_mipmap_program;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_tex;
	GLuint m_tc_matrix_loc;
	 
END_APP_DECLARATION()

DEFINE_APP(MipMapFilter, "mipmapFilter")

void MipMapFilter::Initialize(const char * title)
{
    base::Initialize(title);
	
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER,"Media\\Shaders\\6\\MipmapFilter.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media\\Shaders\\6\\MipmapFilter.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_mipmap_program = LoadShaders(shaders);
 
	m_tc_matrix_loc = glGetUniformLocation(m_mipmap_program,"tc_rotate");

	glGenBuffers(1,&m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
	
	static const GLfloat plane_vertices[] = {
		-20.0f, 0.0f, -50.0f,
        -20.0f, 0.0f,  50.0f,
         20.0f, 0.0f, -50.0f,
         20.0f, 0.0f,  50.0f
	};
	    
	static const GLfloat plane_texcoords[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    static const GLushort plane_indices[] =
    {
        0, 1, 2, 3
    };
	
	glBufferData(GL_ARRAY_BUFFER,sizeof(plane_vertices)+ sizeof(plane_texcoords),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(plane_vertices),plane_vertices);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(plane_vertices), sizeof(plane_texcoords),plane_texcoords);
	
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);
	
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(sizeof(plane_vertices)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glGenBuffers(1,&m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(plane_indices),plane_indices,GL_STATIC_DRAW);
	
	glGenTextures(1,&m_tex);
	glBindTexture(GL_TEXTURE_2D,m_tex);
	glTexStorage2D(GL_TEXTURE_2D,7,GL_RGBA8,64,64);
	
	unsigned int *data = new unsigned int [64*64];
	unsigned int colors[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF00FFFF, 0xFFFF00FF, 0xFFFFFF00, 0xFFFFFFFF };
	int i,j,k;
	int n;
	
	for(i = 0; i < 7; i++) // levels 
	{
		n = 0;
		for(j = 0; j < (64>>i); j++)
		{
			for(k = 0; k < (64 >>i); k++)
			{
				data[n] = (k^(64 - j)) * 0x04040404; // 按位异或
				n++;
			}
		}
		glTexSubImage2D(GL_TEXTURE_2D,i,0,0,64>>i, 64>>i, GL_RGBA,GL_UNSIGNED_BYTE,data);
	}
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 4.5f);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	delete [] data;
	
}

void MipMapFilter::Display(bool auto_redraw)
{
	static const unsigned int start_time = GetTickCount();
	float t = float(GetTickCount() - start_time) /float(0x3FFF);
	static const vmath::vec3 X(1.0f,0.0f,0.0f);
	static const vmath::vec3 Y(0.0f,1.0f,0.0f);
	static const vmath::vec3 Z(0.0f,0.0f,1.0f);
	
	  vmath::mat4 tc_matrix(vmath::mat4::identity());

    glClearColor(0.0f, 0.25f, 0.3f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glDisable(GL_CULL_FACE);
	
	glUseProgram(m_mipmap_program);
	
	tc_matrix = vmath::translate(vmath::vec3(0.0f,0.0f,-60.0f)) *
				vmath::rotate(80.0f * 3.0f * 0.03f, X);
				
	tc_matrix = vmath::perspective(35.0f, 1.0f / m_aspect, 0.1f, 700.0f) * tc_matrix;
	
	glUniformMatrix4fv(m_tc_matrix_loc, 1, GL_FALSE, tc_matrix);
  
    t = fmodf(t, 1.0f);
	if(t < 0.25f)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	}
	else if( t < 0.5f)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	}
	else if( t < 0.75f)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
	}
	else 
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	
	std::cout << "t:" <<t << std::endl;
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT,BUFFER_OFFSET(8 * sizeof(GL_UNSIGNED_SHORT)));
	
	base::Display();
	
}

void MipMapFilter::Finalize()
{
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_vbo);
	glDeleteBuffers(1,&m_ebo);
	
}

void MipMapFilter::Reshape(int width, int height)
{
    glViewport(0, 0 , width, height);

    m_aspect = float(height) / float(width);
}
