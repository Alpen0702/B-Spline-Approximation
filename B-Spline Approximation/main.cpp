#include "B_Spline.h"
#include "procedure.h"
#include <cstdio>

int main(int argc, char** argv)
{
	// �����������h��p��n
	B_Spline* B_Spline_Pointer = getData();

	// �����Ӧ��������ߣ��������ֽ��Ʒ���
	B_Spline_Pointer->init();

	// ���ߵĻ�����ʵʱ�޸�
	glMain(argc, argv, B_Spline_Pointer);
}