// simple

#include <vgl.h>
#include <mat.h>
#include <LoadShaders.h>

GLuint Proj_loc;
const int NumVertices = 4;
const GLfloat zNear = 1.0f;
const GLfloat zFar = 3.0f;

void init()
{
    enum { Patch, NumVAOs };
    GLuint VAOs[NumVAOs];
    glGenVertexArrays( NumVAOs, VAOs );
    glBindVertexArray( VAOs[Patch] );
	enum { Array, NumBuffers};
	GLuint buffers[NumBuffers];
	glGenBuffers(NumBuffers,buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[Array]);

   

	
	GLfloat vertices[NumVertices][2] = {
		{ -0.5, -0.5},
		{  0.5, -0.5},
		{  0.5, -0.5},
		{ -0.5,  0.5}
	};
	
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	ShaderInfo shaders[] = {
	{ GL_VERTEX_SHADER, "Media/shaders/9/simple.vs.glsl"},
	{ GL_TESS_CONTROL_SHADER, "Media/shaders/9/simple.cont.glsl"},
	{ GL_TESS_EVALUATION_SHADER, "Media/shaders/9/simple.eval.glsl"},
	{ GL_FRAGMENT_SHADER, "Media/shaders/9/simple.fs.glsl"},
	{ GL_NONE, NULL}
    };

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	
    GLuint vPosition = glGetAttribLocation( program, "position" );

    glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,NULL);
    glEnableVertexAttribArray(vPosition);

	Proj_loc = glGetUniformLocation(program,"P");
	mat4 modelview = Translate(0,0, -0.5 * (zNear + zFar)) * RotateX( -50.0f);
	
	glUniformMatrix4fv(glGetUniformLocation(program,"MV"),1,GL_TRUE,modelview);
	
	glPatchParameteri(GL_PATCH_VERTICES,4); // 输入面片定点数量
	
	glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.0,0.0,0.0,1.0);
	
}

void display()
{

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_PATCHES,0,NumVertices);
	glutSwapBuffers();
	
	
}

void reshape(int width, int height)
{
	 glViewport(0,0,width,height);
	 GLfloat aspect = GLfloat(width)/height;
	 
	 mat4 projection = Perspective(60,aspect,zNear,zFar);
	 
	 glUniformMatrix4fv(Proj_loc,1,GL_TRUE,projection);
	 
	 glutPostRedisplay();
	 
}

void keyboard(unsigned char key, int x, int y)
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
		case 'm':
		{
			static GLenum mode = GL_FILL;
			mode = (mode == GL_FILL)? GL_LINE : GL_FILL;
			
			glPolygonMode(GL_FRONT_AND_BACK,mode);
		}
	}
	glutPostRedisplay();

}

int main(int argc, char** argv)
{
	glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "patch" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}