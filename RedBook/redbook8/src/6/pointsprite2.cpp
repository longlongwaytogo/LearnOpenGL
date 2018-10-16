// pointsprite.cpp

#include "vapp.h"
#include "vutils.h"

#include "LoadShaders.h"

#include "vmath.h"

#include <stdlib.h>

using namespace vmath;

#define POINT_COUNT 400

BEGIN_APP_DECLARATION(PointSpriteExample)

	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
	private:
		float m_aspect;
		GLuint m_program;
		GLuint m_vbo;
		GLuint m_vao;
		GLuint m_sprite_tex;
		
		GLuint m_model_matrix_loc;
		GLuint m_projection_matrix_loc;
		
		
	
END_APP_DECLARATION()

DEFINE_APP(PointSpriteExample,"pointSprite")

namespace vtarga {
	
	unsigned char* load_targa(const char* filename, GLenum& format, int &width, int& height);
}

static inline float random_float()
{
	float res;
	static unsigned int seed = 0x13371337;
	seed *= 16807;
	
	unsigned int tmp = seed ^(seed>>4) ^(seed << 15);
	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;
	
	return (res - 1.0f);
}

void PointSpriteExample::Initialize(const char* title)
{
		base::Initialize(title);
		
		GLenum format;
		int width, height;
		unsigned char* data;
		
		glGenTextures(1,&m_sprite_tex);
		glBindTexture(GL_TEXTURE_2D,m_sprite_tex);
		
		//std::string filePath = Utils::instance()->getMediaPath() + "Media/textures/sprite2.tga";
		std::string filePath = Utils::instance()->getMediaPath() + "Media/textures/sprite.tga";
		data = vtarga::load_targa(filePath.c_str(),format,width,height);
		
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,format,GL_UNSIGNED_BYTE,data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		
		ShaderInfo shaders[] = {
			{GL_VERTEX_SHADER,"Media/Shaders/6/pointsprite.vs.glsl"},
			{GL_FRAGMENT_SHADER,"Media/Shaders/6/pointsprite2.fs.glsl"},
			{GL_NONE,NULL}
		};
		
		m_program = LoadShaders(shaders);
		
		m_model_matrix_loc = glGetUniformLocation(m_program,"model_matrix");
		m_projection_matrix_loc = glGetUniformLocation(m_program,"projection_matrix");
		
		static vec4* vertex_positions;
		
		glGenVertexArrays(1,&m_vao);
		glBindVertexArray(m_vao);
		
		glGenBuffers(1,&m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vec4)*POINT_COUNT,NULL,GL_STATIC_DRAW);
		
		vertex_positions = (vec4 *)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		for(int n = 0; n < POINT_COUNT; n++)
		{
			vertex_positions[n] = vec4(random_float()*2.0f - 1.0f, 
			random_float()*2.0f - 1.0f,
			random_float()*2.0f - 1.0f,
			random_float()*2.0f - 1.0f);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		
		glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
		
		glEnableVertexAttribArray(0);
		
		glClearColor(0.0f,0.0f,0.20f,1.0f);
		
}

void PointSpriteExample::Display(bool auto_redraw)
{
	float t = float(GetTickCount() & 0x1FFF) / float(0x1FFF);
	static float q = 0.0f;
    static const vec3 X(1.0f, 0.0f, 0.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    static const vec3 Z(0.0f, 0.0f, 1.0f);
	
	 
	
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(32.0f);
	// glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	//mat4 projection_matrix(frustum(-1.0f,1.0f,-m_aspect,m_aspect,1.0f,8.0f));
	//// glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	//glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	//

	//mat4 model_matrix = translate(0.0f,0.0f,-2.0f) * rotate( t * 360.0f, Y) * rotate( t * 720.0f, Z);
	//glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,model_matrix);

	mat4 projection_matrix(frustum(-1.0f, 1.0f, -m_aspect, m_aspect, 1.0f, 8.0f));
    glUniformMatrix4fv(m_projection_matrix_loc, 1, GL_FALSE, projection_matrix);
	
	 mat4 model_matrix = translate(0.0f, 0.0f, -2.0f) *
                   rotate(t * 360.0f, Y) * rotate(t * 720.0f, Z);
    glUniformMatrix4fv(m_model_matrix_loc, 1, GL_FALSE, model_matrix);

	glDrawArrays(GL_POINTS,0,POINT_COUNT);
	
	base::Display();
	
	

}

void PointSpriteExample::Finalize()
{
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_vbo);
 
	glDeleteTextures(1,&m_sprite_tex);
}	

void PointSpriteExample::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	
	m_aspect = float(height) / float(width);
}