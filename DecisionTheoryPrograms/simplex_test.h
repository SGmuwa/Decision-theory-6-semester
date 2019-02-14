#pragma once
#include "UserInterface.h"
#include "Test.h"
#include "simplex.h"

/*
Функция, которую будет подставлять в Симплекс метод.
Она проивзодит следующие вычисления:
f(x) = x0*x0 - x0*x1 + 3*x1*x1 - x0.
length: размер входных данных.
x: аргументы входной функции.
output: указатель, куда надо записать значение функции при этих аргументах.
Возвращает: Код ошибки.
0 - Всё ок.
1 - length не равен 2. Поддерживается только 2.
2 - x отправлен NULL.
3 - output отправлен NULL.
*/
int Simplex_test_function(unsigned char length, const double * x, double * output) {
	if (length != 2) {
		printf("simplex_test_function: length = %d\n", length);
		return 1;
	}
	if (x == NULL) {
		printf("simplex_test_function: x is null\n");
		return 2;
	}
	if (output == NULL) {
		printf("simplex_test_function: output is NULL\n");
		return 3;
	}
	*output = x[0] * x[0] - x[0] * x[1] + 3 * x[1] * x[1] - x[0];
	return 0;
}

extern int Simplex_run(int f(unsigned char length, const double * x, double * output), char isNeedMax, double accuracy, double * output);

int Simplex_test_teacherFindXMinTest(void) {
	double x_answer[2];
	int error = Simplex_run(Simplex_test_function, 0, 0.1, x_answer);
	if (Test_assertEqualsInt(L"1. Во время симплекса произошла ошибка.", 0, error)) return 1;
	if (Test_assertEqualsDouble(L"2. Симплекс не правильно посчитал x0.", 0.483, x_answer[0], 0.001)) return 2;
	if (Test_assertEqualsDouble(L"3. Симплекс не правильно посчитал x1.", 0.129, x_answer[1], 0.001)) return 3;
	return 0;
}

int Simplex_test_functioTest(int f(unsigned char length, const double * x, double * output)) {
	unsigned char length = 1;
	double x[2];
	double output = 132.321;
	if (Test_assertEqualsInt(L"1. Функция не реагирует на ошибку недостаточности количества аргументов", 1, f(length, x, &output))) return 1;
	if (Test_assertEqualsDouble(L"2. Во время ошибки всё равно был отправлен результат.", 132.321, output, 0.0)) return 2;
	length = 2;
	x[0] = 0; x[1] = 0;
	if (Test_assertEqualsInt(L"3. У функции какие-то проблемы", 0, f(length, x, &output))) return 3;
	if (Test_assertEqualsDouble(L"4. Функция считает не правильно.", 0.0, output, 0.0)) return 4;
	x[0] = 0.483; x[1] = 0.129;
	if (Test_assertEqualsInt(L"5. У функции какие-то проблемы", 0, f(length, x, &output))) return 5;
	if (Test_assertEqualsDouble(L"6. Функция считает не правильно.", -0.262, output, 0.001)) return 6;
	return 0;
}

int Simplex_test_functionTeacherTest(void) {
	return Simplex_test_functioTest(Simplex_test_function);
}

int Simplex_test_main(void) {
	if (Test_assertEqualsInt(L"1. Тестируемая функция работает не кооректно", 0, Simplex_test_functionTeacherTest())) return 1;
	if (Test_assertEqualsInt(L"2. Тест поиска минимума от преподавателя не пройден", 0, Simplex_test_teacherFindXMinTest())) return 2;
	return 0;
}
