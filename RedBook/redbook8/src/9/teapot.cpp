#include <vgl.h>
#include <LoadShaders.h>
#include <vmath.h>
#include <Shapes/Teapot.h>
#include <mat.h>
#include <iostream>


using namespace std;


GLuint Proj_loc;  // projection matrix
GLuint Inner_loc; // inner tessellation parameter
GLuint Outer_loc;  // Outer tessellation parameter

GLfloat Inner = 1.0f;
GLfloat Outer = 1.0f;


void Info()
{
	std::cout<< "cmd:" << std::endl;
	std::cout << "q: quit"<< std::endl;
	std::cout << "i: inner--" << std::endl;
	std::cout << "I: inner++" << std::endl;
	std::cout << "o: outer--" << std::endl;
	std::cout << "O: outer++" << std::endl;
	std::cout << "r/R: 复位" << std::endl;
	std::cout << " m/M: 填充模式/网格模式" << std::endl;
}
void init()
{
	Info();

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	enum {ArrayBuffer, ElementBuffer,NumVertexBuffers};
	GLuint buffers[NumVertexBuffers];
	glGenBuffers(NumVertexBuffers,buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(TeapotVertices),
	TeapotVertices,GL_STATIC_DRAW);
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,buffers[ElementBuffer]);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(TeapotIndices),TeapotIndices,GL_STATIC_DRAW);
	
	
	
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "Media/shaders/9/teapot.vs.glsl"},
		{ GL_TESS_CONTROL_SHADER, "Media/shaders/9/teapot.cont.glsl"},
		{ GL_TESS_EVALUATION_SHADER, "Media/shaders/9/teapot.eval.glsl"},
		{ GL_FRAGMENT_SHADER, "Media/shaders/9/teapot.fs.glsl"},
		{ GL_NONE, NULL}
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	
	GLuint vPosition = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition,3,GL_DOUBLE,GL_FALSE,0,BUFFER_OFFSET(0));

	Proj_loc = glGetUniformLocation(program,"P");
	Inner_loc = glGetUniformLocation(program,"Inner");
	Outer_loc = glGetUniformLocation(program,"Outer");
	
	glUniform1f(Inner_loc,Inner);
	glUniform1f(Outer_loc,Outer);
	
	mat4 modelview = Translate( -0.2625f, - 1.575f, -1.0f);
	modelview *= Translate(0.0f,0.0f, -7.5f);
	glUniformMatrix4fv(glGetUniformLocation(program,"MV"),1,GL_TRUE,modelview);
	
	glPatchParameteri(GL_PATCH_VERTICES,NumTeapotVerticesPerPatch);
	glClearColor( 0.0, 0.0, 0.0, 1.0);
	
	
}
	
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_PATCHES,NumTeapotVertices,GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glutSwapBuffers();
}

void reshape( int width, int height)
{

	glViewport(0,0,width,height);
	GLfloat aspect = GLfloat(width)/GLfloat(height);
	mat4 projection = Perspective(60.0,aspect,5,10);
	glUniformMatrix4fv(Proj_loc,1,GL_TRUE,projection);
}




void keyboard(unsigned char key,int x, int y)
{
	switch(key)
	{
		case 'q':
		case 'Q': 
		case 033:
		{
			exit(0);
			break;
		}
		case 'i':
		{
			Inner--;
			if(Inner < 1.0) Inner = 1.0;
			glUniform1f(Inner_loc,Inner);
			break;
		}
		case 'I':
		{
			Inner++;
			if(Inner > 64.0) Inner = 64.0;
			glUniform1f(Inner_loc,Inner);
			break;
		}
		case 'o':
		{
			Outer--;
			if(Outer < 1.0) Outer = 1.0;
			glUniform1f(Outer_loc,Outer);
			break;
		}
		case 'O':
		{
			Outer++;
			if(Outer > 64.0f) Outer = 64.0;
			glUniform1f(Outer_loc,Outer);
			break;
		}
		case 'r':
		case 'R':
		{
			Outer = 1.0;
			Inner = 1.0;
			glUniform1f(Outer_loc,Outer);
			glUniform1f(Inner_loc,Inner);
			break;
		}
		
		case 'm':
		case 'M':
		{
			static GLenum mode = GL_LINE;
			mode = (mode == GL_FILL? GL_LINE  : GL_FILL);
			glPolygonMode(GL_FRONT_AND_BACK,mode);
			break;
		}
		
	}
	
	glutPostRedisplay();
	
}

int
    main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "teapot" );

    glewInit();
    glGetError(); 

    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
