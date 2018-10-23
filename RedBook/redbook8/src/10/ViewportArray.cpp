// ViewportArray.cpp

#include <vapp.h>
#include <vutils.h>
#include <LoadShaders.h>
#include <vbm.h>
#include <vmath.h>
#include <stdio.h>

BEGIN_APP_DECLARATION(ViewportArray)
	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
private:
	GLuint m_program;
	GLfloat m_aspect;
	
	GLuint m_projection_matrix_loc;
	GLuint m_model_view_matrix_loc;
	
	VBObject m_object;
	
	
END_APP_DECLARATION()


DEFINE_APP(ViewportArray, "viewportArray")


void ViewportArray::Initialize(const char* title)
{
	base::Initialize(title);
	
	ShaderInfo shaders [] = {
		{GL_VERTEX_SHADER, "Media/Shaders/10/ViewportArray.vs.glsl"},
		{GL_GEOMETRY_SHADER, "Media/Shaders/10/ViewportArray.gs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/10/ViewportArray.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_program = LoadShaders(shaders);
 
	m_projection_matrix_loc = glGetUniformLocation(m_program,"projection_matrix");
	m_model_view_matrix_loc = glGetUniformLocation(m_program,"model_view_matrix");
	
	std::string filePath = Utils::instance()->getMediaPath() + "Media/ninja.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
	
	
}


void ViewportArray::Display(bool auto_redraw)
{
	float t = float(GetTickCount() & 0xFFF)/0xFFF;
	static vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	vmath::mat4 model_view_matrix[4];
	
	glClearColor( 0.3f, 0.1f, 0.2f, 1.0f );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for(int i = 0; i < 4; ++i)
	{
		model_view_matrix[i] = vmath::mat4(
			vmath::translate(0.0f, 0.0f, 100.0f * sin(6.28318531f * t + i) - 230.0f) *
			vmath::rotate(360.0f * t * float(i + 1), X)*
			vmath::rotate(360.0f * t * float(i + 2), Y)*
			vmath::rotate(360.0f * t * float(i + 3), Z)*
			vmath::translate(0.0f, -80.0f, 0.0f));
	}
	
	glUseProgram(m_program);
	
	vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, m_aspect, -m_aspect, 1.0f, 1000.0f));
	glUniformMatrix4fv(m_model_view_matrix_loc,4,GL_FALSE,model_view_matrix[0]);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	
	/* CULL FACE 设置说明：
	默认程序没有开启CullFace设置，需要手动设置开启。开启CullFace后，需要根据顺时针或逆时针确定模型的正面或背面，
	同时，根据设置需要剔除正面或背面进行剔除。本程序使用胡VMB模型，是按照顺时针方向为正面建模的，如果需要剔除它的背面，可以通过两种方式
	1. 重新设置 glFrontFace(GL_CW)
	或
	2. glCullFace(GL_FRONT)
	*/
	GLint face_mode = 0;
	glGetIntegerv(GL_CULL_FACE_MODE,&face_mode);
    glEnable(GL_CULL_FACE); // vbm 模型的正面，是顺时针方向
   // glCullFace(GL_BACK);
	glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	m_object.Render();
	
	base::Display(auto_redraw);
	
}


void ViewportArray::Finalize()
{
	glUseProgram(0);
	
}

void ViewportArray::Reshape(int width, int height)
{
	m_aspect  = GLfloat(width)/height;
	
	glViewport(0,0,width,height);
	
	float w = float(width)/2;
	float h = float(height)/2;
#if 1	
	glViewportIndexedf(0, 0.0f, 0.0f, w, h);
	glViewportIndexedf(1,    w, 0.0f, w, h);
	glViewportIndexedf(2, 0.0f,    h, w, h);
	glViewportIndexedf(3,    w,    h, w, h);
#else
	glViewprotArray()
#endif 
}

