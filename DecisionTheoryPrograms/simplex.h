#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <corecrt_math.h>


/*
Вычисление максимума или минимума заданной функции.
int f(: Функция, которую надо исследовать.
unsigned char length: Количество измерений аргумента x функции.
const double * x: Указатель на первое измерение аргумента функции.
double * output: Указатель, куда надо записать результат функции.
Возвращает: Код ошибки.
)
unsigned char length: Количество измерений в аргументе функции.
double edgeLength: Длинна ребра симплекс-метода.
char isNeedMax: Поставьте true, если надо искать максимум. Иначе - false.
double accuracy: Заданная точность ответа.
double * output: Указатель, куда записать аргумент-вектор минимума или максимума.
FILE * out: Указатель, куда надо отправлять информацию об отладке. Укажите NULL, чтобы ничего не отправлять.
Возвращает: код ошибки.
1 - Функция не реализована.
2 - Либо f либо output отправлены NULL.
3 - Нехватка памяти для вычислений.
*/
int Simplex_runPrint(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, FILE * out) {
	if (f == NULL || output == NULL)
		return 2;
	double * x_current = (double*)malloc(7 * length * sizeof(double)) + 0 * length,
		*x_one = x_current + 1 * length,
		*x_two = x_current + 2 * length,
		*x_new = x_current + 3 * length,
		*d = x_current + 4 * length,
		*x_center = 5 * length,
		*x_mirror = 6 * length;
	double * x_minmax = NULL; // Это указатель на ту переменную, которая ближе к минимому или максимому.
	if (x_current == NULL)
		return 3;
	for (size_t i = length - 1; i != ~(size_t)0; i--) {
		x_current[i] = 0.0;
		x_one[i] = 0.0;
		x_two[i] = 0.0;
		d[i] = 0.0;
		x_center[i] = 0.0;
		x_mirror[i] = 0.0;
	}
	size_t k = 0;
	double fvalue_current = nan(NULL),
		fvalue_one = nan(NULL),
		fvalue_two = nan(NULL),
		fvalue_new = nan(NULL);	
	int ferror = 0;
	do {
		for(unsigned char i = length - 1; i != ~(unsigned char)0; i--)
			d[i] = (sqrt(length + 1) + pow(length, i) - 1)*edgeLength / (length*sqrt(2));
		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x_one[i] = x_current[i] + d[i];
			x_two[i] = x_current[i] + d[length-i];
		}

		// Вычисление x_current, x_one, x_two и печать их -----------------------

		ferror = f(length, x_current, &fvalue_current);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_current: %lf\n", ferror, fvalue_current);
			free(x_current);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%ld;\t", k);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%d]=%lf;\t", i, x_current[i]);
			fprintf(out, "f(...)=%lf\n", fvalue_current);
		}
		ferror = f(length, x_one, &fvalue_one);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_one: %lf\n", ferror, fvalue_one);
			free(x_current);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%ld;\t", k);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%d]=%lf;\t", i, x_one[i]);
			fprintf(out, "f(...)=%lf\n", fvalue_one);
		}
		ferror = f(length, x_two, &fvalue_two);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_two: %lf\n", ferror, fvalue_two);
			free(x_current);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%ld;\t", k);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%d]=%lf;\t", i, x_two[i]);
			fprintf(out, "f(...)=%lf\n", fvalue_two);
		}

		// Нам нужен максимум или минимум? ------------------------

		if (isNeedMax)
			if (fvalue_one > fvalue_two)
				x_minmax = x_one;
			else
				x_minmax = x_two;
		else
			if (fvalue_one > fvalue_two)
				x_minmax = x_two;
			else
				x_minmax = x_one;

		// Поиск тяжести и отражённой величины

		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x_center[i] = (x_current[i] + x_minmax[i]) / 2.0;
		}

		x_minmax = x_minmax == x_one ? x_two : x_one;

		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x_new[i] = 2 * x_center[i] - x_minmax[i];
		}

		// Печать значения функции x_new -------------------

		ferror = f(length, x_new, &fvalue_new);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_two: %lf\n", ferror, fvalue_new);
			free(x_current);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%ld;\t", k);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%d]=%lf;\t", i, x_new[i]);
			fprintf(out, "f(...)=%lf\n", fvalue_new);
		}

		// Проверка, можем ли закончить алгоритм.

		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x_center[i] = 
		}
	} while (0);
	return 1;
}

/*
Возвращает: код ошибки.
1 - Функция не реализована.
*/
int Simplex_run(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output) {
	return Simplex_runPrint(f, length, edgeLength, isNeedMax, accuracy, output, NULL);
}
