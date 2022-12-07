#include "procedure.h"

// 矩阵收紧，删去外围没有数据的空间
double** matTighten(double* M1, long a, long b, long cols)
{
	double* data = new double[a * b];
	double** M = new double* [a];
	for (int i = 0; i < a; i++)
		M[i] = data + i * b;
	for (int i = 0; i < a; i++)
		for (int j = 0; j < b; j++)
			M[i][j] = M1[i * cols + j];
	return M;
}

// 矩阵乘法
double** matMulti(double* M1, double* M2, long a, long b, long c)
{
	double* data = new double[a * c];
	double** M = new double* [a];
	for (int i = 0; i < a; i++)
		M[i] = data + i * c;
	for (int i = 0; i < a; i++)
		for (int j = 0; j < c; j++)
			M[i][j] = 0;
	for (int i = 0; i < a; i++)
		for (int j = 0; j < c; j++)
			for (int k = 0; k < b; k++)
				M[i][j] += M1[i * b + k] * M2[k * c + j];
	return M;
}

// 矩阵转置
double** matTrans(double* M1, long a, long b)
{
	double* data = new double[b * a];
	double** M = new double* [b];
	for (int i = 0; i < b; i++)
		M[i] = data + i * a;
	for (int i = 0; i < b; i++)
		for (int j = 0; j < a; j++)
			M[i][j] = M1[j * b + i];
	return M;
}

// 矩阵求逆
double** matInverse(double* M1, long a)
{
	double* dataLU = new double[a * a];
	double* dataL = new double[a * a];
	double* dataU = new double[a * a];
	double** LU = new double* [a];
	double** InvL = new double* [a];
	double** InvU = new double* [a];
	for (int i = 0; i < a; i++)
	{
		LU[i] = dataLU + i * a;
		InvL[i] = dataL + i * a;
		InvU[i] = dataU + i * a;
		for (int j = 0; j < a; j++)
		{
			InvL[i][j] = 0;
			InvU[i][j] = 0;
		}
	}
	// 生成LU矩阵的第i行/列
	for (int i = 0; i < a; i++)
	{
		// 生成第i行
		for (int j = i; j < a; j++)
		{
			LU[i][j] = M1[i * a + j];
			for (int k = 0; k < i; k++)
				LU[i][j] -= LU[k][j] * LU[i][k];
		}
		// 生成第i列
		for (int j = i + 1; j < a; j++)
		{
			LU[j][i] = M1[j * a + i];
			for (int k = 0; k < i; k++)
				LU[j][i] -= LU[j][k] * LU[k][i];
			LU[j][i] /= LU[i][i];
		}
	}
	// 计算L的逆矩阵
	for (int j = 0; j < a; j++)
	{
		InvL[j][j] = 1;
		for (int i = j + 1; i < a; i++)
			for (int k = j; k < i; k++)
				InvL[i][j] -= LU[i][k] * InvL[k][j];
	}
	// 计算U的逆矩阵
	for (int j = 0; j < a; j++)
	{
		InvU[j][j] = 1.0 / LU[j][j];
		for (int i = j - 1; i >= 0; i--)
			for (int k = i + 1; k <= j; k++)
				InvU[i][j] -= LU[i][k] * InvU[k][j] / LU[i][i];
	}
	return matMulti(&InvU[0][0], &InvL[0][0], a, a, a);
}