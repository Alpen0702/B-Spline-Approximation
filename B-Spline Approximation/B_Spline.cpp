#include "B_Spline.h"
#include "procedure.h"
#include <GL/freeglut.h>
#include <cmath>
#include <string>
#define M_PI 3.14159265358979323846

double dist(double x1, double y1, double x2, double y2) { return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)); }
double dist2(double x1, double y1, double x2, double y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }

// 求基函数Ni,k的值
double B_Spline::DeBoor_Cox(long i, long k, double t)
{
	if (k == 0)
		if (t >= castPara(i) && t < castPara(i + 1))
			return 1;
		else
			return 0;
	double coef1 = castPara(i + k) - castPara(i);
	if (abs(coef1) > 0.00001)
		coef1 = (t - castPara(i)) / coef1;
	double coef2 = castPara(i + k + 1) - castPara(i + 1);
	if (abs(coef2) > 0.00001)
		coef2 = (castPara(i + k + 1) - t) / coef2;
	return coef1 * DeBoor_Cox(i, k - 1, t) + coef2 * DeBoor_Cox(i + 1, k - 1, t);
}

// 把[0,h+p+1]的节点投射到[p,h+1]上
double B_Spline::castPara(long i)
{
	if (i <= p)
		return p;
	if (i > h)
		return h + 1;
	return i; 
}

// 画曲线
void B_Spline::drawCurve()
{
    // 参数t每变化一个单位就画出freq个点
	long freq = 200;
	double step = 1.0f / freq;

	glBegin(GL_LINE_STRIP);
	// t的范围是p到h+1，步长是1/freq
	for (double t = p; t < h+1; t += step)
	{
		double x = 0, y = 0;
		// 根据递推公式，对控制顶点加权得到曲线上参数为t的点的位置
		for (int i = 0; i <= h; i++)
		{
			double coef = DeBoor_Cox(i, p, t);
			x += coef * P[i][0];
			y += coef * P[i][1];
		}
		glVertex2f(x, y);
	}
    glEnd();
}

// 画给定数据点的参数对应点
void B_Spline::drawBand()
{
	for (int i = 0; i < n; i++)
	{
		// 画出给定数据点在曲线上的参数对应点
		glBegin(GL_POINTS);
		double x = 0, y = 0;
		for (int j = 0; j <= h; j++)
		{
			double coef = DeBoor_Cox(j, p, t[i]);
			x += coef * P[j][0];
			y += coef * P[j][1];
		}
		glVertex2f(x, y);
		glEnd();
		// 画出给定数据点与曲线上参数对应点的连线
		glBegin(GL_LINE_STRIP);
		glVertex2f(x, y);
		glVertex2f(D[i][0], D[i][1]);
		glEnd();
	}
	// 曲线的末端点是一个例外，因为它已经不在曲线定义域内，故需要额外绘制
	glBegin(GL_POINTS);
	double x = D[n][0], y = D[n][1];
	glVertex2f(x, y);
	glEnd();
}

// 曲线初始化
void B_Spline::init()
{
	double radius = 0.8;
	double delta = 2.0 * M_PI / (n + 1);
	double theta = 0;
	for (int i = 0; i <= n + 1; i++)
	{
		D[i][0] = radius * sin(theta);
		D[i][1] = radius * cos(theta);
		theta += delta;
	}	
	calcP();
}

