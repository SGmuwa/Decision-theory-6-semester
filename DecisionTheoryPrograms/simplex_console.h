#include "simplex.h"
#include "UserInterface.h"
#include <float.h>
#include <math.h>
#include <limits.h>
#include "libs/tinyexpr.h"


int Simplex_Console_function(unsigned char length, const double * x, double * output, void * context) {
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

/*
expr - указатель на функцию-выражение.
Возвращает: Код ошибки.
0 - Всё ок.
1 - Заданный length не поддерживается.
2 - x отправлен NULL.
3 - output отправлен NULL.
4 - Ошибка с память
*/
int Simplex_Console_functionParse(unsigned char length, const double * x, double * output, const char * expr) {
	te_variable * input = (te_variable*)malloc(length * sizeof(te_variable));
	if (input == NULL) {
		return 4;
	}
	char buffer[1500] = { 0 };
	for (unsigned char i = 0; i < length; i++) {
		input[i].name = buffer +
#ifdef _MSC_VER
			sprintf_s(buffer, sizeof(buffer), "%sx%u", buffer, i)
#else
			sprintf(buffer, "%sx%u_", buffer, i)
#endif // _MSC_VER
			- 1 - (i < 10 ? 1 : i < 100 ? 2 : 3);
		input[i].address = x + i;
	}
	for (size_t i = 0; i < sizeof(buffer); i++) {
		if (buffer[i] == '_')
			buffer[i] = '\0';
	}
#if TEST == 1
	for (unsigned char i = 0; i < length; i++)
		printf("TEST Simplex_Console_functionParse: name[%u]=%s.\n", i, input[i].name);
#endif
	int err = 0;
	te_expr *n = te_compile(expr, input, length, &err);
	if (n) {
		*output = te_eval(n);
		te_free(n);
	}
	free(input);
	return err;
}

int Simplex_Console_fmain(FILE * in, FILE * out) {
	unsigned char length = UserInterface_GetChek("Сколько переменных в вашей функции?", 254u);
	printf("Список переменных: ");
	for (unsigned char i = 0; i < length; i++) {
		printf("x%u ", i);
	}
	printf("\n");
	char expr[1024] = { 0 };
	UserInterface_GetStr("Введите уравнение (пример 3*x0^2 + 3/2), 1024 символов: ", expr, sizeof(expr));
	int isNeedMax = UserInterface_GetChek("0 - необходимо min\n1 - необходимо max", 1);
	double accuracy = UserInterface_GetDoubleLimit("Необходимая точность симплекса: ", DBL_MIN, DBL_MAX);
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
	int error = Simplex_runPrint(Simplex_Console_functionParse, length, edgeLength, isNeedMax, accuracy, output, start, expr, out);
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
