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
	double * current = (double*)malloc(length * sizeof(double));
	if (current == NULL)
		return 3;
	for (size_t i = length - 1; i != ~(size_t)0; i--)
		current[i] = 0.0;
	size_t k = 0;
	double fvalue = nan(NULL),
		d1 = nan(NULL),
		d2 = nan(NULL);
	int ferror = 0;
	do {
		ferror = f(length, current, &fvalue);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value: %lf", ferror, fvalue);
			free(current);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%ld;\t");
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%z]=%lf;\t", i, current[i]);
			fprintf(out, "f(...)=%lf", fvalue);
		}
		d1 = (sqrt(length + 1) - 1)*edgeLength / (length*sqrt(2));
		d2 = (sqrt(length + 1) + length - 1)*edgeLength / (length*sqrt(2));
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
