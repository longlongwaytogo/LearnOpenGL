// ch05-tranformFeedback.cpp

#include <vapp.h>
#include <vutils.h>
#include <vmath.h>
#include "vbm.h"
#include <loadShaders.h>
#include <stdio.h>

using namespace vmath;

const int point_count = 5000;
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

static vmath::vec3 random_vector(float minmag = 0.0f, float maxmag = 1.0f)
{
	vmath::vec3 randomvec(random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f);
	randomvec = normalize(randomvec);
	randomvec *= (random_float() * (maxmag - minmag) + minmag);

	return randomvec;
}


BEGIN_APP_DECLARATION(TransformFeedback)

// override functions from base class 
virtual void Initialize(const char* title);
virtual void Display(bool auto_redraw);
virtual void Finalize();
virtual void Reshape(int width, int height);

private:

	float  m_aspect;
	GLuint m_baseProg;   // 用于绘制物体
	GLuint m_updateProg; // 用于绘制更新的粒子
	GLuint m_VAO[2];     // 用于关联粒子系统的VBO
	GLuint m_VBO[2];     // 保存粒子坐标、速度信息
	//GLuint m_xfb; // transformFeedback 对象
	GLuint m_feedback_VAO;  // 用于关联 给feedback使用胡tbo
	GLuint m_geometry_tbo;
	GLuint m_geometry_tex;
	
	 
	GLint m_base_model_matrix_loc;
    GLint m_base_projection_matrix_loc;
	
	GLint m_update_model_matrix_loc;
    GLint m_update_projection_matrix_loc;
	
	GLint m_time_step_loc;
	GLint m_triangle_count_loc;
	
	VBObject 	m_object;
	
END_APP_DECLARATION()

DEFINE_APP(TransformFeedback ,"ch05-transformFeedback")

void TransformFeedback::Initialize(const char* title)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	base::Initialize(title);
	 
	 // init shader
	static ShaderInfo base_shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/5/base.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/5/base.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	static ShaderInfo update_shader_info[] = 
	{
		{GL_VERTEX_SHADER,"Media/Shaders/5/update.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/5/update.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_updateProg = LoadShaders(update_shader_info);
	glUseProgram(m_updateProg);

 		
	m_update_model_matrix_loc = glGetUniformLocation(m_updateProg,"model_matrix");
	m_update_projection_matrix_loc = glGetUniformLocation(m_updateProg,"projection_matrix");

	m_triangle_count_loc = glGetUniformLocation(m_updateProg,"triangle_count");
	m_time_step_loc = glGetUniformLocation(m_updateProg,"time_step");

	static const char* varyings[] = 
	{
		"position_out", "velocity_out"
	};
	/*glTransformFeedbackVaryings(GLuint program,
									GLsizei count,
									const GLchar *const* varyings,
									GLenum bufferMode)
	*/
	// 使用交错模式的缓存
	glTransformFeedbackVaryings(m_updateProg,2,varyings,GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_updateProg); // 重新link着色器
	//glUseProgram(m_updateProg); 
	
	m_baseProg = LoadShaders(base_shader_info);
	glUseProgram(m_baseProg);

	m_base_model_matrix_loc = glGetUniformLocation(m_baseProg,"model_matrix");
	m_base_projection_matrix_loc = glGetUniformLocation(m_baseProg,"projection_matrix");
	
	static const char * varyings2[] =
	{
		"world_space_position"
	};
	
	glTransformFeedbackVaryings(m_baseProg,1,varyings2,GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_baseProg);
	//glUseProgram(m_baseProg);
	
	glGenVertexArrays(2,m_VAO);
	glGenBuffers(2,m_VBO);
	
	//此处没有调用glGenTransformFeedbacks和glBindTransformFeedback函数，因为系统存在一个默认的feedback，可以直接使用	
	for(int i = 0 ; i < 2; ++i)
	{
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER,m_VBO[i]);
		/*
		glBindBuffer(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
		*/
		glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
					point_count * (sizeof(vec4) + sizeof(vec3)),
					NULL,
					GL_DYNAMIC_COPY);
		
		// 初始化第一个buffer
		if(i == 0)
		{
			struct buffer_t
			{
				vec4 position;
				vec3 velocity;
			} *buffer = (buffer_t*)glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER,GL_WRITE_ONLY);
			
			for(int j = 0; j < point_count; ++j)
			{
				buffer[j].velocity = random_vector();
				buffer[j].position = vec4(buffer[j].velocity + vec3(-0.5f, 40.0f, 0.0f), 1.0f);
				buffer[j].velocity = vec3(buffer[j].velocity[0], buffer[j].velocity[1] * 0.3f, buffer[j].velocity[2] * 0.3f);
			}
			
			glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
		}
		
		glBindVertexArray(m_VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER,m_VBO[i]);
		/*
		glVertexAttribPointer(GLuint index, 
		GLint size, 
		GLenum type,
		GLboolean normalized,
		GLsizei stride,
		const void* pointer);
		*/
		glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,
		sizeof(vec4) + sizeof(vec3),0);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,
		sizeof(vec4) + sizeof(vec3),(GLvoid*)sizeof(vec4));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	
	// 创建tbo，用于保存三角形顶点数据，此处的tbo被同时绑定到两个对象中，
	// 1. tbo被绑定到feedback中，用于捕获模型绘制的三角形数据
	// 2. tbo被绑定到纹理对象，用于例子系统中获取三角形数据
	glGenBuffers(1,&m_geometry_tbo);
	glGenTextures(1,&m_geometry_tex);
	glBindBuffer(GL_TEXTURE_BUFFER,m_geometry_tbo);
	glBufferData(GL_TEXTURE_BUFFER,1024 * 1024 * sizeof(vec4),NULL,GL_DYNAMIC_COPY);
	glBindTexture(GL_TEXTURE_BUFFER,m_geometry_tex);
	/*
	glTexBuffer(GLenum target, GLenum internalFormat, GLuint buffer);
	*/
	// 将缓存区关联到纹理对象上
	glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32F,m_geometry_tbo);
	
	//  
	glGenVertexArrays(1, &m_feedback_VAO);
	glBindVertexArray(m_feedback_VAO);
	glBindBuffer(GL_ARRAY_BUFFER,m_geometry_tbo);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	
	// init object
	std::string path = Utils::instance()->getMediaPath() + "Media\\armadillo_low.vbm";
    m_object.LoadFromVBM(path.c_str(), 0, 1, 2);
	//m_object.BindVertexArray();
	
   // glBindVertexArray(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TransformFeedback::Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	m_aspect = float(height)/float(width);
}

