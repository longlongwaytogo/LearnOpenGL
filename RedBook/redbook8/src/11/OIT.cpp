// OIT.cpp

#include <vapp.h>
#include <vutils.h>
#include <LoadShaders.h>
#include <vbm.h>
#include <vmath.h>
#include <stdio.h>
#include <iostream>

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048

BEGIN_APP_DECLARATION(OIT)
	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
	void InitShaders();
	void InitBuffers();
	
	
private:
	GLuint m_atomic_counter_buffer; // 原子计数器
	
	GLuint m_head_pointer_texture; // 
	GLuint m_head_pointer_clear_buffer;// 头指针初始化缓存对象
	
	GLuint m_linked_list_buffer; 
	GLuint m_linked_list_texture;

	GLuint m_list_build_program; // 
	GLuint m_resolve_program; //  
	
	GLuint m_quad_vao;
	GLuint m_quad_vbo;
	
	GLfloat m_aspect;
	
	GLuint m_projection_matrix_loc;
	GLuint m_model_matrix_loc;
	GLuint m_view_matrix_loc;
	GLuint m_aspect_loc;
	GLuint m_time_loc;
	
	VBObject m_object;
	
	float m_current_height;
	float m_current_width;
	
END_APP_DECLARATION()

DEFINE_APP(OIT, "OIT")

void OIT::InitShaders()
{
#if 1
	ShaderInfo render_shaders[] = {
		{GL_VERTEX_SHADER,"Media/Shaders/11/OIT.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/11/OIT.fs.glsl"},
		{GL_NONE,NULL} 
	};
	m_list_build_program = LoadShaders(render_shaders);
	m_model_matrix_loc = glGetUniformLocation(m_list_build_program,"model_matrix");
	m_view_matrix_loc = glGetUniformLocation(m_list_build_program,"view_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_list_build_program,"projection_matrix");

	ShaderInfo blit_shaders[] = {
		{GL_VERTEX_SHADER,"Media/shaders/11/OIT_blit.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/11/OIT_blit.fs.glsl"},
		//{GL_FRAGMENT_SHADER,"Media/shaders/11/resolve_lists.fs.glsl"},
		{GL_NONE,NULL}
	};
	m_resolve_program = LoadShaders(blit_shaders);
#else
	ShaderInfo render_shaders[] = {
		{GL_VERTEX_SHADER,"Media/Shaders/11/build_lists.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/11/build_lists.fs.glsl"},
		{GL_NONE,NULL} 
	};
	m_list_build_program = LoadShaders(render_shaders);
	m_model_matrix_loc = glGetUniformLocation(m_list_build_program,"model_matrix");
	m_view_matrix_loc = glGetUniformLocation(m_list_build_program,"view_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_list_build_program,"projection_matrix");

	ShaderInfo blit_shaders[] = {
		{GL_VERTEX_SHADER,"Media/shaders/11/resolve_lists.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/11/resolve_lists.fs.glsl"},
		{GL_NONE,NULL}
	};
	m_resolve_program = LoadShaders(blit_shaders);
#endif 
}

void OIT::InitBuffers()
{
	// pixel 
	GLuint* pData = NULL;
	GLsizei nTotoalPixels = MAX_FRAMEBUFFER_HEIGHT * MAX_FRAMEBUFFER_WIDTH;
	
	// 创建一个2D图像，用于存储逐像素链表中的head指针
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&m_head_pointer_texture);
	glBindTexture(GL_TEXTURE_2D,m_head_pointer_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,  // 2d
	GL_R32UI, // interformat
	MAX_FRAMEBUFFER_HEIGHT, // width
	MAX_FRAMEBUFFER_WIDTH, // height
	0, // border
	GL_RED_INTEGER, // 单通道
	GL_UNSIGNED_INT, // 数据类型
	NULL);
	glBindTexture(GL_TEXTURE_2D,0);
	
	// 将纹理绑定到0号图像变量
	glBindImageTexture(0,m_head_pointer_texture,0,GL_FALSE,0,GL_READ_WRITE,GL_R32UI);
	
	// 每帧开始的时候需要重新初始化m_head_pointer指针
	// 最简单的办法就是直接从PBO中拷贝 
	glGenBuffers(1,&m_head_pointer_clear_buffer);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,m_head_pointer_clear_buffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER,sizeof(GLuint)* nTotoalPixels,NULL,GL_STATIC_DRAW);// 不需要改动
	pData = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_WRITE_ONLY);
	memset(pData,0x00,sizeof(GLuint)* nTotoalPixels);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
	
	// create Unit atomic conter buffer
	glGenBuffers(1,&m_atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER,m_atomic_counter_buffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER,sizeof(GLuint),NULL,GL_DYNAMIC_COPY);
	GLuint* atom_counters = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER,GL_WRITE_ONLY);
	atom_counters[0] = 0;
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER,0,m_atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER,0);
	
	// 创建一个较大的一维缓存来存储片元数据
	// 为屏幕上每个像素都分配一个可以存储3个片元的空间
	glGenBuffers(1,&m_linked_list_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER,m_linked_list_buffer);
	glBufferData(GL_TEXTURE_BUFFER,sizeof(vmath::vec4)* 3 * nTotoalPixels,NULL,GL_DYNAMIC_COPY);

	// create TBO
	glGenTextures(1,&m_linked_list_texture);
	glBindTexture(GL_TEXTURE_BUFFER, m_linked_list_texture);
	glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32UI,m_linked_list_buffer);
	//glBindTexture(GL_TEXTURE_BUFFER,0);
	//glBindBuffer(GL_TEXTURE_BUFFER,0);
	// 将buffer绑定到1号图像变量
	// glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
	glBindImageTexture(1,m_linked_list_texture,0, GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32UI);
	
	/*
	四边形占屏幕大小的二分之一，所以程序运行后，可以看到一半是第一次绘制结果，一半是第二次绘制结果。
	*/
	  static const GLfloat quad_vertices[] =
    {
#if 1
		// 绘制屏幕二分之一
         -0.0f, -1.0f,
         1.0f, -1.0f,
        -0.0f,  1.0f,
         1.0f,  1.0f, 
#else
		// 绘制整个屏幕
		  -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
#endif 
    };


	glGenVertexArrays(1,&m_quad_vao);
	glGenBuffers(1,&m_quad_vbo);
	glBindVertexArray(m_quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER,m_quad_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quad_vertices),quad_vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);
	
	glClearDepth(1.0f);
	
	glBindVertexArray(0);
}
void OIT::Initialize(const char* title)
{
	base::Initialize(title);
	
	InitShaders();
	InitBuffers();
	
	std::string filePath = Utils::instance()->getMediaPath() + "Media/unit_pipe.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
}

