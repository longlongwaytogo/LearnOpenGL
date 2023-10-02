#include <Windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2,
    GLfloat y2, GLfloat x3, GLfloat y3, GLfloat z)
{
    glBegin(GL_TRIANGLES);
    glVertex3f(x1, y1, z);
    glVertex3f(x2, y2, z);
    glVertex3f(x3, y3, z);
    glEnd();
}

void drawViewVolume(GLfloat x1, GLfloat x2, GLfloat y1,
    GLfloat y2, GLfloat z1, GLfloat z2)
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x1, y1, -z1);
    glVertex3f(x2, y1, -z1);
    glVertex3f(x2, y2, -z1);
    glVertex3f(x1, y2, -z1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(x1, y1, -z2);
    glVertex3f(x2, y1, -z2);
    glVertex3f(x2, y2, -z2);
    glVertex3f(x1, y2, -z2);
    glEnd();

    glBegin(GL_LINES);  /*  4 lines     */
    glVertex3f(x1, y1, -z1);
    glVertex3f(x1, y1, -z2);
    glVertex3f(x1, y2, -z1);
    glVertex3f(x1, y2, -z2);
    glVertex3f(x2, y1, -z1);
    glVertex3f(x2, y1, -z2);
    glVertex3f(x2, y2, -z1);
    glVertex3f(x2, y2, -z2);
    glEnd();
}

void drawScene(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 4.0 / 3.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(7.5, 7.5, 12.5, 2.5, 2.5, -5.0, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);   /*  green triangle      */
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -5.0);

    glPushMatrix();
    glTranslatef(-0.2, 0.1, 0);
    glColor3f(0, 0, 1.0);
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -5.0);
    glPopMatrix();

    glColor3f(1.0, 0.0, 0.0);   /*  red triangle        */
    drawTriangle(2.0, 7.0, 3.0, 7.0, 2.5, 8.0, -5.0);
    glColor3f(1.0, 1.0, 0.0);   /*  yellow triangles    */
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, 0.0);
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -10.0);
    drawViewVolume(0.0, 5.0, 0.0, 5.0, 0.0, 10.0);
}

void processHits(GLint hits, GLuint buffer[])
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
}

#define BUFSIZE 512

void selectObjects(void)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;

    glSelectBuffer(BUFSIZE, selectBuf);
    (void)glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 5.0, 0.0, 5.0, 0.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadName(1);
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -5.0);
    glLoadName(21);
    glPushMatrix();
    glTranslatef(-0.2, 0.1, 0);
    glColor3f(0, 0, 1.0);
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -5.0);
    glPopMatrix();

    glLoadName(2);
    drawTriangle(2.0, 7.0, 3.0, 7.0, 2.5, 8.0, -5.0);
    glLoadName(3);
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, 0.0);
    drawTriangle(2.0, 2.0, 3.0, 2.0, 2.5, 3.0, -10.0);
    glPopMatrix();
    glFlush();

    hits = glRenderMode(GL_RENDER);
    processHits(hits, selectBuf);
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
    drawScene();
    selectObjects();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(200, 200);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}