void TransformFeedback::Display(bool auto_redraw)
{
	static int frame_count = 0;
	float t = float(GetTickCount() & 0x3FFFF) / float(0x3FFFF);
	static float q = 0.0f;
	static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, -m_aspect, m_aspect, 1.0f, 5000.0f) * vmath::translate(0.0f, 0.0f, -100.0f)); // project and view matrix 
	vmath::mat4 model_matrix(vmath::scale(0.3f) *
		vmath::rotate(t * 360.0f, 0.0f, 1.0f, 0.0f) *
		vmath::rotate(t * 360.0f * 3.0f, 0.0f, 0.0f, 1.0f));
		
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Setup
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	
	
	// Active instancing program
	glUseProgram(m_baseProg);
	
	glUniformMatrix4fv(m_base_model_matrix_loc, 1, GL_FALSE, model_matrix);
	glUniformMatrix4fv(m_base_projection_matrix_loc,1,GL_FALSE, projection_matrix);
	
	glBindVertexArray(m_feedback_VAO);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,m_geometry_tbo); 
	
	// 捕获三角形顶点数据
	glBeginTransformFeedback(GL_TRIANGLES);
	m_object.Render();
	glEndTransformFeedback();
	
	glUseProgram(m_updateProg);
	model_matrix = vmath::mat4::identity();
	glUniformMatrix4fv(m_update_model_matrix_loc,1,GL_FALSE,model_matrix);
	glUniformMatrix4fv(m_update_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	glUniform1i(m_triangle_count_loc,m_object.GetVertexCount()/3);
	
	// 时间控制
	if( t > q)
	{
		glUniform1f(m_time_step_loc,(t-q)*2000.0f);
		// printf("time:%f\n",(t-q)*2000.0f);
	}
	
	q = t;
	
	// 每隔一帧，交换VBO，使用两个VBO，一个用于当前绘制的数据，一个用于feedback，捕获更新后的三角形顶点和速度信息
	if((frame_count & 1) != 0)
	{
		glBindVertexArray(m_VAO[1]);  // 启用用于绘制的VBO，即m_VBO[1];
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,m_VBO[0]); // 使用m_VBO[0]捕获粒子系统的当前位置和速度
		// printf("frame:%d\n",frame_count);
	}
	else
	{
		glBindVertexArray(m_VAO[0]);// 启用用于绘制的VBO，即m_VBO[0];
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,0,m_VBO[1]);
	}
	
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS,0,min(point_count,(frame_count >> 3)));
	glEndTransformFeedback();
	
	glBindVertexArray(0);
	glUseProgram(0);
	
	frame_count++;
	
	base::Display();
}
void TransformFeedback::Finalize()
{
	glUseProgram(0);
	glDeleteBuffers(2, m_VBO);
	glDeleteBuffers(1,&m_geometry_tbo);
    glDeleteVertexArrays(2, m_VAO);
	glDeleteVertexArrays(1,&m_feedback_VAO);
}
 