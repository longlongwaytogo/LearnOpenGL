// ch07-03-ShadowMap
///////////////////////////////////////////////////////////////////////\
//
// ShadowMap.cpp
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
#include <vmath.h>
#include <vbm.h>
#include <Camera.h>
#include <LoadShaders.h>
#include <stdio.h>

using namespace vmath;

#define FRUSTUM_DEPTH       800.0f
#define DEPTH_TEXTURE_SIZE  2048

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


struct Base_uniform_loc
	{
		GLuint model;
		GLuint view;
		GLuint proj;
		GLuint shadow_matrix;
		GLuint lightPos;
		GLuint mat_ambient;
		GLuint mat_diffuse;
		GLuint mat_specular;
		GLuint mat_specular_power;
	};


BEGIN_APP_DECLARATION(ShadowMap)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);
	virtual void Keyboard(unsigned char key, int x, int y );
	virtual void MouseEvent(int button,int state,int x,int y);

	virtual void MouseDownMotion(int x,int y);

	void DrawScene(bool DepthOnly);

	ShadowMap();
    // Member variables
    float  m_aspect;
    GLuint m_program;
	GLuint m_shadow_program;
	GLuint m_depth_texture;
	GLuint m_fbo;

    Base_uniform_loc m_uniform_loc;
	GLuint m_shadow_mvp_loc;
	VBObject m_object;

	GLuint m_vao;
	GLuint m_vbo;
 
	bool	 m_bLeftMouseDown;
	int		 m_mouseX;
	int		 m_mouseY;

    int     m_currentWidth;
    int     m_currentHeight;
	Camera m_camera;

END_APP_DECLARATION()

#define TEST 0
DEFINE_APP(ShadowMap, "ShadowMap")

ShadowMap::ShadowMap():m_bLeftMouseDown(false),m_mouseX(0),m_mouseY(0)
{

}
void ShadowMap::Initialize(const char * title)
{
	base::Initialize(title);

	// Upload geometry for the ground plane
	static const float ground_vertices[] =
	{
		-500.0f, -50.0f, -500.0f, 1.0f,
		-500.0f, -50.0f, 500.0f, 1.0f,
		500.0f, -50.0f, 500.0f, 1.0f,
		500.0f, -50.0f, -500.0f, 1.0f,
	};

	static const float ground_normals[] =
	{
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1,&m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(ground_vertices) + sizeof(ground_normals),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(ground_vertices),ground_vertices);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(ground_vertices),sizeof(ground_normals),ground_normals);
	
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(const GLvoid*)(NULL + sizeof(ground_vertices)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	ShaderInfo shaders_base [] = {
		{GL_VERTEX_SHADER, "Media/Shaders/7/base.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/7/base.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	ShaderInfo shaders_shadow [] = {
		{GL_VERTEX_SHADER, "Media/Shaders/7/shadow_object.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/7/shadow_object.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_shadow_program = LoadShaders(shaders_shadow);
	
	m_shadow_mvp_loc = glGetUniformLocation(m_shadow_program,"mvp");
	m_program = LoadShaders(shaders_base);

	ShowShaderInfo(m_program);
	//set the depth texture to unit 0 
	glUseProgram(m_program);
	GLint depth_texture = glGetUniformLocation(m_program, "depth_texture");
	int code = glGetError();
	glUniform1i(depth_texture, 0);

	m_uniform_loc.proj = glGetUniformLocation(m_program,"projection_matrix");
	m_uniform_loc.model = glGetUniformLocation(m_program,"model_matrix");
	m_uniform_loc.view = glGetUniformLocation(m_program,"view_matrix");
	m_uniform_loc.shadow_matrix = glGetUniformLocation(m_program,"shadow_matrix");

	m_uniform_loc.lightPos = glGetUniformLocation(m_program,"lightPos");
	m_uniform_loc.mat_ambient = glGetUniformLocation(m_program,"mat_ambient");
	m_uniform_loc.mat_diffuse = glGetUniformLocation(m_program,"mat_diffuse");
	m_uniform_loc.mat_specular = glGetUniformLocation(m_program,"mat_specular");
	m_uniform_loc.mat_specular_power = glGetUniformLocation(m_program,"mat_specular_power");

	// init texture
	glGenTextures(1,&m_depth_texture);
	glBindTexture(GL_TEXTURE_2D,m_depth_texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,DEPTH_TEXTURE_SIZE,DEPTH_TEXTURE_SIZE,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D,0);

	// int depth fbo
	glGenFramebuffers(1,&m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,m_depth_texture,0);

	// we don't need color buffer;
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER,0);



	std::string filePath = Utils::instance()->getMediaPath() + "Media/armadillo_low.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
}

void ShadowMap::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_vbo);
	glDeleteFramebuffers(1,&m_fbo);
}



