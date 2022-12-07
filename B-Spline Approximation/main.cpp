#include "B_Spline.h"
#include "procedure.h"
#include <cstdio>

int main(int argc, char** argv)
{
	// 读入给定数据h、p、n
	B_Spline* B_Spline_Pointer = getData();

	// 计算对应的拟合曲线，包括两种近似方案
	B_Spline_Pointer->init();

	// 曲线的绘制与实时修改
	glMain(argc, argv, B_Spline_Pointer);
}