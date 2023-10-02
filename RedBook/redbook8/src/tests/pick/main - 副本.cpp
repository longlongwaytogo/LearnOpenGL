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

	//��ʼ������ջ
	//glInitNames();
	//��ջ��ѹջ��ѹ��һ������
	glPushName(0);
	glColor3f(1.0f, 0.0f, 0.0f);
	//�õ�ǰ����SUN�滻��ջ������
	glLoadName(SUN);
	glutSolidSphere(1.0, 26, 26);

	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(2.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	//��EARTH�滻��ջ������
	glLoadName(EARTH);
	//glPushName(EARTH);
	glutSolidSphere(0.3, 26, 26);

	glTranslatef(1.0f, 0.0f, 0.0f);
	glColor3f(0.25f, 0.25f, 0.75f);
	//�õ�ǰ����MOON�滻��ջ������
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

//��������¼
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
//	//����ѡ�񻺳���
//	glSelectBuffer(BUFFER_LENGTH, selectBuffer);
//
//	//�л���ͶӰ����������Ҫ���� ��������
//	glMatrixMode(GL_PROJECTION);
//	//����ԭ�ȵ� ͶӰ�����Ա�ָ�
//	glPushMatrix();
//	glLoadIdentity();
//	//����ӿ�
//	glGetIntegerv(GL_VIEWPORT, viewport);
//	//�л���ѡ��ģʽ
//	glRenderMode(GL_SELECT);
//	GLfloat aspect = (GLfloat)viewport[2] / (GLfloat)viewport[3];
//	//����һ��������������ľ���
//	gluPickMatrix(x, viewport[3] - y + viewport[1], 2, 2, viewport);
//	//��ͶӰ������ˣ��õ���������
//	gluPerspective(35.0, aspect, 1.0, 200.0);
//	//��ѡ��ģʽ�� ��ȾͼԪ
//	RenderSphere();
//	//���ص����¼����
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
//	GLuint selectBuff[32] = { 0 };//����һ������ѡ����������  
//	GLint hits, viewport[4];
//
//	glGetIntegerv(GL_VIEWPORT, viewport); //���viewport  
//	glSelectBuffer(64, selectBuff); //����OpenGL��ʼ��selectbuffer  
//	glRenderMode(GL_SELECT);    //����ѡ��ģʽ  
//
//	glInitNames();  //��ʼ������ջ  
//	glPushName(0);  //������ջ�з���һ����ʼ�����֣�����Ϊ��0��  
//
//
//	glMatrixMode(GL_PROJECTION);    //����ͶӰ�׶�׼��ʰȡ  
//	glPushMatrix();     //������ǰ��ͶӰ����  
//	glLoadIdentity();   //���뵥λ����  
//
//	float m[16];
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//
//	gluPickMatrix(x,           // �趨����ѡ���Ĵ�С������ʰȡ���󣬾�������Ĺ�ʽ  
//		viewport[3] - y,    // viewport[3]������Ǵ��ڵĸ߶ȣ���������ת��ΪOpenGL����  
//		2, 2,              // ѡ���Ĵ�СΪ2��2  
//		viewport          // �ӿ���Ϣ�������ӿڵ���ʼλ�úʹ�С  
//	);
//
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//	glOrtho(-10, 10, -10, 10, -10, 10);     //ʰȡ�������ͶӰ���������Ϳ�����ѡ���Ŵ�Ϊ������һ����  
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//
//	draw(GL_SELECT);    // �ú�������Ⱦ���壬���Ҹ������趨����  
//
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();  // ����������ͶӰ�任  
//	glGetFloatv(GL_PROJECTION_MATRIX, m);
//	hits = glRenderMode(GL_RENDER); // ��ѡ��ģʽ��������ģʽ,�ú�������ѡ�񵽶���ĸ���  
//	if (hits > 0)
//		processSelect(selectBuff);  //  ѡ��������  
//}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	//glutInitContextVersion(3, 3); // ʹ��opengl4.3�汾
	//glutInitContextProfile(GLUT_CORE_PROFILE); // ʹ�ú���ģʽ
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("1-triangles");
	init();
	glutDisplayFunc(display);
	 
	glutMainLoop();
	return 0;
}