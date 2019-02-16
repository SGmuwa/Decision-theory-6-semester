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
	double output = nan(NULL);
	int error = Simplex_run(Simplex_Console_function, length, edgeLength, isNeedMax, accuracy, &output);
	if (isnan(output))
		wprintf(L"Произошла ошибка симплекса: %d\n", error);
	else
		wprintf(L"Расчёт симплекса: %lf\n", output);
	return error;
}

int Simplex_Console_main(void) {
	return Simplex_Console_fmain(stdin, stdout);
}
