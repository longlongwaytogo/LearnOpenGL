// ch04 Triangles.cpp
///////////////////////////////////////////////////////////////////////\
//
// triangles.cpp
//
/////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include <vgl.h>
#include <LoadShaders.h>

enum VAO_DIS { Triangles, NumVAOs};
enum Buffer_IDs { ArrayBuffer, NumBuffers};
enum Attrib_IDs { vPosition = 0};

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init()
{
	if(glewInit()){
		std::cerr << " Unable to init GLEW, exit..." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	static const GLfloat VertexData[] =
	{
		-0.90, -0.90,
		0.85, -0.90,
		-0.90, 0.85,

		0.90, -0.85,
		0.90, 0.90,
		-0.85, 0.90,
	};

	static const GLsizei VertexSize = sizeof(VertexData);

	static const GLubyte ColorData[] =
	{
		255, 0, 0, 255,
		0, 255, 0, 255,
		0, 0, 255, 255,
		10, 10, 10, 255,
		100, 100, 100, 255,
		255, 255, 255, 255,
	};
	static const GLfloat ColorSize = sizeof(ColorData);


	glGenVertexArrays(NumVAOs,VAOs);
	glBindVertexArray(VAOs[Triangles]);
	
	
	glGenBuffers(NumBuffers,Buffers);
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(VertexData) + sizeof(ColorData),NULL,
				 GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(VertexData),VertexData);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(VertexData),sizeof(ColorData),ColorData);
			
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
	glVertexAttribPointer(1,4,GL_UNSIGNED_BYTE,GL_TRUE,0,BUFFER_OFFSET(sizeof(VertexData)));
	//Notice BYTE map to float [0.0, 1.0], so 255 is 1.0, 0 is 0.0
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#if 1
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/4/Triangle.vs.glsl"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/4/Triangle.ps.glsl"},
		{GL_NONE,NULL}
	};
#else 
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/triangles/triangles.vert"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/triangles/triangles.frag"},
		{GL_NONE,NULL}
	};

#endif 
	
	
	
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES,0,NumVertices);
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitContextVersion(4,3); // 使用opengl4.3版本
	glutInitContextProfile(GLUT_CORE_PROFILE); // 使用核心模式
	glutInitWindowSize(800,600); 
	glutInitWindowPosition(100,100);
	glutCreateWindow("1-triangles");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	
	
	glDeleteVertexArrays(NumVAOs,VAOs);
	glDeleteBuffers(NumBuffers,Buffers);
	return 0;
}