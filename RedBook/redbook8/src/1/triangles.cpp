// 1.1 Triangles.cpp
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
	
	glGenVertexArrays(NumVAOs,VAOs);
	glBindVertexArray(VAOs[Triangles]);
	
	GLfloat vertices[NumVertices][2] = {
		{ -0.90, -0.90}, // triangle 1
		{  0.85, -0.90},
		{ -0.90,  0.85},
		{  0.90, -0.85}, // triangle 2
		{  0.90,  0.90},
		{ -0.85,  0.90}
	};
	
	glGenBuffers(NumBuffers,Buffers);
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,
				 GL_STATIC_DRAW);
				 
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "Media/Shaders/triangles/triangles.vert"},
		{GL_FRAGMENT_SHADER, "Media/Shaders/triangles/triangles.frag"},
		{GL_NONE,NULL}
	};
	
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	
	glVertexAttribPointer(vPosition,2,GL_FLOAT,
					GL_FALSE,0,BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
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