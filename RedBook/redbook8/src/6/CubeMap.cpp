// ch06-01-CubeMap
///////////////////////////////////////////////////////////////////////\
//
// CubeMap.cpp
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

BEGIN_APP_DECLARATION(CubeMap)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);

    // Member variables
    float  m_aspect;
    GLuint m_object_program;
	GLuint m_skybox_program;
    GLuint m_cube_vao;
	GLuint m_cube_vbo;
	GLuint m_cube_ebo;
	GLuint m_skybox_rotate_loc;
	GLuint m_object_mvp_loc;
	GLuint m_object_mv_loc;
    GLuint m_tex;
	VBObject m_object;

END_APP_DECLARATION()

#define TEST 0
DEFINE_APP(CubeMap, "CubeMap")

void CubeMap::Initialize(const char * title)
{
	base::Initialize(title);

	ShaderInfo shaders_skybox[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/6/TextureCubeMap.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/6/TextureCubeMap.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	ShaderInfo shaders_object[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/6/TextureCubeMapObject.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/6/TextureCubeMapObject.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_skybox_program = LoadShaders(shaders_skybox);
	m_object_program = LoadShaders(shaders_object);
 
	m_skybox_rotate_loc = glGetUniformLocation(m_skybox_program, "tc_rotate");
	m_object_mvp_loc = glGetUniformLocation(m_object_program,"mat_mvp");
	m_object_mv_loc = glGetUniformLocation(m_object_program,"mat_mv");
	
	glGenBuffers(1,&m_cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
	
  static const GLfloat cube_vertices[] =
    {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f
    };

    static const GLushort cube_indices[] =
    {
        0, 1, 2, 3, 6, 7, 4, 5,         // First strip
        2, 6, 0, 4, 1, 5, 3, 7          // Second strip
    };

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices),cube_vertices,GL_STATIC_DRAW);

	// bind vertexarray buffer
	glGenVertexArrays(1,&m_cube_vao);
	glBindVertexArray(m_cube_vao);	
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	 
	glGenBuffers(1, &m_cube_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_cube_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cube_indices),cube_indices,GL_STATIC_DRAW);
	
	vglImageData image;
	std::string imageFile = Utils::instance()->getMediaPath() + "Media/textures/TantolundenCube.dds";
	m_tex = vglLoadTexture(imageFile.c_str(),0,&image);
	
	glTexParameteri(image.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	vglUnloadImage(&image);
	std::string filePath = Utils::instance()->getMediaPath() + "Media/unit_torus.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
}

void CubeMap::Finalize()
{
	glUseProgram(0);
	glDeleteTextures(1,&m_tex);
	glDeleteVertexArrays(1,&m_cube_vao);
	glDeleteBuffers(1,&m_cube_vbo);
	glDeleteBuffers(1,&m_cube_ebo);
}

void CubeMap::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void CubeMap::Display(bool auto_redraw)
{
	static const unsigned int start_time = GetTickCount();
	float t = float(GetTickCount()-start_time)/float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	vmath::mat4 tc_matrix(vmath::mat4::identity());
	
	glClearColor(0.0f, 0.0f ,0.2f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	
	glUseProgram(m_skybox_program);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);	
	tc_matrix = vmath::perspective(35.0f, 1.0f / m_aspect, 0.1f, 100.0f) * tc_matrix;
	glUniformMatrix4fv(m_skybox_rotate_loc,1,GL_FALSE,tc_matrix);

	
	glBindVertexArray(m_cube_vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_cube_ebo);
	//glBindTexture(GL_TEXTURE_2D,m_tex);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT,NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, BUFFER_OFFSET(8 * sizeof(GLushort)));
	
	
	
	glUseProgram(m_object_program);
	tc_matrix = vmath::translate(vmath::vec3(0.0f, 0.0f ,-4.0f)) * 
				vmath::rotate(80.0f * 3.0f * t, Y) * 
				vmath::rotate(70.0f * 3.0f * t, Z);
	
	glUniformMatrix4fv(m_object_mv_loc,1,GL_FALSE,tc_matrix);
	
	tc_matrix = vmath::perspective(35.0f,1.0f/m_aspect,0.1f,100.0f) * tc_matrix;
	glUniformMatrix4fv(m_object_mvp_loc,1,GL_FALSE,tc_matrix);
	 
    glClear(GL_DEPTH_BUFFER_BIT);
	
	m_object.Render();

	base::Display();
}