// 计算控制顶点
void B_Spline::calcP()
{
	genParas("centripetal");
	double Q[Maxn][2];
	double Qk[Maxn][2];
	double tmpN[Maxn][Maxn];
	double** M;
	double** tmpP;
	bool done = 0;
	double thisDist2Sum = 0;
	double lastDist2Sum = 0;
	long iter = 0;
	P[0][0] = D[0][0];
	P[0][1] = D[0][1];
	P[h][0] = D[n][0];
	P[h][1] = D[n][1];

	while (!done)
	{
		for (int k = 1; k < n; k++)
		{
			double coef1 = DeBoor_Cox(0, p, t[k]);
			double coef2 = DeBoor_Cox(h, p, t[k]);
			Qk[k][0] = D[k][0] - coef1 * D[0][0] - coef2 * D[n][0];
			Qk[k][1] = D[k][1] - coef1 * D[0][1] - coef2 * D[n][1];
		}
		// Q是[h-1]*[2]矩阵
		for (int i = 1; i < h; i++)
		{
			Q[i - 1][0] = 0;
			Q[i - 1][1] = 0;
			for (int k = 1; k < n; k++)
			{
				double coef = DeBoor_Cox(i, p, t[k]);
				Q[i - 1][0] += coef * Qk[k][0];
				Q[i - 1][1] += coef * Qk[k][1];
			}
		}
		// N是[n-1]*[h-1]矩阵
		for (int k = 1; k < n; k++)
			for (int i = 1; i < h; i++)
				tmpN[k - 1][i - 1] = DeBoor_Cox(i, p, t[k]);
		double** N = matTighten(&tmpN[0][0], n - 1, h - 1, Maxn);
		// M是[h-1]*[h-1]矩阵
		M = matMulti(&matTrans(&N[0][0], n - 1, h - 1)[0][0], &N[0][0], h - 1, n - 1, h - 1);
		// P是[h-1]*[2]矩阵
		tmpP = matMulti(&matInverse(&M[0][0], h - 1)[0][0], &Q[0][0], h - 1, h - 1, 2);
		for (int i = 1; i < h; i++)
		{
			P[i][0] = tmpP[i - 1][0];
			P[i][1] = tmpP[i - 1][1];
		}
		
		// mode = 'algebraic'
		if (!isGeo)
			done = 1;
		// mode = 'geometric'
		else
		{
			lastDist2Sum = thisDist2Sum;
			thisDist2Sum = 0;
			for (int i = 1; i < n; i++)
			{
				double x = 0, y = 0;
				for (int j = 0; j <= h; j++)
				{
					double coef = DeBoor_Cox(j, p, t[i]);
					x += coef * P[j][0];
					y += coef * P[j][1];
				}
				thisDist2Sum += dist2(x, y, D[i][0], D[i][1]);
			}
			// 若前后两次距离差收敛，就结束迭代
			if (abs(lastDist2Sum - thisDist2Sum) < 0.0001)
				done = 1;
			else
				regenParas();
			iter++;
			if (iter > 100)
				break;
		}
	}
}

// 根据给定数据点计算对应的参数t
void B_Spline::genParas(string mode)
{
	if (mode == "centripetal")
	{
		double alpha = 0.5;
		t[0] = 0;
		for (int i = 1; i <= n; i++)
			t[i] = t[i-1] + pow(dist(D[i][0], D[i][1], D[i - 1][0], D[i - 1][1]), alpha);
		for (int i = 0; i <= n; i++)
			t[i] = t[i] / t[n] * (h - p + 1) + p;
	}
}

// 依据最小几何距离重新计算参数t
void B_Spline::regenParas()
{
	double step = 0.000001;
	double threshold = 0.001;
	double x = 0, y = 0, y0, y1, y2;
	
	for (int i = 1; i < n; i++)
	{
		long iter = 0;
		while (1)
		{
			// 零阶导数
			x = 0, y = 0;
			for (int j = 0; j <= h; j++)
			{
				double coef = DeBoor_Cox(j, p, t[i]);
				x += coef * P[j][0];
				y += coef * P[j][1];
			}
			y0 = dist2(x, y, D[i][0], D[i][1]);

			// 一阶导数
			x = 0, y = 0;
			for (int j = 0; j <= h; j++)
			{
				double coef = DeBoor_Cox(j, p, t[i] + step);
				x += coef * P[j][0];
				y += coef * P[j][1];
			}
			y1 = dist2(x, y, D[i][0], D[i][1]);
			if (abs(y1 - y0) < threshold * step)
				break;

			// 二阶导数
			x = 0, y = 0;
			for (int j = 0; j <= h; j++)
			{
				double coef = DeBoor_Cox(j, p, t[i] + 2 * step);
				x += coef * P[j][0];
				y += coef * P[j][1];
			}
			y2 = dist2(x, y, D[i][0], D[i][1]);

			// 牛顿法求最小距离点
			t[i] -= (y1 - y0) * step / (y2 - 2 * y1 + y0);
			if (t[i] > h + 1)
				t[i] = h + 1 - 0.001;
			if (t[i] < p)
				t[i] = p + 0.001;
			iter++;
			if (iter > 100)
				break;
		}
	}
}

// 判断输入数值是否合法
bool B_Spline::isLegal()
{
	if (n > h && h >= p && p >= 1 && n < 50 && h < 10)
		return true;
	return false;
}
