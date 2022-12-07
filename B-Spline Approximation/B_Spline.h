#pragma once
#include <string>
#define Maxh 10
#define Maxn 50

using namespace std;

class B_Spline
{
public:
	// ������p+1���ǽ���
	long p = 0;
	// ���ƶ�����-1��h+1���ǿ��ƶ�����
	long h = 0;
	// �������ݵ���-1��n+1���Ǹ������ݵ���
	long n = 0;
	// ���ƶ��㣬�ڶ�ά[0]��x���꣬[1]��y����
	double P[Maxh][2];
	// �������ݵ㣬�ڶ�ά[0]��x���꣬[1]��y����
	double D[Maxn][2];
	// �������ݵ��������ж�Ӧ�Ĳ���t
	double t[Maxn];
	// ����ģʽ��0 for algebraic; 1 for geometric
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