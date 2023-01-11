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

// 输入像素坐标，计算世界坐标；前提：fovy=90
double coordinate_trans_x(long x) { return (2.0 * x / windowSizeX - 1) * cameraZ; }
double coordinate_trans_y(long y) { return (1 - 2.0 * y / windowSizeY) * cameraZ; }

// 计算两点距离的平方
double Dist2(double x1, double y1, double x2, double y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }

// 绘制顶点和曲线
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
	
	// 用绿色方形画出控制顶点
	glPointSize(pointSize);
	glDisable(GL_POINT_SMOOTH);
	glColor3f(180.0 / 255, 215.0 / 255, 141.0 / 255);
	glBegin(GL_POINTS);
	for (int i = 0; i <= drawn_B_Spline_pointer->h; i++)
		glVertex2f(drawn_B_Spline_pointer->P[i][0], drawn_B_Spline_pointer->P[i][1]);
	glEnd();
	// 用蓝色方形画出给定数据点
	glColor3f(0.0, 152.0 / 255, 158.0 / 255);
	glBegin(GL_POINTS);
	for (int i = 0; i <= drawn_B_Spline_pointer->n; i++)
		glVertex2f(drawn_B_Spline_pointer->D[i][0], drawn_B_Spline_pointer->D[i][1]);
	glEnd();
	// 用红色粗线画出计算出的B-Spline
	glLineWidth(3.0);
	glColor3f(216.0 / 255, 34.0 / 255, 13.0 / 255);
	drawn_B_Spline_pointer->calcP();
	drawn_B_Spline_pointer->drawCurve();
	// 用橙色细线画出给定数据点与曲线上参数对应点的连线
	glEnable(GL_POINT_SMOOTH);
	glLineWidth(1.0);
	glColor3f(246.0 / 255, 172.0 / 255, 26.0 / 255);
	drawn_B_Spline_pointer->drawBand();

    glFlush();
    glutSwapBuffers();
}

// 滚轮控制缩放
void MouseWheel(int button, int dir, int x, int y)
{
	// 放大
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
	// 缩小
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

// 判断鼠标点击状态
void MouseClick(int button, int state, int x, int y)
{
	// 左键控制顶点
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			mouseLeftDown = true;
		else if (state == GLUT_UP)
			mouseLeftDown = false;
}

// 鼠标移动
void MouseMove(int x, int y)
{
	GLfloat transX = coordinate_trans_x(x);
	GLfloat transY = coordinate_trans_y(y);
	GLfloat transPointSize = coordinate_trans_x(pointSize + windowSizeX / 2.0);
	// 左键控制顶点
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

// 图形操作主程序
void glMain(int argc, char** argv, B_Spline* B_Spline_pointer)
{
    drawn_B_Spline_pointer = B_Spline_pointer;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(windowSizeX, windowSizeY);
    glutCreateWindow("B-Spline Approximation");
	glClearColor(150.0 / 255, 150.0 / 255, 150.0 / 255, 0);
	
	// 设定视角；后续可用鼠标交互修改视角
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0, 1000);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);

	glutMouseWheelFunc(MouseWheel);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);

	// 绘制
    glutDisplayFunc(display);
    
    glutMainLoop();
}