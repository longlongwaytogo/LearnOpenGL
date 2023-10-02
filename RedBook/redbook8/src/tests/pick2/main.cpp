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
    GLuint selectBuff[32] = { 0 };//创建一个保存选择结果的数组  
    GLint hits, viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport); //获得viewport  
    glSelectBuffer(64, selectBuff); //告诉OpenGL初始化  selectbuffer  
    glRenderMode(GL_SELECT);    //进入选择模式  

    glInitNames();  //初始化名字栈  
    glPushName(0);  //在名字栈中放入一个初始化名字，这里为‘0’  
    glMatrixMode(GL_PROJECTION);    //进入投影阶段准备拾取  
    glPushMatrix();     //保存以前的投影矩阵  
    glLoadIdentity();   //载入单位矩阵  

    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    gluPickMatrix(x,            // 设定我们选择框的大小，建立拾取矩阵，就是上面的公式 
        viewport[3]-y,          // viewport[3]保存的是窗口的高度，窗口坐标转换为OpenGL坐标  
        2, 2,                   // 选择框的大小为2，2  
        viewport                // 视口信息，包括视口的起始位置和大小  

    );

    glGetFloatv(GL_PROJECTION_MATRIX, m);
    glOrtho(-10, 10, -10, 10, -10, 10);     //拾取矩阵乘以投影矩阵，这样就可以让选择框放大为和视体一样大  
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    draw(GL_SELECT);    // 该函数中渲染物体，并且给物体设定名字  
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // 返回正常的投影变换  
    glGetFloatv(GL_PROJECTION_MATRIX, m);
    hits = glRenderMode(GL_RENDER); // 从选择模式返回正常模式,该函数返回选择到对象的个数  
    if (hits > 0)
        processHits(hits,selectBuff);  //  选择结果处理  
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