void OIT::Display(bool auto_redraw)
{
	float t = float(GetTickCount() & 0xFFFF)/0x3FFF;
	static vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	GLuint *pData = NULL;
#if 1
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER,m_atomic_counter_buffer);
	//glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER,0,m_atomic_counter_buffer);
	//pData = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER,GL_WRITE_ONLY);
	//pData[0] = 0;
	//glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_atomic_counter_buffer);
    pData = (GLuint *)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
    pData[0] = 0;
    glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

#else
	// 计数值重新设置为 0 
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER,0,m_atomic_counter_buffer);
	const GLuint zero = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER,0,sizeof(zero),&zero);
#endif 
	
	// clear head-pointer image 
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,m_head_pointer_clear_buffer);
	glBindTexture(GL_TEXTURE_2D,m_head_pointer_texture);
	// GLAPI void GLAPIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	glTexSubImage2D(GL_TEXTURE_2D,0,
	0,0, // offset
	m_current_width,
	m_current_height,
	GL_RED_INTEGER,
	GL_UNSIGNED_INT,
	NULL); // 从pbo中传递数据到texture中
	glBindTexture(GL_TEXTURE_2D,0);
	
	// 绑定图像单元
	glBindImageTexture(0,
						m_head_pointer_texture,
						0, // level
						GL_FALSE,0, // 无层
						GL_READ_WRITE,
						GL_R32UI); // 每个像素都是32 unsigned int 						

    // Bind output image for read-write
	glBindImageTexture(1,m_linked_list_texture,0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32UI);
	
	  vmath::mat4 model_matrix = vmath::translate(0.0f, 0.0f, -20.0f) *
                               vmath::rotate(t * 360.0f, 0.0f, 0.0f, 1.0f) *
                               vmath::rotate(t * 435.0f, 0.0f, 1.0f, 0.0f) *
                               vmath::rotate(t * 275.0f, 1.0f, 0.0f, 0.0f);
    vmath::mat4 view_matrix = vmath::mat4::identity();
    vmath::mat4 projection_matrix = vmath::frustum(-1.0f, 1.0f, m_aspect, -m_aspect, 1.0f, 40.f);
	
	glUseProgram(m_list_build_program);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,model_matrix);
	glUniformMatrix4fv(m_view_matrix_loc,1,GL_FALSE,view_matrix);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	m_object.Render(0,8 * 8 * 8);

	glDisable(GL_BLEND);
		 
	glUseProgram(m_resolve_program);
	glBindVertexArray(m_quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	base::Display(auto_redraw);
}

void OIT::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(1,&m_quad_vao);
	glDeleteBuffers(1,&m_quad_vbo);
}

void OIT::Reshape(int width, int height)
{
	m_aspect  = GLfloat(width)/height;
	glViewport(0,0,width,height);

	m_current_width = width;
	m_current_height = height;
}

