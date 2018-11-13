// bezcurve.cabs

#include <GL/glut.h>

GLfloat ctrlPoints[4][3] = {
	{-4.0, -4.0, 0.0}, {-2.0, 4.0, 0.0},
	{ 2.0, -4.0, 0.0}, { 4.0, 4.0, 0.0}};

int render_mode = GL_LINE_STRIP;	
float dimension = 30.0f;
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	// glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlPoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
}

void display(void)
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	//glBegin(GL_LINE_STRIP);
	 glBegin(render_mode);
		for(i = 0; i <= dimension; i++)
		{
			glEvalCoord1f((GLfloat)i/dimension);
		}
	glEnd();
	
	glPointSize(5.0);
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_POINTS);
		for(i = 0; i < 4; ++i)
			glVertex3fv(&ctrlPoints[i][0]);
	glEnd();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w >=h)
		glOrtho(-5.0,5.0,-5.0*(GLfloat)w/(GLfloat)h, 5.0*(GLfloat)w/(GLfloat)h,-5,5);
	else
		glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,5.0*(GLfloat)h/(GLfloat)w,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void keyboard(unsigned int key, int x, int y)
{
	switch(key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		if(render_mode == GL_POINTS)
			render_mode = GL_LINE_STRIP;
		else
			render_mode = GL_POINTS;
		break;

	case 'a':
		dimension++;
		break;
	case 'd':
		dimension--;
		break;
	}

	if(dimension > 60) dimension =60;
	if(dimension < 6) dimension = 6;
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(10,10);
	glutCreateWindow("bezcurve");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}