
#include "vapp.h"
#include "vutils.h"
#include "vbm.h"
#include <string>
#include "vmath.h"
#include "LoadShaders.h"

#include <stdio.h>
BEGIN_APP_DECLARATION(FurApplication)
    // Override functions from base class
    virtual void Initialize(const char * title);
    virtual void Display(bool auto_redraw);
    virtual void Finalize(void);
    virtual void Reshape(int width, int height);

    // Member variables
    float  m_aspect;
	GLuint m_texture;
    GLuint m_base_prog;
    GLuint m_fur_prog;
    GLuint fur_texture;
    VBObject m_object;

    GLint m_fur_model_matrix_pos;
    GLint m_fur_projection_matrix_pos;
    GLint m_base_model_matrix_pos;
    GLint m_base_projection_matrix_pos;
END_APP_DECLARATION()

DEFINE_APP(FurApplication, "Fur Rendering")

void FurApplication::Initialize(const char* title)
{
	base::Initialize(title);
	
	ShaderInfo base_shaders[] = 
	{
		{GL_VERTEX_SHADER,"Media/shaders/10/fur_base.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/10/fur_base.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_base_prog = LoadShaders(base_shaders);
	m_base_model_matrix_pos = glGetUniformLocation(m_base_prog,"model_matrix");
	m_base_projection_matrix_pos = glGetUniformLocation(m_base_prog,"projection_matrix");
	
	ShaderInfo fur_shaders[] = {
		{GL_VERTEX_SHADER,"Media/shaders/10/fur.vs.glsl"},
		{GL_GEOMETRY_SHADER,"Media/shaders/10/fur.gs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/10/fur.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_fur_prog = LoadShaders(fur_shaders);
	m_fur_model_matrix_pos = glGetUniformLocation(m_fur_prog,"model_matrix");
	m_fur_projection_matrix_pos = glGetUniformLocation(m_fur_prog,"projection_matrix");
	
	glGenTextures(1,&m_texture);
	glBindTexture(GL_TEXTURE_2D,m_texture);
	
	unsigned char * tex = (unsigned char *)malloc(1024 * 1024 * 4);
    memset(tex, 0, 1024 * 1024 * 4);

    int n, m;

    for (n = 0; n < 256; n++)
    {
        for (m = 0; m < 1270; m++)
        {
            int x = rand() & 0x3FF;
            int y = rand() & 0x3FF;
            tex[(y * 1024 + x) * 4 + 0] = (rand() & 0x3F) + 0xC0;
            tex[(y * 1024 + x) * 4 + 1] = (rand() & 0x3F) + 0xC0;
            tex[(y * 1024 + x) * 4 + 2] = (rand() & 0x3F) + 0xC0;
            tex[(y * 1024 + x) * 4 + 3] = n;
        }
    }
	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1024,1024,0,GL_RGBA,GL_UNSIGNED_BYTE,tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	free(tex);
	
	std::string path = Utils::instance()->getMediaPath() + "Media/ninja.vbm";
	m_object.LoadFromVBM(path.c_str(),0,1,2);
	
}

void FurApplication::Display(bool auto_redraw)
{
	  float t = float(GetTickCount() & 0x3FFF) / float(0x3FFF);
    static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
    static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
    static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vmath::mat4 p(vmath::frustum(-1.0f, 1.0f, m_aspect, -m_aspect, 1.0f, 5000.0f));
    vmath::mat4 m;

    m = vmath::mat4(vmath::translate(0.0f,
                                       0.0f,
                                       /* 100.0f * sinf(6.28318531f * t)*/ - 130.0f) *
                    //vmath::rotation(360.0f * t, X) *
                    vmath::rotate(360.0f * t * 1.0f, Y) *
                    vmath::rotate(180.0f, Z) *
                    vmath::translate(0.0f, -80.0f, 0.0f));

    glUseProgram(m_base_prog);
	glUniformMatrix4fv(m_base_model_matrix_pos,1,GL_FALSE,m[0]);
	glUniformMatrix4fv(m_base_projection_matrix_pos,1,GL_FALSE,p[0]);


	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_object.Render();
	
	glUseProgram(m_fur_prog);

    glUniformMatrix4fv(m_fur_model_matrix_pos, 1, GL_FALSE, m[0]);
    glUniformMatrix4fv(m_fur_projection_matrix_pos, 1, GL_FALSE, p);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glDepthMask(GL_FALSE); // 禁止写入深度缓存
	
	m_object.Render();
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	base::Display(auto_redraw);
}

void FurApplication::Finalize()
{
	
	
}

void FurApplication::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect = float(width)/height;
	
}
