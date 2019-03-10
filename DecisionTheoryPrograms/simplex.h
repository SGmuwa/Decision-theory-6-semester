#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <corecrt_math.h>

// TODO: Проверить шаг 6

/*
Функция занимается поиском минимального или максимального значения.
Если минимальных или максимальных несколько, то функция вернёт тот, который ближе к концу массива values.
size_t * output: Указатель, куда поместить индекс минимального или максимального среди values.
double * values: Указатель на перечень значений, среди которых надо найти минимальное или максимальное.
size_t count: количество values.
unsigned char isNeedMax: поставьте 0, если надо найти минимальное значение. Если надо найти другое, установите 1.
Возвращает: код ошибки.
1 - функция не реализована.
2 - был подан указатель NULL. Функция не поддерживает безуказательные операции.
3 - количество равно 0.
4 - выполнение с count == SIZE_MAX не поддерживается.
*/
int Simplex_searchMinmax(size_t * output, const double * values, size_t count, unsigned char isNeedMax) {
	if (output == NULL || values == NULL) return 2;
	if (count == 0) return 3;
	if (count == SIZE_MAX) return 4;
	size_t result = count - 1; // Предположим, что последний элемент тот, который нам нужен.
	if (isNeedMax) {
		for (size_t i = count - 2; i != SIZE_MAX; i--)
			if (values[result] < values[i])
				result = i;
	}
	else {
		for (size_t i = count - 2; i != SIZE_MAX; i--)
			if (values[result] > values[i])
				result = i;
	}
	*output = result;
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
7 - Ошибка при вызове функции Simplex_searchMinmax.
*/
int Simplex_runPrint(int f(unsigned char length, const double * x, double * output, void * contextFunction), const unsigned char length, double edgeLength, char isNeedMax, double accuracy, double * output, const double * start, void * contextFunction, FILE * out) {

	// Освобождение memory1, 2 и 3. Все элементы внутри x.
	#define Simplex_FREEALL {for(size_t qkfjewigwegiojwegfj = length + 2 - 1; qkfjewigwegiojwegfj != SIZE_MAX; qkfjewigwegiojwegfj--) free(x[qkfjewigwegiojwegfj]); free(memory1); free(memory2); free(memory3);}
	// Вычисление функции f в точке x. Результат помещается по указателю pointerReturn. Если ошибка, то вызывается Simplex_FREEALL и return 4;. Печать ошибки в out если это возможно.
	#define Simplex_CALLFUNCTION(x, pointerReturn) {ferror = f(length, x, pointerReturn, contextFunction); if (ferror != 0) { if (out != NULL) fprintf(out, "error fuction: %d, last value: %lf\n", ferror, *pointerReturn); Simplex_FREEALL; return 4; }}
	// Печать заголовка, аргументов и значение функции. Всё, что написано в format будет отправлено в printf перед печатью.
	#define Simplex_FUNCTIONPRINT(x, pointerReturn, format) {if (out != NULL) { fprintf(out, format); for (size_t jwoiqqf = 0; jwoiqqf < length; jwoiqqf++) fprintf(out, "x[%zu]=%0.3lf;\t", jwoiqqf, x[jwoiqqf]); fprintf(out, "f(...)=%0.3lf\n", *pointerReturn); }}
	// Вычисление и печать функции. Вызывает Simplex_CALLFUNCTION, а затем Simplex_FUNCTIONPRINT.
	#define Simplex_CALLFUNCTIONANDPRINT(x, pointerReturn, format) {Simplex_CALLFUNCTION(x, pointerReturn) Simplex_FUNCTIONPRINT(x, pointerReturn, format)}



	// Материал взят отсюда: https://docs.google.com/document/d/1FDIk30yvL9qWl7x6AWMDSHX6wCzaQEIFVrTddNiGejs/edit#heading=h.hxufmka5m960
	// Шаг 1. -----------------------

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
	for (unsigned char ii = length + 1; ii != (unsigned char)~0; ii--) {
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
	
	for (size_t i = length - 1; i != SIZE_MAX; i--) {
		for (size_t ii = length + 2 - 1; ii != 0; ii--) {
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
		for (unsigned char jj = length + 2 - 1; jj != (unsigned char)~0; jj--)
			free(x[jj]);
		free(memory1);
		free(memory2);
		return 3;
	}
	double * fvalue = memory3;
	double * E = memory3 + length + 2;
	double * fE = memory3 + length + 2 + length + 1;
	for (unsigned char ii = length + 2 - 1; ii != (unsigned char)~0; ii--)
		fvalue[ii] = nan(NULL);
	for (unsigned char ii = length + 1 - 1; ii != (unsigned char)~0; ii--)
		E[ii] = nan(NULL);
	double
		fvalue_center = nan(NULL),
		fvalue_minmax = nan(NULL), // Если isNeedMax = 0, то min. Если isNeedMax = 1, то хранит max.
		fvalue_maxmin = nan(NULL), // Если isNeedMax = 0, то max. Если isNeedMax = 1, то хранит min.
		d[] = { nan(NULL),
			nan(NULL) };
	int ferror = 0;

	// x[0] - начальные. x[1] ... x[length] - новые.
	// Шаг 2 ---------------------------------------------------------
	
	for (unsigned char i = 0; i < 2; i++)
		d[i] = (sqrt(length + 1) + i * length - 1)*edgeLength / (length*sqrt(2));

	for(size_t ii = 1; ii < length + 1; ii++)
		// Перебор векторов.
		for (size_t i = length - 1; i != SIZE_MAX; i--)
			// Перебор координат вектора
			if (i == ii - 1)
				x[ii][i] = x[0][i] + d[0];
			else
				x[ii][i] = x[0][i] + d[1];

	// Вычисление значения функции в точках x[1] ... x[length] и печать их -----------------------
	for (size_t ii = 0; ii < length + 1; ii++)
		Simplex_CALLFUNCTIONANDPRINT(x[ii], &(fvalue[ii]), ("%zu;\t", k++));
	// True, если надо продолжить.
	unsigned char need_continue = 0;
	do {
		// Шаг 3 --------------------------------------

		// Нам нужен максимум или минимум?
		// isNeedMax = 0 => Max. Иначе - min.
		// Содержит в себе индекс самого "противоположного" элемента.
		size_t maxminIndex;
		// Ищем самый ненужный элемент.
		if (ferror = Simplex_searchMinmax(&maxminIndex, fvalue, length + 1, !isNeedMax)) {
			Simplex_FREEALL;
			/* Печать отчёта об Simplex_searchMinmax. */ if (out != NULL) fprintf(out, "Simplex_searchMinmax: error %zu\n", ferror);
			return 7; }
		x_maxmin = x[maxminIndex]; fvalue_maxmin = fvalue[maxminIndex];
		/* Печать fvalue_maxmin. */ if (out != NULL) fprintf(out, "fvalue_maxmin: %lf\n", fvalue_maxmin);

		// Шаг 4. ------------------------------------
		// Поиск центра тяжести

		for (size_t i = length - 1; i != SIZE_MAX; i--) {
			x_center[i] = 0.0;
			for (size_t ii = length + 2 - 2; ii != SIZE_MAX; ii--) {
				if(ii != maxminIndex)
					// тут надо взять length наименьших (!isNeedMax) или length наибольших (isNeedMax)
					x_center[i] += x[ii][i]; 
			}
			x_center[i] /= length;
		}

		/* Печать f_center. */ if (out != NULL) { fprintf(out, "N;\t"); for (size_t i = 0; i < length; i++) fprintf(out, "x[%zu]=%0.3lf;\t", i, x_center[i]); fprintf(out, "f_center(...)=not need\n"); }

		// Шаг 5. --------------------------------------------------

		for (size_t i = length - 1; i != SIZE_MAX; i--)
			x[length + 2 - 1][i] = 2 * x_center[i] - x_maxmin[i]; // Последний элемент.

		// Печать значения функции x_new -------------------
		Simplex_CALLFUNCTIONANDPRINT(x[length + 2 - 1], &fvalue[length + 2 - 1], ("%zu;\t", k++));
		// Шаг 6. --------------------------------------------------

		size_t minmaxIndex;
		if (fvalue[length + 2 - 1] <= fvalue_maxmin)
		{
			x[maxminIndex] = x[length + 1];
			fvalue[maxminIndex] = fvalue[length + 1];
		}
		else
		{
			// Шаг 7.
			if (ferror = Simplex_searchMinmax(&minmaxIndex, x, length + 1, isNeedMax)) {
				Simplex_FREEALL;
				/* Печать отчёта об Simplex_searchMinmax. */ if (out != NULL) fprintf(out, "Simplex_searchMinmax: error %zu\n", ferror);
				return 7;
			}
			for (size_t vectorInd = length + 1 - 1; vectorInd != SIZE_MAX; vectorInd--)
				if (vectorInd != minmaxIndex)
					for (size_t dimInd = length - 1; dimInd != SIZE_MAX; dimInd--)
						x[vectorInd][dimInd] =
						(x[minmaxIndex][dimInd] + x[vectorInd][dimInd]) / 2.0;
		}

		// Шаг 8. Определение центра тяжести. -------------
		// Проверка, можем ли закончить алгоритм.

		for (size_t i = length - 1; i != SIZE_MAX; i--) {
			x_center[i] = 0.0;
			for (size_t ii = length + 1 - 1; ii != SIZE_MAX; ii--) {
					x_center[i] += x[ii][i];
			}
			x_center[i] /= length + 1;
		}
		Simplex_CALLFUNCTIONANDPRINT(x_center, &fvalue_center, "N;\t");

		// Шаг 9. -------------------------------------

		for (size_t ii = length + 1 - 1, i = 0; ii != SIZE_MAX; ii--)
				fE[i++] = fabs(fvalue[ii] - fvalue_center);
		/* Печать погрешностей. */ if (out != NULL) { for (size_t ii = length + 1 - 1, i = 0; ii != SIZE_MAX; ii--) fprintf(out, "E[%d]=%0.3lf\t", ii, fE[ii]); fprintf(out, "\n"); }

		// Готовим новый симплекс на тот случай, если не подходит ------

		// isNeedMax == True => надо отбросить самый маленький.
		// isNeedMax == False => надо отбросить самый максимальный.
		/* Печать оставшихся функций. */ if (out != NULL) { for (size_t ii = length + 1 - 1; ii != SIZE_MAX; ii--) { fprintf(out, "f[%d]=%0.3lf\t", ii, fvalue[ii]); } fprintf(out, "\n"); }

		need_continue = 0;
		for (size_t ii = length + 1 - 1; ii != SIZE_MAX; ii--) {
			need_continue = need_continue || fE[ii] >= accuracy;
		}

	} while (need_continue);

	// Запись ответа
	for (size_t i = 0; i < length; i++) {
		output[i] = x[length + 2 - 1][i];
	}

	Simplex_FREEALL;
	return 0;

	#undef Simplex_FREEALL
	#undef Simplex_CALLFUNCTION
	#undef Simplex_FUNCTIONPRINT
	#undef Simplex_CALLFUNCTIONANDPRINT
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
