
// trim.c

// surface.c

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CALLBACK 
#define CALLBACK __stdcall 
#endif 

GLfloat ctlpoints[4][4][3];
int showPoints = 0;
int showMode = GLU_FILL;
GLUnurbsObj* theNurb;

void init_surface(void)
{
	int u,v;
	for(u = 0; u < 4; u++)
	{
		for(v = 0; v < 4; v++)
		{
			ctlpoints[u][v][0] = 2.0*((GLfloat)u - 1.5);
			ctlpoints[u][v][1] = 2.0*((GLfloat)v - 1.5);
			
			if((u == 1 || u == 2) && (v == 1|| v == 2))
				ctlpoints[u][v][2] = 3.0;
			else
				ctlpoints[u][v][2] = - 3.0;
		}
	}
}

void CALLBACK nurbsError(GLenum errorCode)
{
	const GLubyte* estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr,"Nurbs error:%s\n",estring);
	exit(0);
}

void init(void)
{
	GLfloat mat_diffuse [] = {0.7, 0.7, 0.7, 1.0};
	GLfloat mat_specular[] = { 1.0, 1.0,1.0,1.0};
	GLfloat mat_shininess[] = { 100.0};
	
	glClearColor(0.0, 0.0, 0.0,0.0);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	
	init_surface();
	
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb,GLU_SAMPLING_TOLERANCE, 25);
	gluNurbsProperty(theNurb,GLU_DISPLAY_MODE,GLU_FILL);
	
}



void display(void)
{
	int i, j;
	
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	GLfloat edgePt[5][2] = /* counter colckwise*/
	{{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0},{0.0,0.0}};
	GLfloat curvePt[4][2] = /*clockwise*/
	{{0.25,0.5},{0.25,0.75},{0.75,0.75},{0.75,0.5}};
	GLfloat curveKnots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	GLfloat pwlPt[3][2] = /*clockwise*/
	{{0.75,0.5},{0.5,0.25},{0.25,0.5}};
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(330,1,0.,0.);
	glScalef(0.5,0.5,0.5);
	
	gluBeginSurface(theNurb);
	
	gluNurbsSurface(theNurb,8,knots,8,knots,4*3, 3,&ctlpoints[0][0][0],	4,4,GL_MAP2_VERTEX_3);
	
		gluBeginTrim(theNurb);
		/*
		gluPwlCurve (
			GLUnurbs            *nobj, 
			GLint               count, 
			GLfloat             *array, 
			GLint               stride, 
			GLenum              type);
		*/
			gluPwlCurve(theNurb,5,&edgePt[0][0],2,GLU_MAP1_TRIM_2);
		gluEndTrim(theNurb);
	
		gluBeginTrim(theNurb);
		/*
		gluNurbsCurve (
			GLUnurbs            *nobj, 
			GLint               nknots, 
			GLfloat             *knot, 
			GLint               stride, 
			GLfloat             *ctlarray, 
			GLint               order, 
			GLenum              type);
		*/
			gluNurbsCurve(theNurb,8,curveKnots,2,&curvePt[0][0],4,GLU_MAP1_TRIM_2);
			gluPwlCurve(theNurb,3,&pwlPt[0][0],2,GLU_MAP1_TRIM_2);
		gluEndTrim(theNurb);
	
	gluEndSurface(theNurb);
	
	glPopMatrix();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLfloat)w,(GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,(GLdouble)w/(GLdouble)h,3.0,8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-5.0);
	
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'w':
	case 'W':
		if(showMode == GLU_FILL)
			showMode = GLU_OUTLINE_POLYGON;
		
		else if(showMode == GLU_OUTLINE_POLYGON)
			showMode = GLU_OUTLINE_PATCH;
		else 
			showMode = GLU_FILL;
		break;
		case 'c':
		case 'C':
			showPoints = !showPoints;
			
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(500,500);
	glutCreateWindow("nurbs");
	
	init();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	
	glutMainLoop();
	return 0;
}