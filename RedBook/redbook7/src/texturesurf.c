// texturesurf.c

#include <GL/glut.h>
#include <math.h>

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

GLfloat texpts[2][2][2] = {{{0.0,0.0},{0.0,1.0}},
						  {{1.0,0.0},{1.0,1.0}}};
						  
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glEvalMesh2(GL_FILL,0,20,0,20);
	glFlush();
}

#define ImageWidth 64
#define ImageHeight 64

GLubyte image[3*ImageWidth*ImageHeight];

void makeImage(void)
{
	int i,j;
	float ti,tj;
	
	for(i = 0; i < ImageWidth; ++i)
	{
		ti = 2.0 * 3.14159265*i/ImageWidth;
		for(j = 0; j < ImageHeight; ++j)
		{
			tj = 2.0 * 3.14159265*j/ImageHeight;
			image[3*(ImageHeight*i + j)] = (GLubyte)127*(1.0 +sin(ti));
			image[3*(ImageHeight*i + j) + 1] = 
			(GLubyte)127*(1.0 + cos(2*tj));
			image[3*(ImageHeight*i + j) + 2] = 
			(GLubyte)127*(1.0 + cos(ti + tj));
		}
	}
	
}

void init(void)
{
	glMap2f(GL_MAP2_VERTEX_3,0,1,3,4,
	0,1,12,4,&ctrlpoints[0][0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,0,1,2,2,
	0,1,4,2,&texpts[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glMapGrid2f(20.0,0.0,1.0,20.0,0.0,1.0);
	makeImage();
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ImageWidth,ImageHeight,0,
	GL_RGB,GL_UNSIGNED_BYTE,image);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w,
              4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
   else
      glOrtho(-4.0*(GLfloat)w/(GLfloat)h,
              4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(85.0, 1.0, 1.0, 1.0);
}
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:
		exit(0);
		break;
	
	}
	glutPostRedisplay();

}


int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_DEPTH|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(500,500);
	glutCreateWindow("texturesurf");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}























