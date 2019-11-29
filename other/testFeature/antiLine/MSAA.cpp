
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>

#include <LoadShaders.h> 
#include <vmath.h>

// https://www.shadertoy.com/new
// https://www.shadertoy.com/view/XtB3zw
// shadertoy 抗锯齿处理
void Init()
{
	glewInit();


}

void Reshape(int w,int h)
{
	glViewport(0,0,w,h);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GLUT_MULTISAMPLE);
	glTranslatef(-0.4f,0.0f,0.0f);
	glutSolidTeapot(0.3);

	glDisable(GLUT_MULTISAMPLE);
	glColor3f(1.0f,0.0f,0.0f);
	glTranslatef(0.8f,0.0f,0.0f);
	glutSolidTeapot(0.3);

	glutSwapBuffers();
	glutPostRedisplay();
}


/*
#version 450
 
uniform sample2DMS tex;
in ivec2 texCoord;
in color;
 
out fColor;
 
void main()
{
    fColor = 0.5 * color + 0.5 * texelFetch(tex, texCoord, gl_SampleID);
}

--------------------- 
作者：yunchao_he 
来源：CSDN 
原文：https://blog.csdn.net/yunchao_he/article/details/78354528 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
void main(int argc,char* argv[])
{ 
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(600,480);
    glutCreateWindow("OpenGL反走样");
    
	Init();
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);

    glutMainLoop();
}