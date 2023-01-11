#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include "B_Spline.h"
#include "procedure.h"
#include <cstdio>

B_Spline* drawn_B_Spline_pointer;
GLint windowSizeX = 720;
GLint windowSizeY = 720;
GLfloat cameraX = 0.0;
GLfloat cameraY = 0.0;
GLfloat cameraZ = 1.0;
GLfloat pointSize = 10.0;
GLboolean mouseLeftDown = GL_FALSE;

// �����������꣬�����������ꣻǰ�᣺fovy=90
double coordinate_trans_x(long x) { return (2.0 * x / windowSizeX - 1) * cameraZ; }
double coordinate_trans_y(long y) { return (1 - 2.0 * y / windowSizeY) * cameraZ; }

// ������������ƽ��
double Dist2(double x1, double y1, double x2, double y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }

// ���ƶ��������
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
	
	// ����ɫ���λ������ƶ���
	glPointSize(pointSize);
	glDisable(GL_POINT_SMOOTH);
	glColor3f(180.0 / 255, 215.0 / 255, 141.0 / 255);
	glBegin(GL_POINTS);
	for (int i = 0; i <= drawn_B_Spline_pointer->h; i++)
		glVertex2f(drawn_B_Spline_pointer->P[i][0], drawn_B_Spline_pointer->P[i][1]);
	glEnd();
	// ����ɫ���λ����������ݵ�
	glColor3f(0.0, 152.0 / 255, 158.0 / 255);
	glBegin(GL_POINTS);
	for (int i = 0; i <= drawn_B_Spline_pointer->n; i++)
		glVertex2f(drawn_B_Spline_pointer->D[i][0], drawn_B_Spline_pointer->D[i][1]);
	glEnd();
	// �ú�ɫ���߻����������B-Spline
	glLineWidth(3.0);
	glColor3f(216.0 / 255, 34.0 / 255, 13.0 / 255);
	drawn_B_Spline_pointer->calcP();
	drawn_B_Spline_pointer->drawCurve();
	// �ó�ɫϸ�߻����������ݵ��������ϲ�����Ӧ�������
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(1.0);
	glColor3f(246.0 / 255, 172.0 / 255, 26.0 / 255);
	drawn_B_Spline_pointer->drawBand();

    glFlush();
    glutSwapBuffers();
}

// ���ֿ�������
void MouseWheel(int button, int dir, int x, int y)
{
	// �Ŵ�
	if (dir > 0)
	{
		cameraX *= 0.8;
		cameraY *= 0.8;
		cameraZ *= 0.8;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
	}
	// ��С
	else
	{
		cameraX *= 1.25;
		cameraY *= 1.25;
		cameraZ *= 1.25;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
		glutPostRedisplay();
	}
}

// �ж������״̬
void MouseClick(int button, int state, int x, int y)
{
	// ������ƶ���
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			mouseLeftDown = true;
		else if (state == GLUT_UP)
			mouseLeftDown = false;
}

// ����ƶ�
void MouseMove(int x, int y)
{
	GLfloat transX = coordinate_trans_x(x);
	GLfloat transY = coordinate_trans_y(y);
	GLfloat transPointSize = coordinate_trans_x(pointSize + windowSizeX / 2.0);
	// ������ƶ���
	if (mouseLeftDown)
		for (int i = 0; i <= drawn_B_Spline_pointer->n; i++)
			if (Dist2(transX, transY, drawn_B_Spline_pointer->D[i][0], drawn_B_Spline_pointer->D[i][1]) < transPointSize * transPointSize)
			{
				drawn_B_Spline_pointer->D[i][0] = transX;
				drawn_B_Spline_pointer->D[i][1] = transY;
				break;
			}
	glutPostRedisplay();
}

// ͼ�β���������
void glMain(int argc, char** argv, B_Spline* B_Spline_pointer)
{
    drawn_B_Spline_pointer = B_Spline_pointer;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(windowSizeX, windowSizeY);
    glutCreateWindow("B-Spline Approximation");
	glClearColor(150.0 / 255, 150.0 / 255, 150.0 / 255, 0);
	
	// �趨�ӽǣ�����������꽻���޸��ӽ�
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0, 1000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);

	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);

	// ����
    glutDisplayFunc(display);
    
    glutMainLoop();
}