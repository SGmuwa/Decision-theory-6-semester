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
1 - Заданный length не поддерживается.
2 - x отправлен NULL.
3 - output отправлен NULL.
*/
int Simplex_test_functionTeacher(unsigned char length, const double * x, double * output) {
	if (length != 2) {
		return 1;
	}
	if (x == NULL) {
		return 2;
	}
	if (output == NULL) {
		return 3;
	}
	*output = x[0] * x[0] - x[0] * x[1] + 3 * x[1] * x[1] - x[0];
	return 0;
}

int Simplex_test_function0(unsigned char length, const double * x, double * output) {
	if (length < 2 || length > 3)  return 1; if (x == NULL) return 2; if (output == NULL) return 3;
	*output = x[0] * 11.0 / 5.0 + x[1] * x[1] * 11.0 / 5.0 + x[0] * x[0] * 23.0 / 10.0 - 11.0 / 5.0
		+ (length == 2 ? 0 : (
			x[2] * x[2] * 23.0 / 10.0));
	return 0;
}

int Simplex_test_function60(unsigned char length, const double * x, double * output) {
	if (length < 2 || length > 3)  return 1; if (x == NULL) return 2; if (output == NULL) return 3;
	*output = x[0] * x[0] * 13.0 / 5.0 + x[1] * x[1] * 7.0 / 5.0
		+ (length == 2 ? -x[1] * 21.0 / 10.0 : (
			x[2] * 6.0 / 5.0 - x[1] * x[2] * 21.0 / 10.0));
	return 0;
}

extern int Simplex_run(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, const double * start);

int Simplex_test_teacherFindXMinTest(FILE * out) {
	// https://docs.google.com/document/d/1FDIk30yvL9qWl7x6AWMDSHX6wCzaQEIFVrTddNiGejs/edit
	double x_answer[2];
	double start[] = { 0.0, 0.0 };
	int error = Simplex_runPrint(Simplex_test_functionTeacher, 2, 0.25, 0, 0.1, x_answer, start, out);
	if (Test_assertEqualsInt(L"1. Во время симплекса произошла ошибка.", 0, error)) return 1;
	if (Test_assertEqualsDouble(L"2. Симплекс не правильно посчитал x0.", 0.483, x_answer[0], 0.001)) return 2;
	if (Test_assertEqualsDouble(L"3. Симплекс не правильно посчитал x1.", 0.129, x_answer[1], 0.001)) return 3;
	return 0;
}

int Simplex_test_studentsFindXMinTest(FILE * out) {
	// https://drive.google.com/drive/folders/1jfJSP_ob3i55cCLQ8aHz47avZ9fDo0YN
	double x[3] = {nan(NULL), nan(NULL), nan(NULL)}; // Сюда записывается ответ.
	double f;
	double start[3]; // Начальный x.
	double m = 0.25; // Длинна ребра.
	double E = 0.1; // Точность.
	int error = 0;
	start[0] = 1.0; start[1] = 1.0;
	fprintf(out, "0. -------\n");
	error = Simplex_runPrint(Simplex_test_function0, 2, m, 0, E, x, start, out);
	if (Test_assertEqualsInt(L"0.1. Во время симплекса произошла ошибка.", 0, error)) return 1;
	if (Test_assertEqualsDouble(L"0.2. Первая координата не верна.", -11.0 / 23.0, x[0], 2*E)) return 1;
	if (Test_assertEqualsDouble(L"0.3. Вторая координата не верна.", 0, x[1], 2*E)) return 1;
	Simplex_test_function0(2, x, &f);
	if (Test_assertEqualsDouble(L"0.4. Значение функции не верно.", -627.0 / 230.0, f, E)) return 1;
	fprintf(out, "60. -------\n");
	error = Simplex_runPrint(Simplex_test_function60, 2, m, 0, E, x, start, out);
	if (Test_assertEqualsInt(L"60.1. Во время симплекса произошла ошибка.", 0, error)) return 61;
	if (Test_assertEqualsDouble(L"60.2. Первая координата не верна.", 0, x[0], 2*E)) return 61;
	if (Test_assertEqualsDouble(L"60.3. Вторая координата не верна.", 0.75, x[1], 2*E)) return 61;
	Simplex_test_function60(2, x, &f);
	if (Test_assertEqualsDouble(L"0.4. Значение функции не верно.", -63.0 / 80.0, f, E)) return 61;
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
	return Simplex_test_functioTest(Simplex_test_functionTeacher);
}

// Запуск тестирования всего симплекс-метода.
int Simplex_test_main(void) {
	int error = 0;
	FILE * testFp = NULL;
#ifdef _MSC_VER
	fopen_s(&testFp, "test.log", "w+");
#else
	testFp = fopen("test.log", "w+");
#endif
	if (testFp == NULL)
		testFp = stdout;

	if (error == 0 && Test_assertEqualsInt(L"1. Тестируемая функция работает не кооректно", 0, Simplex_test_functionTeacherTest())) error = 1;
	if (error == 0 && Test_assertEqualsInt(L"2. Тест поиска минимума от преподавателя не пройден", 0, Simplex_test_teacherFindXMinTest(testFp))) error = 2;
	if (error == 0 && Test_assertEqualsInt(L"3. Тест поиска минимума для студентов не пройден", 0, Simplex_test_studentsFindXMinTest(testFp))) error = 3;

	if (error != 0 && testFp != stdout)
	{
		fseek(testFp, 0, SEEK_SET);
		char buffer;
		while (!feof(testFp)) {
			fread(&buffer, sizeof(char), 1, testFp);
			printf("%c", buffer);
		}
	}
	if (testFp != stdout)
	{
		fclose(testFp);
	}
	return error;
}