void ShadowMap::Display(bool auto_redraw)
{
	static const unsigned int start_time = GetTickCount();
	float t = float(GetTickCount() & 0xFFFF) / float(0xFFFF);

	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	vmath::vec3 light_position = vmath::vec3(sinf(t * 6.0f * 3.141592f) * 300.0f, 200.0f, cosf(t * 4.0f * 3.141592f) * 100.0f + 250.0f);

	glm::mat4 view_matrix = m_camera.GetViewMatrix();
 	glEnable(GL_CULL_FACE);
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


	/*vmath::mat4 model_mat = vmath::translate(vmath::vec3(0,0,-140));
	vmath::mat4 view_mat = toVmathMat4(view_matrix);
	vmath::mat4 projection_mat = vmath::perspective(45.0f,1.0f/m_aspect,0.30f,1000.0f);*/
	// 场景绘制需要的矩阵
	vmath::mat4 model_mat = vmath::rotate(t* 720.0f,Y);
	vmath::mat4 view_mat = vmath::translate(0.0f,0.0f,-300.0f);
	vmath::mat4 projection_mat = vmath::frustum(-1.0f, 1.0f, -m_aspect, m_aspect, 1.0f, FRUSTUM_DEPTH);

    view_mat = toVmathMat4(view_matrix);

	const vmath::mat4 scale_bias_matrix = vmath::mat4(vmath::vec4(0.5f,0.0f,0.0f,0.0f),
				vmath::vec4(0.0f, 0.5f, 0.0f, 0.0f),
				vmath::vec4(0.0f, 0.0f, 0.5f, 0.0f),
				vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f)
				);

	// 阴影图需要的矩阵
	vmath::mat4 light_view_mat = vmath::lookat(light_position,vmath::vec3(0.0f),Y);
	vmath::mat4 light_projection_mat = vmath::frustum(-1.0f,1.0f,-1.0f,1.0f,1.0f,FRUSTUM_DEPTH);

	// render scene from light position
	glUseProgram(m_shadow_program);
	glUniformMatrix4fv(m_shadow_mvp_loc,1,GL_FALSE,light_projection_mat * light_view_mat * model_mat);

	// bind fbo
	glBindFramebuffer(GL_FRAMEBUFFER,m_fbo);
	glViewport(0,0,DEPTH_TEXTURE_SIZE,DEPTH_TEXTURE_SIZE);

	// clear
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

    

	// enable polygon offset to resolve depth-fighting isuse
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f,4.0f);

	// 绘制阴影
	DrawScene(true);

	glDisable(GL_POLYGON_OFFSET_FILL);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, m_currentWidth, m_currentHeight);
	glUseProgram(m_program);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	

	glUniformMatrix4fv(m_uniform_loc.model,1,GL_FALSE,model_mat);
	glUniformMatrix4fv(m_uniform_loc.view,1,GL_FALSE,view_mat);
	glUniformMatrix4fv(m_uniform_loc.proj,1,GL_FALSE,projection_mat);
    glUniformMatrix4fv(m_uniform_loc.shadow_matrix,1,GL_FALSE,scale_bias_matrix * light_projection_mat * light_view_mat);
    glUniform3fv(m_uniform_loc.lightPos, 1, light_position);
	glBindTexture(GL_TEXTURE_2D,m_depth_texture);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	 DrawScene(false);

	base::Display();
}

	
void ShadowMap::DrawScene(bool depthOnly)
{
    if(!depthOnly)
    {
        // set object material;
        glUniform3fv(m_uniform_loc.mat_ambient,1,vmath::vec3(0.1f,0.0f,0.2f));
        glUniform3fv(m_uniform_loc.mat_diffuse,1,vmath::vec3(0.3f,0.2f,0.8f));
        glUniform3fv(m_uniform_loc.mat_specular,1,vmath::vec3(1.0f,1.0f,1.0f));
        glUniform1f(m_uniform_loc.mat_specular_power,25.0f);
    }

    // draw object
    m_object.Render();

    // set ground material
    if(!depthOnly)
    {
        glUniform3fv(m_uniform_loc.mat_ambient,1,vmath::vec3(0.1f, 0.1f, 0.1f));
        glUniform3fv(m_uniform_loc.mat_diffuse,1,vmath::vec3(0.1f, 0.5f, 0.1f));
        glUniform3fv(m_uniform_loc.mat_specular,1,vmath::vec3(0.1f, 0.1f, 0.1f));
        glUniform1f(m_uniform_loc.mat_specular_power,3.0f);
    }

    // draw the ground
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glBindVertexArray(0);
}

void ShadowMap::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
    
    m_currentWidth = width;
    m_currentHeight = height;
	m_aspect = float(height) / float(width);
}


  void ShadowMap::Keyboard(unsigned char key, int x, int y )
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

  void ShadowMap::MouseEvent(int button,int state,int x,int y)
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

  void ShadowMap::MouseDownMotion(int x, int y)
  {
	  int dx =  m_mouseX - x;
	  int dy =  m_mouseY - y;
	  if(m_bLeftMouseDown)
				m_camera.ProcessMouseMovement(float(dx),float(dy));
  }
