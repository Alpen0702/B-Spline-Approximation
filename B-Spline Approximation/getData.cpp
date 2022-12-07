#define _CRT_SECURE_NO_WARNINGS
#include "procedure.h"
#include "B_Spline.h"
#include <cstdio>
#include <cstdlib>

B_Spline* getData()
{
	B_Spline* B_Spline_Pointer = new B_Spline;
	while (1)
	{
		// 输入控制顶点数-1
		printf("Please type in h (sum of the control points - 1). Press Enter to continue.\n");
		scanf("%d", &B_Spline_Pointer->h);
		printf("Got h = %d\n", B_Spline_Pointer->h);
		// 输入次数（阶数-1）
		printf("Please type in p (degree of the B-Spline). Press Enter to continue.\n");
		scanf("%d", &B_Spline_Pointer->p);
		printf("Got p = %d\n", B_Spline_Pointer->p);
		// 输入给定数据点数-1
		printf("Please type in n (sum of the given data points - 1). Press Enter to continue.\n");
		scanf("%d", &B_Spline_Pointer->n);
		printf("Got n = %d\n", B_Spline_Pointer->n);
		// 选择模式
		printf("Please choose distance mode. Type in '0' for algebraic or '1' for geometric.\n");
		scanf("%d", &B_Spline_Pointer->isGeo);
		if (B_Spline_Pointer->isGeo)
			printf("Got mode = 'geometric'\n");
		else
			printf("Got mode = 'algebraic'\n");
		// 判定输入数据是否合法
		if (B_Spline_Pointer->isLegal())
		{
			printf("Parameters checked.\n");
			system("pause");
			return B_Spline_Pointer;
		}
		else
			printf("Illegal parameters. Please ensure that n > h >= p >= 1 and n < 50, h < 10. Type in again.\n");
		system("pause");
	}
}