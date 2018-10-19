// ch07-01-ShaderLight
///////////////////////////////////////////////////////////////////////\
//
// ShaderLight.cpp
//
/////////////////////////////////////////////////////////////////

//#define USE_GL3W

//#ifdef USSE_GL3W
//#include <GL3/gl3w.h> 
//#else
//#include <GL/glew.h>
//#endif 

#include <vapp.h>
#include <vutils.h>
#include <vermilion.h>
#include <vmath.h>
#include <vbm.h>
#include <Camera.h>
#include <LoadShaders.h>
#include <stdio.h>

vmath::mat4 toVmathMat4(glm::mat4& refMat)
{
	vmath::mat4 mat; 
	for(int i = 0 ; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
			mat[i][j] = refMat[i][j];
	}
	return mat;
}


BEGIN_APP_DECLARATION(LightShader)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
	virtual void Keyboard(unsigned char key, int x, int y );
	virtual void MouseEvent(int button,int state,int x,int y);

	virtual void MouseDownMotion(int x,int y);

	LightShader();
    // Member variables
    float  m_aspect;
    GLuint m_program;
 
 
	GLuint m_object_mvp_loc;
	GLuint m_object_mv_loc;

	GLuint m_ambient_loc;	
	GLuint m_light_color_loc;
	GLuint m_light_position_loc;
	GLuint m_shininess_loc;
	GLuint m_strength_loc;


	VBObject m_object;

	bool	 m_bLeftMouseDown;
	int		 m_mouseX;
	int		 m_mouseY;
	Camera m_camera;

END_APP_DECLARATION()

#define TEST 0
DEFINE_APP(LightShader, "LightShader")

LightShader::LightShader():m_bLeftMouseDown(false),m_mouseX(0),m_mouseY(0)
{

}
void LightShader::Initialize(const char * title)
{
	base::Initialize(title);

	ShaderInfo shaders [] = {
		{GL_VERTEX_SHADER, "Media/Shaders/7/light_shader-1.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/7/light_shader-1.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_program = LoadShaders(shaders);
 
	m_object_mvp_loc = glGetUniformLocation(m_program,"mvp_matrix");
	m_object_mv_loc = glGetUniformLocation(m_program,"normal_matrix");
	
	 
	std::string filePath = Utils::instance()->getMediaPath() + "Media/unit_torus.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
}

void LightShader::Finalize()
{
	glUseProgram(0);
	 
}

void LightShader::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(height) / float(width);
}

void LightShader::Display(bool auto_redraw)
{
	static const unsigned int start_time = GetTickCount();
	float t = float(GetTickCount()-start_time)/float(0x3FFF);
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	glm::mat4 view_matrix = m_camera.GetViewMatrix();
 
	glClearColor(0.0f, 0.0f ,0.0f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	glUseProgram(m_program);

	
	vmath::mat4 model_mat = vmath::translate(vmath::vec3(0,0,-4));
	vmath::mat4 model_view = toVmathMat4(view_matrix)  * model_mat;
	glUniformMatrix4fv(m_object_mv_loc,1,GL_FALSE,model_view);
	vmath::mat4 mvp_matrix = vmath::perspective(45.0f,1.0f/m_aspect,0.30f,100.0f) * model_view;
	glUniformMatrix4fv(m_object_mvp_loc,1,GL_FALSE,mvp_matrix);
	 
    glClear(GL_DEPTH_BUFFER_BIT);
	
	m_object.Render();

	base::Display();
}

	
  void LightShader::Keyboard(unsigned char key, int x, int y )
	{
		Camera_Movement direction = FORWARD ;
		 if(key == 'W' || key == 'w')
		 {
			 // up 
			 direction = FORWARD;
		 }
		 else if( key == 'S' || key == 's')
		 {
			 direction = BACKWARD;
		 }
		 else if( key == 'A' || key == 'a')
		 {
			 direction = LEFT;
		 }
		 else if(key == 'D' || key == 'd')
		 {
			 direction = RIGHT;
		 }
		 m_camera.Processkeyboard(direction,0.10);
	}

  void LightShader::MouseEvent(int button,int state,int x,int y)
	{
		 
		if(button == GLUT_LEFT_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
				m_bLeftMouseDown = true;
				m_mouseX = x;
				m_mouseY = y;
			}
			else if(state == GLUT_UP)
			{
				m_bLeftMouseDown = false;
			}

			
		}
		else if(button == GLUT_MIDDLE_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
			}
			else if(state == GLUT_UP)
			{
			}
		}
		else if(button == GLUT_RIGHT_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
			}
			else if(state == GLUT_UP)
			{
			}
		}

		
	}

  void LightShader::MouseDownMotion(int x, int y)
  {
	  int dx =  m_mouseX - x;
	  int dy =  m_mouseY - y;
	  if(m_bLeftMouseDown)
				m_camera.ProcessMouseMovement(float(dx),float(dy));
  }
