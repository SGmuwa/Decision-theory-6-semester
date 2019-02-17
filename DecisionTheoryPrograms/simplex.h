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
4 - Ошибка при вызове функции.
5 - Ошибка при поиске fvalue_minmax.
*/
int Simplex_runPrint(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, FILE * out) {
	if (f == NULL || output == NULL)
		return 2;
	double * memory = (double*)malloc(7 * length * sizeof(double)) + 0 * length;
	double * x[] = { memory + 0 * length, // current
		memory + 1 * length, // one
		memory + 2 * length, // two
		memory + 3 * length }, // new
		*d = memory + 4 * length,
		*x_center = memory + 5 * length,
		*x_mirror = memory + 6 * length;
	double * x_minmax = NULL; // Это указатель на ту переменную, которая ближе к минимому или максимому.
	if (memory == NULL)
		return 3;
	for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
		for (unsigned char ii = length - 1; ii != ~(unsigned char)0; i--) {
			x[ii][i] = 0.0;
		}
		d[i] = 0.0;
		x_center[i] = 0.0;
		x_mirror[i] = 0.0;
	}
	size_t k = 0;
	double fvalue[] = { nan(NULL), // current
		nan(NULL), // one
		nan(NULL), // two
		nan(NULL) }, // new
		fvalue_center = nan(NULL),
		fvalue_minmax = nan(NULL),
		fE_current = nan(NULL),
		fE_minmax = nan(NULL),
		fE_new = nan(NULL);
	int ferror = 0;
	for (unsigned char i = length - 1; i != ~(unsigned char)0; i--)
		d[i] = (sqrt(length + 1) + pow(length, i) - 1)*edgeLength / (length*sqrt(2));
	for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
		x[1][i] = x[0][i] + d[i];
		x[2][i] = x[0][i] + d[length - i];
	}

	// Вычисление x_current, x_one, x_two и печать их -----------------------
	for (unsigned char ii = 0; ii < 3; ii++) {
		ferror = f(length, x[0], &fvalue[0]);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue[%d]: %lf\n", ferror, (int)ii, fvalue[ii]);
			free(memory);
			return 4;
		}
	}
	do {
		// Нам нужен максимум или минимум? ------------------------

		if (isNeedMax)
			if (fvalue[1] > fvalue[2])
				x_minmax = x[1];
			else
				x_minmax = x[2];
		else
			if (fvalue[1] > fvalue[2])
				x_minmax = x[2];
			else
				x_minmax = x[1];

		// Поиск тяжести и отражённой величины

		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x_center[i] = (x[0][i] + x_minmax[i]) / 2.0;
		}

		x_minmax = x_minmax == x[1] ? x[2] : x[1]; // Нам жуно другое, поэтому делаем подмену.

		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x[3][i] = 2 * x_center[i] - x_minmax[i];
		}

		x_minmax = x_minmax == x[1] ? x[2] : x[1]; // И возвращаем как было.

		// Печать значения функции x_new -------------------

		ferror = f(length, x[3], &fvalue[3]);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue[%d]: %lf\n", ferror, 3, fvalue[3]);
			free(memory);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%ld;\t", k);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%d]=%lf;\t", i, x[3][i]);
			fprintf(out, "f(...)=%lf\n", fvalue[3]);
		}

		// Проверка, можем ли закончить алгоритм.

		for (unsigned char i = length - 1; i != ~(unsigned char)0; i--) {
			x_center[i] = (x[0][i] + x_minmax[i] + x[3][i]) / 3.0;
		}
		ferror = f(length, x_center, &fvalue_center);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_center: %lf\n", ferror, fvalue_center);
			free(memory);
			return 4;
		}
		fvalue_minmax = nan(NULL);
		for(unsigned char ii = 4 - 1; ii != ~(unsigned char)0; ii--)
			if (x_minmax == x[ii]) {
				fvalue_minmax = fvalue[ii];
				break;
			}
		if (isnan(fvalue_minmax)) {
			free(memory);
			return 5;
		}

		fE_current = fabs(fvalue[0] - fvalue_center);
		fE_minmax = fabs(fvalue_minmax - fvalue_center);
		fE_new = fabs(fvalue[3] - fvalue_center);

		// Готовим новый симплекс на тот случай, если не подходит ------

		k++;

		// isNeedMax == True => надо отбросить самый маленький.
		// isNeedMax == False => надо отбросить самый максимальный.

		unsigned char minmaxIndex = 3;
		if (isNeedMax)
			for (unsigned char ii = 4 - 2; ii != ~(unsigned char)0; ii--) {
				if (fvalue[ii] < fvalue[minmaxIndex])
					minmaxIndex = ii;
			}
		else
			for (unsigned char ii = 4 - 2; ii != ~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[minmaxIndex])
					minmaxIndex = ii;
			}

		for (unsigned char ii = 0, i = 0; ii < 3; ii++) {
			if (i == minmaxIndex)
				i++;
			x[ii] = x[i++];
		}

	} while (fE_current >= accuracy && fE_minmax >= accuracy && fE_new >= accuracy);
	// Запись ответа
	for (unsigned char i = 0; i < length; i++) {
		output[i] = x[3][i];
	}
	free(memory);
	return 0;
}

/*
Возвращает: код ошибки.
1 - Функция не реализована.
*/
int Simplex_run(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output) {
	return Simplex_runPrint(f, length, edgeLength, isNeedMax, accuracy, output, NULL);
}
