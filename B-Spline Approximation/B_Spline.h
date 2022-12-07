#pragma once
#include <string>
#define Maxh 10
#define Maxn 50

using namespace std;

class B_Spline
{
public:
	// 次数；p+1才是阶数
	long p = 0;
	// 控制顶点数-1；h+1才是控制顶点数
	long h = 0;
	// 给定数据点数-1；n+1才是给定数据点数
	long n = 0;
	// 控制顶点，第二维[0]是x坐标，[1]是y坐标
	double P[Maxh][2];
	// 给定数据点，第二维[0]是x坐标，[1]是y坐标
	double D[Maxn][2];
	// 给定数据点在曲线中对应的参数t
	double t[Maxn];
	// 距离模式；0 for algebraic; 1 for geometric
	bool isGeo = 0;
	
	void init();
	void calcP();
	void drawCurve();
	void drawBand();
	void genParas(string mode);
	void regenParas();
	double castPara(long i);
	double DeBoor_Cox(long i, long k, double t);
	bool isLegal();
};