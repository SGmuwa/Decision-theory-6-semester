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
int Simplex_test_functionTeacher(unsigned char length, const double * x, double * output, void * context) {
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

int Simplex_test_functionParabola(unsigned char length, const double * x, double * output, void * context) {
	if (length > 1)  return 1; if (x == NULL) return 2; if (output == NULL) return 3;
	*output = x[0] * x[0];
	return 0;
}

int Simplex_test_function0(unsigned char length, const double * x, double * output, void * context) {
	if (length < 2 || length > 3)  return 1; if (x == NULL) return 2; if (output == NULL) return 3;
	*output = x[0] * 11.0 / 5.0 + x[1] * x[1] * 11.0 / 5.0 + x[0] * x[0] * 23.0 / 10.0 - 11.0 / 5.0
		+ (length == 2 ? 0 : (
			x[2] * x[2] * 23.0 / 10.0));
	return 0;
}

int Simplex_test_function60(unsigned char length, const double * x, double * output, void * context) {
	if (length < 2 || length > 3)  return 1; if (x == NULL) return 2; if (output == NULL) return 3;
	*output = x[0] * x[0] * 13.0 / 5.0 + x[1] * x[1] * 7.0 / 5.0
		+ (length == 2 ? -x[1] * 21.0 / 10.0 : (
			x[2] * 6.0 / 5.0 - x[1] * x[2] * 21.0 / 10.0));
	return 0;
}

int Simplex_test_function50(unsigned char length, const double * x, double * output, void * context) {
	if (length < 2 || length > 3)  return 1; if (x == NULL) return 2; if (output == NULL) return 3;
	// 1 0 0,1 2 0,25 0,0 0 0 0,51096 0,516373 0,115625 0,17032 -0,279711 -0,0932371 -0,285123 -0,22682 -0,449209 -0,376556
	*output = x[0] * 13.0 / 10.0 + x[1] * 13.0 / 10.0 + x[0] * x[0] * 19.0 / 10.0 + x[1] * x[1] * 9.0 / 5.0
		+ (length == 2 ? 0 : (
			x[2] * x[2] * 13.0 / 5.0));
	return 0;
}

int Simplex_test_teacherFindXMinTest(FILE * out) {
	// https://docs.google.com/document/d/1FDIk30yvL9qWl7x6AWMDSHX6wCzaQEIFVrTddNiGejs/edit
	double x_answer[2] = { nan(NULL), nan(NULL) };
	double start[] = { 0.0, 0.0 };
	int error = Simplex_runPrint(Simplex_test_functionTeacher, 2, 0.25, 0, 0.1, x_answer, start, NULL, out);
	if (Test_assertEqualsInt(L"1. Во время симплекса произошла ошибка.", 0, error)) return 1;
	if (Test_assertEqualsDouble(L"2. Симплекс не правильно посчитал x0.", 0.483, x_answer[0], 0.001)) return 2;
	if (Test_assertEqualsDouble(L"3. Симплекс не правильно посчитал x1.", 0.129, x_answer[1], 0.001)) return 3;
	return 0;
}

/*
Тестирование для перебора разных функций.
*/
int Simplex_test_studentsFindXMinTest(FILE * out) {
	// https://drive.google.com/drive/folders/1jfJSP_ob3i55cCLQ8aHz47avZ9fDo0YN
	double x[3] = {nan(NULL), nan(NULL), nan(NULL)}; // Сюда записывается ответ.
	double f;
	int error = 0;
	struct paramsOfTests {
		double start[3]; // Начальный x.
		double m; // Длинна ребра.
		double E; // Точность.
		unsigned char length; // Количество переменных.
		unsigned char isNeedMax; // Нужно найти максимум функции?
		double answer[3]; // Правильный ответ.
		double fanswer; // Правильное значение функции.
		int error; // Какой код ошибки ожидается? В случае, если указать не 0, то ответы не будут сравниваться.
		int(*function)(unsigned char length, const double * x, double * output, void * context); // Функция, которую надо проверить.
	};
	struct paramsOfTests param[] = {
		{{1, 0, 0}, 1, 0.1, 1, 0, {0.0, 0.0, 0.0}, 0.0, 0, Simplex_test_functionParabola},
		{{1.0, 1.0, 0.0}, 0.25, 0.1, 2, 0, {-11.0 / 23.0, 0.0, 0.0}, -627.0 / 230.0, 0, Simplex_test_function0},
		{{1.0, 1.0, 0.0}, 0.25, 0.1, 2, 0, {-13.0 / 38.0, -13.0 / 36.0, 0.0}, -6253.0 / 13680.0, 0, Simplex_test_function50},
		{{1.0, 1.0, 0.0}, 0.25, 0.1, 2, 0, {0.0, 0.75, 0.0}, -63.0 / 80.0, 0, Simplex_test_function60},
		{{-1.0, 1.0, 1.0}, 0.25, 0.1, 3, 0, {-11.0/23.0, 0.0, 0.0}, -627.0 / 230.0, 0, Simplex_test_function60}
	};
	wchar_t buffer[256];
	for (unsigned i = 0; i < sizeof(param) / sizeof(struct paramsOfTests); i++) {
		fprintf(out, "%u. -------\n", i);

		error = Simplex_runPrint(param[i].function, param[i].length, param[i].m, param[i].isNeedMax, param[i].E, x, param[i].start, NULL, out);
#ifdef _MSC_VER
		swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t),
#else
		swprintf(buffer,
#endif // _MSC_VER
			L"%u.1. Во время симплекса произошла ошибка.", i);
		if (Test_assertEqualsInt(buffer, param[i].error, error))
			return i + 1;

		if (param[i].error == 0) {

			for (unsigned ii = 0; ii < param[i].length; ii++) {
#ifdef _MSC_VER
				swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t),
#else
				swprintf(buffer,
#endif // _MSC_VER
					L"%u.2. Координата %u не верна.", i, ii);
				if (Test_assertEqualsDouble(buffer, param[i].answer[ii], x[ii], 2 * param[i].E)) return i + 1;
			}
#ifdef _MSC_VER
			swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t),
#else
			swprintf(buffer,
#endif // _MSC_VER
				L"%u.3. Функция оказалась не вычисляема.", i);
			if (Test_assertEqualsInt(buffer, 0, param[i].function(param[i].length, x, &f, NULL))) return i + 1;

#ifdef _MSC_VER
			swprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t),
