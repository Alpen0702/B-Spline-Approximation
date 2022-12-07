#pragma once
#include "B_Spline.h"

// getData.cpp
B_Spline* getData();

// glOperation.cpp
void glMain(int argc, char** argv, B_Spline* b_Spline);

// matOperation.cpp
double** matTighten(double* M1, long a, long b, long cols);
double** matMulti(double* M1, double* M2, long a, long b, long c);
double** matTrans(double* M1, long a, long b);
double** matInverse(double* M1, long a);