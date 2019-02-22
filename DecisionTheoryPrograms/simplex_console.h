#include "simplex.h"
#include "UserInterface.h"
#include <float.h>
#include <math.h>

int Simplex_Console_function(unsigned char length, const double * x, double * output) {
	for (unsigned char i = 0; i < length; i++) {
#ifdef _MSC_VER
		printf_s(
#else
		printf(
#endif
			"x[%d] = %lf; ", i, x[i]
		);
	}
	*output = UserInterface_GetDouble("value of function = ");
	return 0;
}

int Simplex_Console_fmain(FILE * in, FILE * out) {
	int isNeedMax = UserInterface_GetChek("0 - необходимо min\n1 - необходимо max", 1);
	double accuracy = UserInterface_GetDoubleLimit("Необходимая точность симплекса: ", DBL_MIN, DBL_MAX);
	unsigned char length = UserInterface_GetChek("Сколько переменных в вашей функции?", 254u);
	double edgeLength = UserInterface_GetDoubleLimit("Длинна ребра: ", DBL_MIN, DBL_MAX);
	double * start = (double *)malloc(2 * length * sizeof(double));
	if (start == NULL) {
		printf("Simplex_Console_fmain:malloc error.");
		return 1;
	}
	char buffer[32];
	for (unsigned char i = 0; i < length; i++) {
#ifndef _MSC_VER
		sprintf(buffer, "x_start[%d] = ", i);
#else
		sprintf_s(buffer, 32, "x_start[%d] = ", i);
#endif
		start[i] = UserInterface_GetDoubleLimit(buffer, -DBL_MAX, DBL_MAX);
	}
	double * output = start + length;
	int error = Simplex_runPrint(Simplex_Console_function, length, edgeLength, isNeedMax, accuracy, output, start, out);
	if (error != 0)
		wprintf(L"Произошла ошибка симплекса: %d\n", error);
	else {
		wprintf(L"Расчёт симплекса:\n");
		for (unsigned char i = 0; i < length; i++)
			wprintf(L"x[%d]=%lf\t", i, output[i]);
		wprintf(L"\n");
	}
	free(start);
	return error;
}

int Simplex_Console_main(void) {
	return Simplex_Console_fmain(stdin, stdout);
}
