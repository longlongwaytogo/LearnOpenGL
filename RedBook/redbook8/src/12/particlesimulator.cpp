// ParticleSimulator.cpp

#include "vapp.h"
#include "vutils.h"
#include "vmath.h"
#include "LoadShaders.h"
#include <stdio.h>

enum 
{
	PARTICLE_GROUP_SIZE  = 128,
	PARTICLE_GROUP_COUNT = 8000,
	PARTICLE_COUNT		 = PARTICLE_GROUP_COUNT * PARTICLE_GROUP_SIZE,
	MAX_ATTRACTORS = 64 
};



BEGIN_APP_DECLARATION(ParticleSimulator)
	virtual void Initialize(const char* title);
	virtual void Display(bool auto_redraw);
	virtual void Finalize();
	virtual void Reshape(int width, int height);
	
	GLuint m_compute_program;
	GLuint m_compute_shader;
	
	union
	{
		struct
		{
			GLuint position_buffer;
			GLuint velocity_buffer;
		};
		GLuint m_buffers[2];
	};
	
	union
	{
		struct
		{
			GLuint position_tbo;
			GLuint velocity_tbo;
		};
		GLuint m_tbos[2];
	};
	
	// Attractor UBO
	GLuint m_attractor_buffer;
	
	// Mass of the attractor
	float m_attractor_masses[MAX_ATTRACTORS];
	float m_aspect_ratio;

	GLuint m_mvp_loc;
	GLuint m_dtime_loc;
	
	GLuint m_render_program;
	GLuint m_vao;
	GLuint m_vbo;
END_APP_DECLARATION()

DEFINE_APP(ParticleSimulator, "simple compute shader example")


static inline float random_float()
{
	float res;
	unsigned int tmp;
	static unsigned int seed = 0xFFFF0C59;
	seed *= 16807;
	tmp = seed ^ (seed >> 4) ^(seed << 15);
	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;
	return (res - 1.0f);
	
}

static vmath::vec3 random_vector(float minmag = 0.0f, float maxmag = 1.0f)
{
	vmath::vec3 randomvec(random_float() * 2.0f -1.0f, random_float() * 2.0f - 1.0f,random_float() * 2.0f -1.0f);
	randomvec = normalize(randomvec);
	randomvec *= (random_float() * (maxmag - minmag) + minmag);
	return randomvec;
}

void ParticleSimulator::Initialize(const char* title)
{
	base::Initialize(title);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao); 
	
	glGenBuffers(2,m_buffers);
	glBindBuffer(GL_ARRAY_BUFFER,position_buffer);
	glBufferData(GL_ARRAY_BUFFER,PARTICLE_COUNT * sizeof(vmath::vec4),NULL,GL_DYNAMIC_COPY);
	// 映射位置缓存并使用随机响亮初始化
	// glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
	vmath::vec4* positions = (vmath::vec4*) 
	glMapBufferRange(GL_ARRAY_BUFFER, // target
					0, // offset 
					sizeof(vmath::vec4)*PARTICLE_COUNT, // length
					GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = vmath::vec4(random_vector(-10.0f,10.0f),random_float());
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	 glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,NULL);
	 glEnableVertexAttribArray(0);
	
	//glBindImageTexture(0,m_position_buffer,0,GL_FALSE,0,GL_WRITE_ONLY,GL_RGBA32F);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	// 初始化速度缓存
	glBindBuffer(GL_ARRAY_BUFFER,velocity_buffer);
	glBufferData(GL_ARRAY_BUFFER,
				PARTICLE_COUNT*sizeof(vmath::vec4),
				NULL,
				GL_DYNAMIC_COPY);
	vmath::vec4* velocities = (vmath::vec4*)glMapBufferRange(GL_ARRAY_BUFFER,
										0, // offset
										sizeof(vmath::vec4)*PARTICLE_COUNT, // length
										GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int i = 0; i < PARTICLE_COUNT; i++)
	{
		velocities[i] = vmath::vec4(random_vector(-0.1f, 0.1f),0.0f);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	// 生成tbo
	glGenTextures(2, m_tbos);
	for(int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_BUFFER,m_tbos[i]);
		glTexBuffer(GL_TEXTURE_BUFFER,GL_RGBA32F,m_buffers[i]);
	}
	
	glGenBuffers(1, &m_attractor_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_attractor_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 32* sizeof(vmath::vec4),NULL,GL_STATIC_DRAW);
	
	for(int i = 0; i < MAX_ATTRACTORS; ++i)
	{
		m_attractor_masses[i] = 0.5f + random_float() * 0.5f;
	}
	// 设置uniform的绑定索引为0
	glBindBufferBase(GL_UNIFORM_BUFFER,0,m_attractor_buffer);
	
	ShaderInfo renderShaders[] = 
	{
		{GL_VERTEX_SHADER,"Media/shaders/12/particlesimulator.vs.glsl"},
		{GL_FRAGMENT_SHADER,"Media/shaders/12/particlesimulator.fs.glsl"},
		{GL_NONE,NULL}
	};
	
	m_render_program = LoadShaders(renderShaders);
	m_mvp_loc = glGetUniformLocation(m_render_program,"mvp");

	ShaderInfo computeShaders[] = 
	{
		{GL_COMPUTE_SHADER,"Media/shaders/12/particlesimulator.cp.glsl"},
		{GL_NONE,NULL}
	};
	
	m_compute_program = LoadShaders(computeShaders);
	m_dtime_loc = glGetUniformLocation(m_compute_program,"dt");

}

void ParticleSimulator::Display(bool auto_redraw)
{
	static const GLuint start_ticks = ::GetTickCount() - 100000;
    GLuint current_ticks = ::GetTickCount();
    static GLuint last_ticks = current_ticks;
    float time = ((start_ticks - current_ticks) & 0xFFFFF) / float(0xFFFFF);
    float delta_time = (float)(current_ticks - last_ticks) * 0.075f;
	
	// 更新质量
	vmath::vec4* attractors = (vmath::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER,
											0,
											32 * sizeof(vmath::vec4),
											GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
	for(int i = 0; i < 32; ++i)
	{
		  attractors[i] = vmath::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
                                    cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
                                    sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
                                    m_attractor_masses[i]);
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	
	if(delta_time >= 2.0f)
	{
		delta_time = 2.0f;
	}
	
	glUseProgram(m_compute_program);
	glBindImageTexture(0,velocity_tbo,0,GL_FALSE,0,GL_READ_WRITE,GL_RGBA32F);
	glBindImageTexture(1,position_tbo,0,GL_FALSE,0,GL_READ_WRITE,GL_RGBA32F);

	glUniform1f(m_dtime_loc,delta_time);

	glDispatchCompute(PARTICLE_GROUP_COUNT,1,1);
	
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); 
	
	 vmath::mat4 mvp = vmath::perspective(45.0f, m_aspect_ratio, 0.1f, 1000.0f) *
                      vmath::translate(0.0f, 0.0f, -60.0f) *
                      vmath::rotate(time * 1000.0f, vmath::vec3(0.0f, 1.0f, 0.0f));
					 
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(m_render_program);
	glUniformMatrix4fv(m_mvp_loc,1,GL_FALSE,mvp);
	glBindVertexArray(m_vao);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	
	glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
	
	last_ticks = current_ticks;

	base::Display();
}
void ParticleSimulator::Finalize()
{
	glUseProgram(0);
	glDeleteVertexArrays(1,&m_vao);
	glDeleteProgram(m_compute_program);
}

void ParticleSimulator::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	m_aspect_ratio = float(width)/ height;
}
	