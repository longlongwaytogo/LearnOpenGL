#include <Windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//https://blog.csdn.net/zsc09_leaf/article/details/6785472?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-6785472-blog-12954507.235%5Ev38%5Epc_relevant_default_base3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-6785472-blog-12954507.235%5Ev38%5Epc_relevant_default_base3&utm_relevant_index=2

void draw(GLenum model);
void SelectObject(GLint x, GLint y);

void showAllHitsInfo(GLint hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint names, * ptr;
    printf("-------------------------------------------\n");
    printf("hits = %d\n", hits);
    ptr = (GLuint*)buffer;
    for (i = 0; i < hits; i++) { /*  for each hit  */
        names = *ptr;
        printf(" number of names for hit = %d\n", names); ptr++;
        printf("  z1 is %g;", (float)*ptr / 0x7fffffff); ptr++;
        printf(" z2 is %g\n", (float)*ptr / 0x7fffffff); ptr++;
        printf("   the name is ");
        for (j = 0; j < names; j++) {     /*  for each name */
            printf("%d ", *ptr); ptr++;
        }
        printf("\n");
    }
    printf("--------------------------------------\n");
}

void processHits(GLint hits, GLuint buffer[])
{
    showAllHitsInfo(hits, buffer);

    unsigned int i, j;
    GLuint names, * ptr, minZ, * ptrNames, numberOfNames;

    printf("hits = %d\n", hits);
    ptr = (GLuint*)buffer;
    minZ = 0xffffffff;
    for (i = 0; i < hits; i++) {
        names = *ptr;
        ptr++;
        if (*ptr < minZ) {
            numberOfNames = names;
            minZ = *ptr;
            ptrNames = ptr + 2;
        }

        ptr += names + 2;
    }
    printf("The closest hit names are ");
    ptr = ptrNames;
    for (j = 0; j < numberOfNames; j++, ptr++) {
        printf("%d ", *ptr);
    }
    printf("\n");

}

void SelectObject(GLint x, GLint y)
{
    GLuint selectBuff[32] = { 0 };//����һ������ѡ����������  
    GLint hits, viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport); //���viewport  
    glSelectBuffer(64, selectBuff); //����OpenGL��ʼ��  selectbuffer  
    glRenderMode(GL_SELECT);    //����ѡ��ģʽ  

    glInitNames();  //��ʼ������ջ  
    glPushName(0);  //������ջ�з���һ����ʼ�����֣�����Ϊ��0��  
    glMatrixMode(GL_PROJECTION);    //����ͶӰ�׶�׼��ʰȡ  
    glPushMatrix();     //������ǰ��ͶӰ����  
    glLoadIdentity();   //���뵥λ����  

    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    gluPickMatrix(x,            // �趨����ѡ���Ĵ�С������ʰȡ���󣬾�������Ĺ�ʽ 
        viewport[3]-y,          // viewport[3]������Ǵ��ڵĸ߶ȣ���������ת��ΪOpenGL����  
        2, 2,                   // ѡ���Ĵ�СΪ2��2  
        viewport                // �ӿ���Ϣ�������ӿڵ���ʼλ�úʹ�С  

    );

    glGetFloatv(GL_PROJECTION_MATRIX, m);
    glOrtho(-10, 10, -10, 10, -10, 10);     //ʰȡ�������ͶӰ���������Ϳ�����ѡ���Ŵ�Ϊ������һ����  
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    draw(GL_SELECT);    // �ú�������Ⱦ���壬���Ҹ������趨����  
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // ����������ͶӰ�任  
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    hits = glRenderMode(GL_RENDER); // ��ѡ��ģʽ��������ģʽ,�ú�������ѡ�񵽶���ĸ���  
    if (hits > 0)
        processHits(hits,selectBuff);  //  ѡ��������  
}

void draw(GLenum model = GL_RENDER)
{
    if (model == GL_SELECT)
    {
       glColor3f(1.0, 0.0, 0.0);
        glLoadName(100);
        glPushMatrix();
            glTranslatef(-5, 0.0, 10.0);
            glBegin(GL_QUADS);
                glVertex3f(-1, -1, 0);
                glVertex3f(1, -1, 0);
                glVertex3f(1, 1, 0);
                glVertex3f(-1, 1, 0);
            glEnd();
        glPopMatrix();


        glColor3f(0.5, 0.0, 1.0);
        glLoadName(45);
        glPushMatrix();
         glTranslatef(-4, 0.0, 10.0);
            glBegin(GL_QUADS);
                glVertex3f(-1, -1, 0);
                glVertex3f(1, -1, 0);
                glVertex3f(1, 1, 0);
                glVertex3f(-1, 1, 0);
            glEnd();
        glPopMatrix();

        glColor3f(0.0, 0.0, 1.0);
        glLoadName(101);
        glPushMatrix();
        glTranslatef(5, 0.0, -9.0);
        glBegin(GL_QUADS);
            glVertex3f(-1, -1, 0);
            glVertex3f(1, -1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();
    }
    else
    {
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(-5, 0.0, -5.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.5, 0.0, 1.0);
        glTranslatef(-4, 0.0, 10.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();

      
        glColor3f(0, 0, 1.0);
        glPushMatrix();
        glTranslatef(5, 0.0, -9.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f(1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();
    }
}

void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(4.0, 1024.0 / 768.0, 0.5, 1000);
    glOrtho(-10, 10, -10, 10, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   // gluLookAt(0,0,10,0,0,-1, 0.0, 1.0, 0.0);
    draw(GL_RENDER);
    //SelectObject();

   // glutWireCube(1);
    glFlush();
}

void mouseEvent(int button, int state, int x, int y)
{
    // state
   // GLUT_DOWN
    //    GLUT_UP
    if (state == GLUT_DOWN) {
        SelectObject(x, y);
    }
  
    std::cout << "mouse down:" << x << "," << y << std::endl;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseEvent);
    glutMainLoop();
    return 0;
}