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
void * contextFunction: Указатель на контекст функции. Будет отправлено в функцию.
Возвращает: Код ошибки.
)
unsigned char length: Количество измерений в аргументе функции.
double edgeLength: Длинна ребра симплекс-метода.
char isNeedMax: Поставьте true, если надо искать максимум. Иначе - false.
double accuracy: Заданная точность ответа.
double * output: Указатель, куда записать аргумент-вектор минимума или максимума.
const double * start: Указатель на начальный вектор.
FILE * out: Указатель, куда надо отправлять информацию об отладке. Укажите NULL, чтобы ничего не отправлять.
void * contextFunction: Указатель на контекст функции. Он будет передан функции.
Возвращает: код ошибки.
1 - Функция не реализована.
2 - Либо f либо output отправлены NULL.
3 - Нехватка памяти для вычислений.
4 - Ошибка при вызове функции.
5 - Ошибка при поиске fvalue_minmax.
6 - Обнаружен бесконечный цикл. Попробуйте увеличить точность или уменьшить длинну ребра.
*/
int Simplex_runPrint(int f(unsigned char length, const double * x, double * output, void * contextFunction), const unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, const double * start, void * contextFunction, FILE * out) {
	if (f == NULL || output == NULL)
		return 2;
	if (length > 250)
		return 1;
	double * memory1 = (double*)malloc(2 * length * sizeof(double)) + 0 * length;
	if (memory1 == NULL)
		return 3;
	double 
		*x_center = memory1 + 0 * length,
		*x_mirror = memory1 + 1 * length;
	double ** memory2 = (double **)malloc((length + 2) * sizeof(double*));
	if (memory2 == NULL) {
		free(memory1);
		return 3;
	}
	double ** x = memory2;
	for (unsigned char ii = length + 1; ii != (unsigned char)~(unsigned char)0; ii--) {
		x[ii] = (double*)malloc(length * sizeof(double));
		if (x[ii] == NULL) {
			ii++;
			while (ii != length + 2) {
				free(x[ii]);
			}
			free(memory1);
			free(memory2);
			return 3;
		}
	}

	double * x_minmax = NULL; // Это указатель на ту переменную, которая ближе к минимому или максимому. Если isNeedMax = 0, то min. Если isNeedMax = 1, то хранит max.
	double * x_maxmin = NULL; // Это указатель на ту переменную, которая ближе к минимому или максимому. Если isNeedMax = 0, то max. Если isNeedMax = 1, то хранит min.
	
	for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
		for (unsigned char ii = length + 2 - 1; ii != 0; ii--) {
			x[ii][i] = 0.0;
		}
		x[0][i] = start[i];
		x_center[i] = 0.0;
		x_mirror[i] = 0.0;
	}
	size_t k = 0;
	// length = 2 => length0, length1, 1, 2, length0, legth1, 1
	double * memory3 = (double*)malloc(((length + 2) + (length + 1) + (length + 1)) * sizeof(double));
	if (memory3 == NULL) {
		for (unsigned char jj = length + 2 - 1; jj != (unsigned char)~(unsigned char)0; jj--)
			free(x[jj]);
		free(memory1);
		free(memory2);
		return 3;
	}
	double * fvalue = memory3;
	double * E = memory3 + length + 2;
	double * fE = memory3 + length + 2 + length + 1;
	for (unsigned char ii = length + 2 - 1; ii != (unsigned char)~(unsigned char)0; ii--)
		fvalue[ii] = nan(NULL);
	for (unsigned char ii = length + 1 - 1; ii != (unsigned char)~(unsigned char)0; ii--)
		E[ii] = nan(NULL);
	double
		fvalue_center = nan(NULL),
		fvalue_minmax = nan(NULL), // Если isNeedMax = 0, то min. Если isNeedMax = 1, то хранит max.
		fvalue_maxmin = nan(NULL), // Если isNeedMax = 0, то max. Если isNeedMax = 1, то хранит min.
		d[] = { nan(NULL),
			nan(NULL) };
	int ferror = 0;
	for (unsigned char i = 0; i < 2; i++)
		d[i] = (sqrt(length + 1) + i * length - 1)*edgeLength / (length*sqrt(2));
	for(unsigned char ii = 1; ii < length + 1; ii++) // Перебор векторов x_one, x_two
		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) { // Перебор координат вектора
			if (i == ii - 1)
				x[ii][i] = x[0][i] + d[0];
			else
				x[ii][i] = x[0][i] + d[1];
		}

	// Вычисление x_current, x_one, x_two и печать их -----------------------
	for (unsigned char ii = 0; ii < length + 1; ii++) {
		ferror = f(length, x[ii], &(fvalue[ii]), contextFunction);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue[%d]: %lf\n", ferror, (int)ii, fvalue[ii]);
			for (unsigned char jj = length + 2 - 1; jj != (unsigned char)~(unsigned char)0; jj--)
				free(x[jj]);
			free(memory1);
			free(memory2);
			free(memory3);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%zu;\t", k++);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x[ii][i]);
			fprintf(out, "f(...)=%0.3lf\n", fvalue[ii]);
		}
	}
	unsigned char need_continue = 0;
	do {
		// Нам нужен максимум или минимум? ------------------------

		unsigned char maxminIndex = length;

		// Последнего элемента у нас нет. Предположим, что максимальным (isNeedMax == false) является x_two
		if (isNeedMax)
			// length + 2: размер массива
			// -2: Последнего нет, предпоследний уже взят.
			// -1: Правило перебора по циклу с конца на начало.
			for (unsigned char ii = length + 2 - 2 - 1; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] < fvalue[maxminIndex])
					maxminIndex = ii;
			}
		else
			for (unsigned char ii = length + 2 - 2 - 1; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[maxminIndex])
					maxminIndex = ii;
			}

		x_maxmin = x[maxminIndex];
		fvalue_maxmin = fvalue[maxminIndex];
		if (out != NULL)
			fprintf(out, "fvalue_maxmin: %lf\n", fvalue_maxmin);

		// Поиск тяжести и отражённой величины

		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
			x_center[i] = 0.0;
			for (unsigned char ii = length + 2 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if(ii != maxminIndex)
					x_center[i] += x[ii][i]; // тут надо взять два наименьших (isNeedMax == false) или два наибольших (isNeedMax)
			}
			x_center[i] /= length;
		}

		if (out != NULL) {
			fprintf(out, "N;\t");
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x_center[i]);
			fprintf(out, "f_center(...)=not need\n");
		}

		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
			x[length + 2 - 1][i] = 2 * x_center[i] - x_maxmin[i]; // Последний элемент.
		}

		// Печать значения функции x_new -------------------

		ferror = f(length, x[length + 2 - 1], &fvalue[length + 2 - 1], contextFunction);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue[%d]: %lf\n", ferror, length + 2 - 1, fvalue[length + 2 - 1]);
			for (unsigned char jj = length + 2 - 1; jj != (unsigned char)~(unsigned char)0; jj--)
				free(x[jj]);
			free(memory1);
			free(memory2);
			free(memory3);
			return 4;
		}
		if (out != NULL) {
			fprintf(out, "%zu;\t", k++);
			for (size_t i = 0; i < length; i++)
				fprintf(out, "x[%zu]=%0.3lf;\t", i, x[length + 2 - 1][i]);
			fprintf(out, "f(...)=%0.3lf\n", fvalue[length + 2 - 1]);
		}

		// Проверка, можем ли закончить алгоритм.

		for (unsigned char i = length - 1; i != (unsigned char)~(unsigned char)0; i--) {
			x_center[i] = 0.0;
			for (unsigned char ii = length + 2 - 1; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (ii != maxminIndex) 
					x_center[i] += x[ii][i]; // Надо пропустить наибольший, если isNeedMax == false.
			}
			x_center[i] /= length + 1;
		}
		ferror = f(length, x_center, &fvalue_center, contextFunction);
		if (ferror != 0) {
			if (out != NULL)
				fprintf(out, "error fuction: %d, last value fvalue_center: %lf\n", ferror, fvalue_center);
			free(memory1);
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
			for (unsigned char ii = length + 2 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[minmaxIndex])
					minmaxIndex = ii;
			}
		else
			for (unsigned char ii = length + 2 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[minmaxIndex])
					minmaxIndex = ii;
			}
		x_minmax = x[minmaxIndex];
		fvalue_minmax = fvalue[minmaxIndex];

		for (unsigned char ii = length + 2 - 1, i = 0; ii != (unsigned char)~(unsigned char)0; ii--) {
			if (ii != maxminIndex) // Надо пропустить наибольший, если isNeedMax == false.
				fE[i++] = fabs(fvalue[ii] - fvalue_center);
		}
		if (out != NULL) {
			for (unsigned char ii = length + 1 - 1, i = 0; ii != (unsigned char)~(unsigned char)0; ii--)
				fprintf(out, "E[%d]=%0.3lf\t", ii, fE[ii]);
			fprintf(out, "\n");
		}

		// Готовим новый симплекс на тот случай, если не подходит ------

		// isNeedMax == True => надо отбросить самый маленький.
		// isNeedMax == False => надо отбросить самый максимальный.

		unsigned char needDeleteIndex = length + 1;
		if (isNeedMax) {
			for (unsigned char ii = length + 2 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] < fvalue[needDeleteIndex])
					needDeleteIndex = ii;
			}
		}
		else
			for (unsigned char ii = length + 2 - 2; ii != (unsigned char)~(unsigned char)0; ii--) {
				if (fvalue[ii] > fvalue[needDeleteIndex])
					needDeleteIndex = ii;
			}

		if (out != NULL)
			fprintf(out, "delete f = %0.3lf\t", fvalue[needDeleteIndex]);

		if (needDeleteIndex == length + 1) {
			for (unsigned char jj = length + 2 - 1; jj != (unsigned char)~(unsigned char)0; jj--)
				free(x[jj]);
			free(memory1);
			free(memory2);
			free(memory3);
			return 6;
		}

		for (unsigned char ii = 0, i = 0; ii < length + 2 - 1; ii++) {
			if (i == needDeleteIndex)
				i++;
			fvalue[ii] = fvalue[i];
			for(unsigned char j = length - 1; j != (unsigned char)~(unsigned char)0; j--)
				x[ii][j] = x[i][j];
			i++;
		}

		if (out != NULL) {
			for (unsigned char ii = length + 1 - 1; ii != (unsigned char)~(unsigned char)0; ii--) {
				fprintf(out, "f[%d]=%0.3lf\t", ii, fvalue[ii]);
			}
			fprintf(out, "\n");
		}

		need_continue = 0;
		for (unsigned char ii = length + 1 - 1; ii != (unsigned char)~(unsigned char)0; ii--) {
			need_continue = need_continue || fE[ii] >= accuracy;
		}

	} while (need_continue);

	// Запись ответа
	for (unsigned char i = 0; i < length; i++) {
		output[i] = x[length + 2 - 1][i];
	}

	for (unsigned char jj = length + 2 - 1; jj != (unsigned char)~(unsigned char)0; jj--)
		free(x[jj]);
	free(memory1);
	free(memory2);
	free(memory3);
	return 0;
}