#else
			swprintf(buffer,
#endif // _MSC_VER
				L"%u.4. Значение функции не верно.", i);
			if (Test_assertEqualsDouble(buffer, param[i].fanswer, f, param[i].E)) return i + 1;
		}
	}
	return 0;
}

int Simplex_test_functioTest(int f(unsigned char length, const double * x, double * output, void * context)) {
	unsigned char length = 1;
	double x[2];
	double output = 132.321;
	if (Test_assertEqualsInt(L"1. Функция не реагирует на ошибку недостаточности количества аргументов", 1, f(length, x, &output, NULL))) return 1;
	if (Test_assertEqualsDouble(L"2. Во время ошибки всё равно был отправлен результат.", 132.321, output, 0.0)) return 2;
	length = 2;
	x[0] = 0; x[1] = 0;
	if (Test_assertEqualsInt(L"3. У функции какие-то проблемы", 0, f(length, x, &output, NULL))) return 3;
	if (Test_assertEqualsDouble(L"4. Функция считает не правильно.", 0.0, output, 0.0)) return 4;
	x[0] = 0.483; x[1] = 0.129;
	if (Test_assertEqualsInt(L"5. У функции какие-то проблемы", 0, f(length, x, &output, NULL))) return 5;
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
		size_t size = ftell(testFp) + 1;
		fseek(testFp, 0, SEEK_SET);
		char * buffer = malloc(size);
		if (buffer == NULL)
		{
			while (!feof(testFp))
			{
				fread(&buffer, sizeof(char), 1, testFp);
				printf("%c", *buffer);
			}
		}
		else
		{
			size = fread(buffer, sizeof(char), size, testFp);
			buffer[size] = 0;
			printf("%s", buffer);
			free(buffer);
		}
	}
	if (testFp != stdout)
	{
		fclose(testFp);
	}
	return error;
}
