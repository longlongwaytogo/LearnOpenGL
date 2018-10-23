// Draw_xfb.cpp

#include <vapp.h>
#include <vutils.h>
#include <LoadShaders.h>
#include <vbm.h>
#include <vmath.h>
#include <stdio.h>
#include <iostream>

BEGIN_APP_DECLARATION(Draw_xfb)
	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
private:
	GLuint m_program;
	GLuint m_sort_program;
	
	GLuint m_vao[2];
	GLuint m_vbo[2];
	
	GLuint m_xfb;
	GLfloat m_aspect;
	
	GLuint m_projection_matrix_loc;
	GLuint m_model_view_matrix_loc;
	
	VBObject m_object;
	
	
END_APP_DECLARATION()


DEFINE_APP(Draw_xfb, "draw_xfb")


void Draw_xfb::Initialize(const char* title)
{
	base::Initialize(title);
	
	// create transformfeedback object
	glGenTransformFeedbacks(1,&m_xfb);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,m_xfb);
	
	
	ShaderInfo sort_shaders [] = {
		{GL_VERTEX_SHADER, "Media/Shaders/10/Draw_xfb.sort.vs.glsl"},
		{GL_GEOMETRY_SHADER, "Media/Shaders/10/Draw_xfb.sort.gs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_sort_program = LoadShaders(sort_shaders);
	
	glUseProgram(m_sort_program);
	ShowShaderInfo(m_sort_program);
	m_model_view_matrix_loc = glGetUniformLocation(m_sort_program,"model_matrix");
	m_projection_matrix_loc = glGetUniformLocation(m_sort_program,"projection_matrix");

	static const char* varyings[] = 
	{
		"rf_position","rf_normal",
		"gl_NextBuffer",
		"lf_position","lf_normal"
	};
	
	glTransformFeedbackVaryings(m_sort_program,5,varyings,GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_sort_program);
	
	{
    GLint linked;
    glGetProgramiv( m_sort_program, GL_LINK_STATUS, &linked );
    if ( !linked ) 
	{
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv( m_sort_program, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog( m_sort_program, len, &len, log );
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete [] log;
#endif /* DEBUG */
	}
	}

	ShowShaderInfo(m_sort_program);

	glGenVertexArrays(2,m_vao);
	glGenBuffers(2,m_vbo);
	
	for(int i = 0; i < 2; ++i)
	{
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER,m_vbo[i]);
		glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,1024*1024*sizeof(GLfloat),NULL,GL_DYNAMIC_COPY);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER,i,m_vbo[i]); // 讲buffer绑定到指定绑定点上
		
		glBindVertexArray(m_vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER,m_vbo[i]);// 讲m_vbo 再次绑定到顶点缓存中，一个buffer 可以被多个对象绑定
		// glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
		glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, sizeof(vmath::vec4) + sizeof(vmath::vec3),NULL);
		glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, sizeof(vmath::vec4) + sizeof(vmath::vec3),(GLvoid *)(sizeof(vmath::vec4)));
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
	
	ShaderInfo render_shaders[] = {
		{GL_VERTEX_SHADER,"Media/Shaders/10/Draw_xfb.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/Shaders/10/Draw_xfb.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_program = LoadShaders(render_shaders);
 
	GLuint pass_color_loc = glGetUniformLocation(m_program,"pass_color");
	glUniform1f(pass_color_loc,0);
	
	std::string filePath = Utils::instance()->getMediaPath() + "Media/ninja.vbm";
	m_object.LoadFromVBM(filePath.c_str(),0,1,2);
	
	
}


void Draw_xfb::Display(bool auto_redraw)
{
	float t = float(GetTickCount() & 0x3FFF)/0x3FFF;
	static vmath::vec3 X(1.0f, 0.0f, 0.0f);
	static vmath::vec3 Y(0.0f, 1.0f, 0.0f);
	static vmath::vec3 Z(0.0f, 0.0f, 1.0f);
	vmath::mat4 projection_matrix(vmath::frustum(-1.0f, 1.0f, m_aspect, -m_aspect, 1.0f, 5000.0f));
	vmath::mat4 model_view_matrix = vmath::mat4(vmath::translate(0.0f,
                                       0.0f,
                                       100.0f * sinf(6.28318531f * t) - 230.0f) *
                    vmath::rotate(360.0f * t, X) *
                    vmath::rotate(360.0f * t * 2.0f, Y) *
                    vmath::rotate(360.0f * t * 5.0f, Z) *
                    vmath::translate(0.0f, -80.0f, 0.0f));

	
	glUseProgram(m_sort_program);
	

	glUniformMatrix4fv(m_model_view_matrix_loc,1,GL_FALSE,model_view_matrix);
	glUniformMatrix4fv(m_projection_matrix_loc,1,GL_FALSE,projection_matrix);
	
	glClearColor( 0.3f, 0.1f, 0.2f, 1.0f );
    glClearDepth( 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	
	glEnable(GL_RASTERIZER_DISCARD);// 禁用光栅化操作
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,m_xfb);
	
	glBeginTransformFeedback(GL_POINTS);
		m_object.Render();
	glEndTransformFeedback();
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,0);
	
	glDisable(GL_RASTERIZER_DISCARD);//启用光栅化操作
	static const vmath::vec4 colors[2] = {
		vmath::vec4(0.8f, 0.8f, 0.9f, 0.5f),
		vmath::vec4(0.3f, 1.0f, 0.3f, 0.8f)
	};
	
	glUseProgram(m_program);
	for(int i = 0; i < 2; ++i)
	{
		glUniform4fv(0,1,colors[i]);
		glBindVertexArray(m_vao[i]);
		glDrawTransformFeedbackStream(GL_TRIANGLES,m_xfb,i);
	}

	base::Display(auto_redraw);
	
}


void Draw_xfb::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(2,m_vao);
	glDeleteBuffers(2,m_vbo);
	
	
}

void Draw_xfb::Reshape(int width, int height)
{
	m_aspect  = GLfloat(width)/height;
	
	glViewport(0,0,width,height);
	
	float w = float(width)/2;
	float h = float(height)/2;

}

