#include <gl/glut.h>
#include <GL/GLU.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef GLAPIENTRY
#define GLAPIENTRY  __stdcall 
#endif
GLuint startList;

static void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glCallList(startList);
	glCallList(startList+1);
	glFlush();
}

// 分格化起始回调
static void GLAPIENTRY beginCallback(GLenum which)
{
	glBegin(which);
}

// 分格化错误回调
static void GLAPIENTRY errorCallback(GLenum errorCode)
{
	const GLubyte* estring;
	estring = gluErrorString(errorCode);
	fprintf(stderr,"Tessellation Error:%s\n",(char*) estring);
	exit(0);
}

// 分格化结束回调
static void GLAPIENTRY endCallback(void)
{
	glEnd();
}


// 定点回调
static void GLAPIENTRY vertexCallback(GLvoid* vertex)
{
	const GLdouble *pointer;
	pointer = (GLdouble*) vertex;
	glColor3dv(pointer + 3);
	glVertex3dv(pointer);
}


static void GLAPIENTRY combineCallback(GLdouble coords[3], GLdouble* vertex_data[4],
									   GLfloat weight[4], GLdouble**dataOut)
{
	GLdouble* vertex;
	int i;
	vertex = (GLdouble*)malloc(6*sizeof(GLdouble));
	 // 生成的新顶点
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	// 根据加权计算定点的颜色
	for(i = 3; i < 6; i++)
	{
		vertex[i] = weight[0] * vertex_data[0][i]    
					+ weight[1] * vertex_data[1][i]
					+ weight[2] * vertex_data[2][i]
					+ weight[3] * vertex_data[3][i];
	
	}
	*dataOut = vertex;
}

static void init(void)
{
	GLUtesselator* tobj;
	// 四边形
	GLdouble rect[4][3] = {{50.0, 50.0, 0.0},
							{200.0, 50.0, 0.0},
							{200.0, 200.0,0.0},
							{50.0, 200.0,0.0}};
	
	// 三角形
	 GLdouble tri[3][3] = {{75.0, 75.0, 0.0},
                         {125.0, 175.0, 0.0},
                         {175.0, 75.0, 0.0}};
						 
	// 五角星
   GLdouble star[5][6] = {{250.0, 50.0, 0.0, 1.0, 0.0, 1.0},
                          {325.0, 200.0, 0.0, 1.0, 1.0, 0.0},
                          {400.0, 50.0, 0.0, 0.0, 1.0, 1.0},
                          {250.0, 150.0, 0.0, 1.0, 0.0, 0.0},
                          {400.0, 150.0, 0.0, 0.0, 1.0, 0.0}};

	glClearColor(0.0,0.0,0.0,0.0);
	startList = glGenLists(2);
	tobj = gluNewTess();
	gluTessCallback(tobj, GLU_TESS_VERTEX, &glVertex3dv);
	gluTessCallback(tobj, GLU_TESS_BEGIN, &beginCallback);
	gluTessCallback(tobj, GLU_TESS_END, &endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR, &errorCallback);
	
	// displaylist
	glNewList(startList,GL_COMPILE);
	glShadeModel(GL_FLAT);
	gluTessBeginPolygon(tobj,NULL);
		gluTessBeginContour(tobj);
			gluTessVertex(tobj, rect[0],rect[0]);
			gluTessVertex(tobj, rect[1],rect[1]);
			gluTessVertex(tobj, rect[2],rect[2]);
			gluTessVertex(tobj, rect[3],rect[3]);
		gluTessEndContour(tobj);
		
		gluTessBeginContour(tobj);
			 gluTessVertex(tobj, tri[0], tri[0]);
			 gluTessVertex(tobj, tri[1], tri[1]);
			 gluTessVertex(tobj, tri[2], tri[2]);
		gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);
	glEndList();
	
	gluTessCallback(tobj, GLU_TESS_VERTEX, &vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN, &beginCallback);
	gluTessCallback(tobj, GLU_TESS_END, &endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR,&errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE, &combineCallback);
	
	
	// smooth shaded self-intersection star
	glNewList(startList + 1, GL_COMPILE);
	glShadeModel(GL_SMOOTH);
	gluTessProperty(tobj,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_POSITIVE); // 正数
	//gluTessProperty(tobj,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_ABS_GEQ_TWO); 绘制环绕数大于等于2
	// gluTessProperty(tobj,GLU_TESS_BOUNDARY_ONLY,GL_TRUE); 使用线框模式绘制
	gluTessBeginPolygon(tobj,NULL);
	gluTessBeginContour(tobj);
		gluTessVertex(tobj,star[0],star[0]);
		gluTessVertex(tobj,star[1],star[1]);
		gluTessVertex(tobj,star[2],star[2]);
		gluTessVertex(tobj,star[3],star[3]);
		gluTessVertex(tobj,star[4],star[4]);
	gluTessEndContour(tobj);
	gluTessEndPolygon(tobj);
	glEndList();
	gluDeleteTess(tobj);
	
}


static void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)w,0.0,(GLdouble)h);
	
}

static void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
		exit(0);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500,500);
	glutCreateWindow("tess");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	return 0;
}