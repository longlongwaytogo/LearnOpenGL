// ch03-3-Instance_1

#include <vapp.h>
#include <vutils.h>
#include <vmath.h>
#include "vbm.h"
#include <loadShaders.h>
#include <stdio.h>

using namespace vmath;

#define INSTANCE_COUNT 100

BEGIN_APP_DECLARATION(Instance_2)

// override functions from base class 
virtual void Initialize(const char* title);
virtual void Display(bool auto_redraw);
virtual void Finalize();
virtual void Reshape(int width, int height);

private:

	float  m_aspect;
	GLuint m_prog;
	
	GLuint m_color_texture;
	GLuint m_model_texture;
	GLuint m_color_tbo;
	GLuint m_model_tbo; 
	
	GLint m_color_tbo_loc;
	GLint m_model_tbo_loc;
	GLint m_view_matrix_loc;
    GLint m_projection_matrix_loc;
	
	VBObject 	m_object;
	
END_APP_DECLARATION()

DEFINE_APP(Instance_2 ,"ch03-4-Instance_2")

void Instance_2::Initialize(const char* title)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	base::Initialize(title);
	 
	 // init shader
	static ShaderInfo shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/3/ch03-4-Instance-2.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/3/ch03-4-Instance-2.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_prog = LoadShaders(shader_info);
	glUseProgram(m_prog);
	
	 m_view_matrix_loc = glGetUniformLocation(m_prog,"view_matrix");
	// //m_model_matrix_loc = glGetUniformLocation(m_prog,"model_matrix");
	 m_projection_matrix_loc = glGetUniformLocation(m_prog,"projection_matrix");
	 //Vertex Attrib Array index
	m_color_tbo_loc = glGetUniformLocation(m_prog, "color_tbo");
	m_model_tbo_loc = glGetUniformLocation(m_prog, "model_tbo");

	/*
	为了使用第二个纹理（以及第一个），我们必须改变一点渲染流程， 先绑定两个纹理到对应的纹理单元，
	然后定义哪个uniform采样器对应哪个纹理单元： 注意，我们使用glform1i设置uniform采样器的位置值，
	或者说纹理单元。 通过glUniform1i的设置，我们保证每个uniform采样器对应着正确的纹理单元。
	*/
	glUniform1i(m_color_tbo_loc, 0);
	glUniform1i(m_model_tbo_loc, 1);

	 
	// init object
	std::string path = Utils::instance()->getMediaPath() + "Media\\armadillo_low.vbm";
    m_object.LoadFromVBM(path.c_str(), 0, 1, 2);
	m_object.BindVertexArray();
	
	// colors init
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
	
	// use TBO  
	// Notice the set color_tb0 to gl_texture_buffer layer 0, your should use texture to change it
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&m_color_texture);
	glBindTexture(GL_TEXTURE_BUFFER,m_color_texture);
	
	// 生成TBO,并绑定纹理
	glGenBuffers(1,&m_color_tbo);
	glBindBuffer(GL_TEXTURE_BUFFER,m_color_tbo);
	glBufferData(GL_TEXTURE_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW); 
	glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32F,m_color_tbo);
	
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1,&m_model_texture);
	glBindTexture(GL_TEXTURE_BUFFER,m_model_texture);
	 
	glGenBuffers(1,&m_model_tbo);
	glBindBuffer(GL_TEXTURE_BUFFER,m_model_tbo);
	glBufferData(GL_TEXTURE_BUFFER,INSTANCE_COUNT * sizeof(mat4),NULL,GL_DYNAMIC_DRAW);
	glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32F,m_model_tbo);
	
	
	  // Done (unbind the object's VAO)
   // glBindVertexArray(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Instance_2::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	m_aspect = float(height)/float(width);
}

void Instance_2::Display(bool auto_redraw)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	
	
	// Active instancing program
	glUseProgram(m_prog);
	
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
    static float q = 0.0f;
    static const vec3 X(1.0f, 0.0f, 0.0f);
    static const vec3 Y(0.0f, 1.0f, 0.0f);
    static const vec3 Z(0.0f, 0.0f, 1.0f);
	
	mat4 matrices[INSTANCE_COUNT];
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
	
	glBindBuffer(GL_TEXTURE_BUFFER,m_model_tbo);
	glBufferData(GL_TEXTURE_BUFFER,sizeof(matrices),matrices,GL_DYNAMIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER,0);
	
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
 
	glUseProgram(0);
	
	base::Display();
}
void Instance_2::Finalize()
{
	glUseProgram(0);
	glDeleteBuffers(1, &m_model_tbo);
    glDeleteBuffers(1, &m_color_tbo);
	
}
 