// ch012-2 bezsurf.c

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define USE_GLEvalMesh2 1
typedef enum  _tagMode
{
	MODE_POINTS = GL_POINTS,
	MODE_LINES = GL_LINE_STRIP,
	MODE_POLYGON = GL_QUAD_STRIP,
	
} MODE;
MODE mode[3];
MODE current_mode;

int mode_index = 0;




GLfloat ctrlpoints[4][4][3] = {
	{{-1.5,-1.5, 4.0}, {-0.5, -1.5, 2.0},
	{ 0.5, -1.5, -1.0},{1.5, -1.5, 2.0}},
	 
	{{-1.5, -0.5, 1.0},{-0.5, -0.5, 3.0},
	{0.5, -0.5, 0.0},{1.5,-0.5, -1.0}},
	
	{{-1.5,0.5, 4.0},{-0.5,0.5,0.0},
	{0.5,0.5, 3.0},{1.5,0.5,4.0}},
	
	{{-1.5,1.5,-2.0},{-0.5,1.5,-2.0},
	{0.5,1.5,0.0},{1.5,1.5,-1.0}}
};

void display(void)
{
	int i,j;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	glRotatef(85.0,1.0,1.0,1.0);

	if(current_mode == GL_POINTS)
	{
		//glEnable(GL_POINTS);
		//glPointSize(5.0f);
		//glEvalMesh2(current_mode,0,20,0,20);
		///*glBegin(GL_POINTS);

		//for(i = 0; i < 30; ++i)
		//	for(j = 0; j<= 8; ++j)
		//	{

		//	}
		//glEnd();*/
		//glDisable(GL_POINTS);

		glPointSize(5.0f);
#if !USE_GLEvalMesh2 
		for(j = 0; j <= 1; j++)
		{
			glBegin(GL_POINTS);
			for(i = 0; i <= 20; ++i)
			{
				glEvalCoord2f((GLfloat)i/20.0,(GLfloat)j/20.0);
			}
			glEnd();
		
			glBegin(GL_POINTS);
			for(i = 0; i <= 20; i++)
				glEvalCoord2f((GLfloat)j/20,(GLfloat)i/20.0);
			glEnd();
		}
#else
		glEvalMesh2(GL_POINT,0,20,0,20);
#endif 
		
		glPointSize(1.0f);

	}
	else if(current_mode == GL_LINE_STRIP)
	{
		//glEvalMesh2(current_mode,0,20,0,20);
#if !USE_GLEvalMesh2
		for(j = 0; j <= 8; j++)
		{
			glBegin(GL_LINE_STRIP);
			for(i = 0; i <= 30; ++i)
			{
				glEvalCoord2f((GLfloat)i/30.0,(GLfloat)j/8.0);
			}
			glEnd();
		
			glBegin(GL_LINE_STRIP);
			for(i = 0; i <= 30; i++)
				glEvalCoord2f((GLfloat)j/8.0,(GLfloat)i/30.0);
			glEnd();
		}
#else
		glEvalMesh2(GL_LINE,0,20,0,20);
#endif 
			 
	}
	else if(current_mode == GL_QUAD_STRIP)
	{
		glEvalMesh2(GL_FILL,0,20,0,20);
	}

	glPopMatrix();
	glFlush();
}


void init(void)
{
	mode[0] = MODE_POINTS;
	mode[1] = MODE_LINES;
	mode[2] = MODE_POLYGON;
	current_mode = mode[0];

	glClearColor(0.0,0.0,0.0,0.0);
 
	glMap2f(GL_MAP2_VERTEX_3,0,1,3,4,
	0,1,12,4,&ctrlpoints[0][0][0]);

 
	glEnable(GL_MAP2_VERTEX_3);
	// 定义二维网格，从u1到u2,经历un个均匀分布的阶段
	//void APIENTRY glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
	glMapGrid2f(20,0.0,1.0,20,0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}

void keyboard(unsigned int key, int x, int y)
{
	switch(key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		current_mode = mode[++mode_index%3];
		break;
	}
	glutPostRedisplay();

}

void reshape(int w,int h)
{
	glViewport(0,0,(GLfloat)w,(GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	if(w <= h)
		glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/w, 5.0*(GLfloat)h/w,-5.0, 5.0);
	else
		glOrtho(-5.0, 5.0, -5.0*(GLfloat)w/h, 5.0*(GLfloat)w/h,-5.0, 5.0);
}
void main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(800,600);
	glutCreateWindow("bezsurf");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}