/*
Вычисление максимума или минимума заданной функции.
int f(: Функция, которую надо исследовать.
unsigned char length: Количество измерений аргумента x функции.
const double * x: Указатель на первое измерение аргумента функции.
double * output: Указатель, куда надо записать результат функции.
void * contextFunction: Указатель на контекст функции. Будет отправлено в функцию.
Возвращает: Код ошибки.
)
unsigned char length: Количество измерений в аргументе функции.
double edgeLength: Длинна ребра симплекс-метода.
char isNeedMax: Поставьте true, если надо искать максимум. Иначе - false.
double accuracy: Заданная точность ответа.
double * output: Указатель, куда записать аргумент-вектор минимума или максимума.
const double * start: Указатель на начальный вектор.
FILE * out: Указатель, куда надо отправлять информацию об отладке. Укажите NULL, чтобы ничего не отправлять.
void * contextFunction: Указатель на контекст функции. Он будет передан функции.
Возвращает: код ошибки.
1 - Функция не реализована.
2 - Либо f либо output отправлены NULL.
3 - Нехватка памяти для вычислений.
4 - Ошибка при вызове функции.
5 - Ошибка при поиске fvalue_minmax.
6 - Обнаружен бесконечный цикл. Попробуйте увеличить точность или уменьшить длинну ребра.
*/
int Simplex_run(int f(unsigned char length, const double * x, double * output, void * contextFunction), unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, const double * start) {
	return Simplex_runPrint(f, length, edgeLength, isNeedMax, accuracy, output, start, NULL, NULL);
}
