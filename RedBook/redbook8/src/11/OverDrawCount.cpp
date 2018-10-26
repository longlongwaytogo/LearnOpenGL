// OverDrawCount.cpp

#include <vapp.h>
#include <vutils.h>
#include <LoadShaders.h>
#include <vbm.h>
#include <vmath.h>
#include <stdio.h>
#include <iostream>

#define MAX_FRAMEBUFFER_WIDTH 1024
#define MAX_FRAMEBUFFER_HEIGHT 1024

BEGIN_APP_DECLARATION(OverDrawCount)
	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
	void InitShaders();
	void InitBuffers();
	
	
private:
	// Color palette buffer texture  TBO 
	GLuint m_image_palette_buffer; // 纹理缓冲区对象
	GLuint m_image_palette_texture; //纹理 
	
	GLuint m_overdraw_count_buffer;
	GLuint m_overdraw_count_clear_buffer;
	
	// 输出纹理，渲染第一阶段，在shader中写入数据，渲染第二阶段，在shader中读取数据
	GLuint m_output_texture;   
	// Output iamge and PBO for clear it 


	GLuint m_render_program; // 渲染模型着色器
	GLuint m_resolve_program; // 重显示着色器
	
	GLuint m_quad_vao;
	GLuint m_quad_vbo;
	
	GLuint m_xfb;
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

DEFINE_APP(OverDrawCount, "overDrawCount")

