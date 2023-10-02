#include <iostream>

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

float yRot = 0.0f;
float xRot = 0.0f;
#define SUN 1
#define MOON 2
#define EARTH 3

void RenderScene();
void RenderSphere();
void ChangeSize(GLsizei w, GLsizei h);
void ProcessHit(int hits, GLuint* buf);
void TimerFunc(int value);


void init()
{
	if (glewInit()) {
		std::cerr << " Unable to init GLEW, exit..." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glutSolidSphere(1.0, 26, 26);
	RenderScene();
	glFlush();
}

void RenderSphere()
{
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -10.0f);

	glutSolidSphere(1.0, 26, 26);
	return;

	//初始化名称栈
	//glInitNames();
	//往栈顶压栈，压如一个名称
	glPushName(0);
	glColor3f(1.0f, 0.0f, 0.0f);
	//用当前名称SUN替换掉栈顶名称
	glLoadName(SUN);
	glutSolidSphere(1.0, 26, 26);

	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(2.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	//用EARTH替换掉栈顶名称
	glLoadName(EARTH);
	//glPushName(EARTH);
	glutSolidSphere(0.3, 26, 26);

	glTranslatef(1.0f, 0.0f, 0.0f);
	glColor3f(0.25f, 0.25f, 0.75f);
	//用当前名称MOON替换掉栈顶名称
	glLoadName(MOON);
	//glPushName(MOON);
	glutSolidSphere(0.1, 26, 26);
	glPopMatrix();
}
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderSphere();
	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	GLfloat fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TimerFunc(int value)
{
	yRot += 0.5;
	if (yRot > 360.0f)
	{
		yRot = 0.0f;
	}
	glutPostRedisplay();
	glutTimerFunc(50, TimerFunc, 1);
}

//处理点击记录
void ProcessHit(int hits, GLuint* buf)
{
	for (int i = 1; i <= hits; ++i)
	{
		GLuint nameNum = *buf;
		printf("hit number %d \n", i);
		printf("name stack count is %d\n", *buf); buf++;
		printf("min z value is %g\n", (float)*buf / 0x7FFFFFFF); buf++;
		printf("max z value is %g\n", (float)*buf / 0x7FFFFFFF); buf++;
		printf("name value is : ");
		for (int j = 0; j < nameNum; ++j)
		{
			switch (*buf)
			{
			case SUN:
				printf("SUN \t");
				break;
			case EARTH:
				printf("EARTH \t");
				break;
			case MOON:
				printf("MOON \t");
				break;
			default:
				break;
			}
			buf++;
		}
		printf("\n");
	}
}


//void ProcessSelection(int x, int y)
//{
//	GLint viewport[4], hits;
//
//	static GLuint selectBuffer[BUFFER_LENGTH];
//	//设置选择缓冲区
//	glSelectBuffer(BUFFER_LENGTH, selectBuffer);
//
//	//切换到投影矩阵，我们需要创建 可视区域
//	glMatrixMode(GL_PROJECTION);
//	//保留原先的 投影矩阵，以便恢复
//	glPushMatrix();
//	glLoadIdentity();
//	//获得视口
//	glGetIntegerv(GL_VIEWPORT, viewport);
//	//切换到选择模式
//	glRenderMode(GL_SELECT);
//	GLfloat aspect = (GLfloat)viewport[2] / (GLfloat)viewport[3];
//	//创建一个描述可视区域的矩阵
//	gluPickMatrix(x, viewport[3] - y + viewport[1], 2, 2, viewport);
//	//与投影矩阵相乘，得到可视区域
//	gluPerspective(35.0, aspect, 1.0, 200.0);
//	//在选择模式下 渲染图元
//	RenderSphere();
//	//返回点击记录数。
//	hits = glRenderMode(GL_RENDER);
//	ProcessHit(hits, selectBuffer);
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//
//	glMatrixMode(GL_MODELVIEW);
//}


//void draw(GLenum model = GL_RENDER)
//{
//	if (model == GL_SELECT)
//	{
//		glColor3f(1.0, 0.0, 0.0);
//		glLoadName(100);
//		glPushMatrix();
//		glTranslatef(-5, 0.0, 10.0);
//		glBegin(GL_QUADS);
//		glVertex3f(-1, -1, 0);
//		glVertex3f(1, -1, 0);
//		glVertex3f(1, 1, 0);
//		glVertex3f(-1, 1, 0);
//		glEnd();
//		glPopMatrix();
//
//		glColor3f(0.0, 0.0, 1.0);
//		glLoadName(101);
//		glPushMatrix();
//		glTranslatef(5, 0.0, -10.0);
//		glBegin(GL_QUADS);
//		glVertex3f(-1, -1, 0);
//		glVertex3f(1, -1, 0);
//		glVertex3f(1, 1, 0);
//		glVertex3f(-1, 1, 0);
//		glEnd();
//		glPopMatrix();
//	}
//	else
//	{
//		glColor3f(1.0, 0.0, 0.0);
//		glPushMatrix();
//		glTranslatef(-5, 0.0, -5.0);
//		glBegin(GL_QUADS);
//		glVertex3f(-1, -1, 0);
//		glVertex3f(1, -1, 0);
//		glVertex3f(1, 1, 0);
//		glVertex3f(-1, 1, 0);
//		glEnd();
//		glPopMatrix();
//
//		glColor3f(0.0, 0.0, 1.0);
//		glPushMatrix();
//		glTranslatef(5, 0.0, -10.0);
//		glBegin(GL_QUADS);
//		glVertex3f(-1, -1, 0);
//		glVertex3f(1, -1, 0);
//		glVertex3f(1, 1, 0);
//		glVertex3f(-1, 1, 0);
//		glEnd();
//		glPopMatrix();
//	}
//
//
//}
//void SelectObject(GLint x, GLint y)
//{
//	GLuint selectBuff[32] = { 0 };//创建一个保存选择结果的数组  
//	GLint hits, viewport[4];
//
//	glGetIntegerv(GL_VIEWPORT, viewport); //获得viewport  
//	glSelectBuffer(64, selectBuff); //告诉OpenGL初始化selectbuffer  
//	glRenderMode(GL_SELECT);    //进入选择模式  
//
//	glInitNames();  //初始化名字栈  
//	glPushName(0);  //在名字栈中放入一个初始化名字，这里为‘0’  
//
//
//	glMatrixMode(GL_PROJECTION);    //进入投影阶段准备拾取  
//	glPushMatrix();     //保存以前的投影矩阵  
//	glLoadIdentity();   //载入单位矩阵  
//
//	float m[16];
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//
//	gluPickMatrix(x,           // 设定我们选择框的大小，建立拾取矩阵，就是上面的公式  
//		viewport[3] - y,    // viewport[3]保存的是窗口的高度，窗口坐标转换为OpenGL坐标  
//		2, 2,              // 选择框的大小为2，2  
//		viewport          // 视口信息，包括视口的起始位置和大小  
//	);
//
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//	glOrtho(-10, 10, -10, 10, -10, 10);     //拾取矩阵乘以投影矩阵，这样就可以让选择框放大为和视体一样大  
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//
//	draw(GL_SELECT);    // 该函数中渲染物体，并且给物体设定名字  
//
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();  // 返回正常的投影变换  
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//	hits = glRenderMode(GL_RENDER); // 从选择模式返回正常模式,该函数返回选择到对象的个数  
//	if (hits > 0)
//		processSelect(selectBuff);  //  选择结果处理  
//}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	//glutInitContextVersion(3, 3); // 使用opengl4.3版本
	//glutInitContextProfile(GLUT_CORE_PROFILE); // 使用核心模式
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("1-triangles");
	init();
	glutDisplayFunc(display);
	 
	glutMainLoop();
	return 0;
}