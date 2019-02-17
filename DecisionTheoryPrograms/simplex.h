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
const double * start: Указатель на начальный вектор.
FILE * out: Указатель, куда надо отправлять информацию об отладке. Укажите NULL, чтобы ничего не отправлять.
Возвращает: код ошибки.
1 - Функция не реализована.
2 - Либо f либо output отправлены NULL.
3 - Нехватка памяти для вычислений.
4 - Ошибка при вызове функции.
5 - Ошибка при поиске fvalue_minmax.
*/
int Simplex_runPrint(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, const double * start, FILE * out) {
	if (f == NULL || output == NULL)
		return 2;
	if (length > 2) {
		return 1;
	}
	double * memory = (double*)malloc(7 * length * sizeof(double)) + 0 * length;
	double * x[] = { memory + 0 * length, // current
		memory + 1 * length, // one
		memory + 2 * length, // two
		memory + 3 * length }, // new
		*d = memory + 4 * length,
		*x_center = memory + 5 * length,
		*x_mirror = memory + 6 * length;
	double * x_minmax = NULL; // Это указатель на ту переменную, которая ближе к минимому или максимому. Если isNeedMax = 0, то min. Если isNeedMax = 1, то хранит max.
	double * x_maxmin = NULL; // Это указатель на ту переменную, которая ближе к минимому или максимому. Если isNeedMax = 0, то max. Если isNeedMax = 1, то хранит min.
	if (memory == NULL)
		return 3;
	for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
		for (unsigned char ii = 4 - 1; ii != 0; ii--) {
			x[ii][i] = 0.0;
		}
		x[0][i] = start[i];
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
		fvalue_minmax = nan(NULL), // Если isNeedMax = 0, то min. Если isNeedMax = 1, то хранит max.
		fvalue_maxmin = nan(NULL), // Если isNeedMax = 0, то max. Если isNeedMax = 1, то хранит min.
		fE[] = { nan(NULL),
		nan(NULL),
		nan(NULL) };
	int ferror = 0;
	for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--)
		d[i] = (sqrt(length + 1) + i * length - 1)*edgeLength / (length*sqrt(2));
	for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
		x[1][i] = x[0][i] + d[i];
		x[2][i] = x[0][i] + d[length - i - 1];
	}

	// Вычисление x_current, x_one, x_two и печать их -----------------------
	for (unsigned char ii = 0; ii < 3; ii++) {
		ferror = f(length, x[ii], &(fvalue[ii]));
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue[%d]: %lf\n", ferror, (int)ii, fvalue[ii]);
			free(memory);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%zu;\t", k++);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x[ii][i]);
			fprintf(out, "f(...)=%0.3lf\n", fvalue[ii]);
		}
	}
	do {
		// Нам нужен максимум или минимум? ------------------------

		unsigned char maxminIndex = 2;
		if (isNeedMax)
			for (unsigned char ii = 4 - 3; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] < fvalue[maxminIndex])
					maxminIndex = ii;
			}
		else
			for (unsigned char ii = 4 - 3; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[maxminIndex])
					maxminIndex = ii;
			}

		x_maxmin = x[maxminIndex];
		fvalue_maxmin = fvalue[maxminIndex];
		if (out != NULL)
			printf("fvalue_maxmin: %lf\n", fvalue_maxmin);

		// Поиск тяжести и отражённой величины

		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
			x_center[i] = 0.0;
			for (unsigned char ii = 4 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if(ii != maxminIndex)
					x_center[i] += x[ii][i]; // тут надо взять два наименьших (isNeedMax == false) или два наибольших (isNeedMax)
			}
			x_center[i] /= 2.0;
		}

		if (out != NULL) {
			fprintf(out, "N;\t");
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x_center[i]);
			fprintf(out, "f_center(...)=not need\n");
		}

		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
			x[3][i] = 2 * x_center[i] - x_maxmin[i];
		}

		// Печать значения функции x_new -------------------

		ferror = f(length, x[3], &fvalue[3]);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue[%d]: %lf\n", ferror, 3, fvalue[3]);
			free(memory);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%zu;\t", k++);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x[3][i]);
			fprintf(out, "f(...)=%0.3lf\n", fvalue[3]);
		}

		// Проверка, можем ли закончить алгоритм.

		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
			x_center[i] = 0.0;
			for (unsigned char ii = 4 - 1; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (ii != maxminIndex)
					x_center[i] += x[ii][i]; // Надо пропустить наибольший, если isNeedMax == false.
			}
			x_center[i] /= 3.0;
		}
		ferror = f(length, x_center, &fvalue_center);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_center: %lf\n", ferror, fvalue_center);
			free(memory);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "N;\t");
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x_center[i]);
			fprintf(out, "f_center(...)=%0.3lf\n", fvalue_center);
		}

		unsigned char minmaxIndex = 3;
		if (isNeedMax)
			for (unsigned char ii = 4 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[minmaxIndex])
					minmaxIndex = ii;
			}
		else
			for (unsigned char ii = 4 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[minmaxIndex])
					minmaxIndex = ii;
			}
		x_minmax = x[minmaxIndex];
		fvalue_minmax = fvalue[minmaxIndex];

		for (unsigned char ii = 4 - 1, i = 0; ii != (unsigned char)~(unsigned char)0; ii--) {
			if (ii != maxminIndex) // Надо пропустить наибольший, если isNeedMax == false.
				fE[i++] = fabs(fvalue[ii] - fvalue_center);
		}
		if (out != NULL) {
			printf("E[0]=%0.3lf\tE[1]=%0.3lf\tE[2]=%0.3lf\n", fE[0], fE[1], fE[2]);
		}

		// Готовим новый симплекс на тот случай, если не подходит ------

		// isNeedMax == True => надо отбросить самый маленький.
		// isNeedMax == False => надо отбросить самый максимальный.

		unsigned char needDeleteIndex = 3;
		if (isNeedMax) {
			for (unsigned char ii = 4 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] < fvalue[needDeleteIndex])
					needDeleteIndex = ii;
			}
		}
		else
			for (unsigned char ii = 4 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[needDeleteIndex])
					needDeleteIndex = ii;
			}

		if (out != NULL)
			printf("delete f = %0.3lf\t", fvalue[needDeleteIndex]);

		for (unsigned char ii = 0, i = 0; ii < 3; ii++) {
			if (i == needDeleteIndex)
				i++;
			fvalue[ii] = fvalue[i];
			for(unsigned char j = length - 1; j != (unsigned char)~(unsigned char)0; j--)
				x[ii][j] = x[i][j];
			i++;
		}

		if (out != NULL)
			printf("f[0]=%0.3lf\tf[1]=%0.3lf\tf[2]=%0.3lf\n", fvalue[0], fvalue[1], fvalue[2]);

	} while (fE[0] >= accuracy || fE[1] >= accuracy || fE[2] >= accuracy);
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
int Simplex_run(int f(unsigned char length, const double * x, double * output), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, const double * start) {
	return Simplex_runPrint(f, length, edgeLength, isNeedMax, accuracy, output, start, NULL);
}