void OverDrawCount::InitShaders()
{
	ShaderInfo render_shaders[] = {
		{GL_VERTEX_SHADER,"Media/Shaders/11/OverDrawCount.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/11/OverDrawCount.fs.glsl"},
		{GL_NONE,NULL} 
	};
	m_render_program = LoadShaders(render_shaders);
	m_model_matrix_loc = glGetUniformLocation(m_render_program,"model_matrix");
	m_view_matrix_loc = glGetUniformLocation(m_render_program,"view_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_render_program,"projection_matrix");

	ShaderInfo blit_shaders[] = {
		{GL_VERTEX_SHADER,"Media/shaders/11/OverDrawCount_blit.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/11/OverDrawCount_blit.fs.glsl"},
		{GL_NONE,NULL}
	};
	m_resolve_program = LoadShaders(blit_shaders);
}

void OverDrawCount::InitBuffers()
{
	// TBO
	glGenBuffers(1,&m_image_palette_buffer);
	glBindBuffer(GL_TEXTURE_BUFFER,m_image_palette_buffer); 
	// glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
	glBufferData(GL_TEXTURE_BUFFER,256*4*sizeof(float),NULL,GL_STATIC_DRAW); // 此处分配内存大小类似于一个1D纹理，使用256*1 的GL_RGBA32F,每个颜色一个float(4字节 4*8=32bit）
	glGenTextures(1,&m_image_palette_texture);
	glBindTexture(GL_TEXTURE_BUFFER,m_image_palette_texture); // 将纹理绑定到纹理缓冲区对象
	glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32F,m_image_palette_buffer);
	
	vmath::vec4* data = (vmath::vec4*)glMapBuffer(GL_TEXTURE_BUFFER,GL_WRITE_ONLY);
	for(int i = 0; i < 256; i++)
	{
		// 初始化1D纹理数据
		data[i] = vmath::vec4((float)i);
		//data[i] = vmath::vec4((float)i, (float)(256-i),(float)(rand()&0xFFFF),1.0f);
	}
	glUnmapBuffer(GL_TEXTURE_BUFFER);
	
	// create output texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&m_overdraw_count_buffer);
	glBindTexture(GL_TEXTURE_2D,m_overdraw_count_buffer);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	// glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
	glTexImage2D(GL_TEXTURE_2D,0,GL_R32UI,MAX_FRAMEBUFFER_WIDTH,MAX_FRAMEBUFFER_HEIGHT,0,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL);
	glBindTexture(GL_TEXTURE_2D,0);

	glGenBuffers(1,&m_overdraw_count_clear_buffer);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,m_overdraw_count_clear_buffer); // 从buffer -->> Framebuffer or texture buffer
	// 对buffer 分配空间 ，红宝书使用GLuint，改成GLFloat也是可以的
	glBufferData(GL_PIXEL_UNPACK_BUFFER,MAX_FRAMEBUFFER_WIDTH*MAX_FRAMEBUFFER_HEIGHT* 4 *sizeof(GLuint),NULL,GL_STATIC_DRAW); 
	data = (vmath::vec4*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_WRITE_ONLY);
	memset(data,0x00,MAX_FRAMEBUFFER_HEIGHT*MAX_FRAMEBUFFER_WIDTH * 4 *sizeof(GLuint));
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
	// creat  vao 
	static const GLfloat quad_vertices[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		-1.0f,  1.0f,
		 1.0f,  1.0f,
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
void OverDrawCount::Initialize(const char* title)
{
	base::Initialize(title);
	
	InitShaders();
	InitBuffers();
	
	std::string filePath = Utils::instance()->getMediaPath() + "Media/unit_pipe.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
}

void OverDrawCount::Display(bool auto_redraw)
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
	
	// clear output image  使用 m_output_texture_clear_buffer中的数据，重新覆盖m_output_texture中的数据，即清空原有数据，准备重新写入。
	/*
	GL_PIXEL_PACK_BUFFER_ARB 传递像素数据到PBO中。或GL_PIXEL_UNPACK_BUFFER_ARB 从PBO中传回数据
	例如，glReadPixels()和glGetTexImage()是"pack"像素操作， glDrawPixels(), glTexImage2D() ，glTexSubImage2D() 是"unpack" 操作。
	当一个PBO的标志为GL_PIXEL_PACK_BUFFER_ARB, glReadPixels()从OpenGL的一个帧缓冲区读取数亿，并将数据写(pack)入PBO中。
	当一个PBO的标志为GL_PIXEL_UNPACK_BUFFER_ARB时, glDrawPixels()从PBO读取(unpack)像素数据并复制到OpenGL帧缓冲区中。
	PBO的主要优势是可以快速地传递像素数据通过显卡的DMA (Direct Memory Access) 而涉及CPU循环。另一个优势是它有异步DMA传输。
	让我们对比使用PBO后的纹理传送方法。左侧图是从图像文件或视频中加载纹理。首先，资源被加载到系统内存中，
	然后使用glTexImage2D()函数从系统内存复制到OpenGL纹理对象中。这两次数据传输(加载和复制)完全由CPU执行。
	https://blog.csdn.net/dreamcs/article/details/7708018
	*/
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER,m_overdraw_count_clear_buffer);
	glBindTexture(GL_TEXTURE_2D,m_overdraw_count_buffer);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,m_current_width,m_current_height,GL_RED_INTEGER,GL_UNSIGNED_INT,NULL); // 从pbo中传递数据到texture中
	glBindTexture(GL_TEXTURE_2D,0);

    // Bind output image for read-write
	glBindImageTexture(0,m_overdraw_count_buffer,0,GL_FALSE,0,GL_READ_WRITE,GL_R32UI);
	
	  vmath::mat4 model_matrix = vmath::translate(0.0f, 0.0f, -15.0f) *
                               vmath::rotate(t * 360.0f, 0.0f, 0.0f, 1.0f) *
                               vmath::rotate(t * 435.0f, 0.0f, 1.0f, 0.0f) *
                               vmath::rotate(t * 275.0f, 1.0f, 0.0f, 0.0f);
    vmath::mat4 view_matrix = vmath::mat4::identity();
    vmath::mat4 projection_matrix = vmath::frustum(-1.0f, 1.0f, m_aspect, -m_aspect, 1.0f, 40.f);
	
	glUseProgram(m_render_program);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	glUniformMatrix4fv(m_model_matrix_loc,1,GL_FALSE,model_matrix);
	glUniformMatrix4fv(m_view_matrix_loc,1,GL_FALSE,view_matrix);
	
	// 禁止写入颜色缓存
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);	
	m_object.Render(0,8 * 8 * 8);

	// 启用写入颜色缓存
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	
	glBindImageTexture(0,m_overdraw_count_buffer,0,GL_FALSE,0,GL_READ_ONLY,GL_R32UI);
	
	glBindVertexArray(m_quad_vao);
	glUseProgram(m_resolve_program);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	base::Display(auto_redraw);
}

void OverDrawCount::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(1,&m_quad_vao);
	glDeleteBuffers(1,&m_quad_vbo);
}

void OverDrawCount::Reshape(int width, int height)
{
	m_aspect  = GLfloat(width)/height;
	glViewport(0,0,width,height);

	m_current_width = width;
	m_current_height = height;
}

