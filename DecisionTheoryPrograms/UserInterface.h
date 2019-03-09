#pragma once
/*
Михаил Павлович Сидоренко ([SG]Muwa https://github.com/SGmuwa)
Российский технологийческий университет
Версия 1.2

Данный файл создан для упрощения работы программиста с консолью
с помощью стандартных средств работы с консолью и файлами.
*/

#ifndef _USERINTERFACE_H_
#define _USERINTERFACE_H_
#include <stdio.h>
#include <math.h>

#ifdef CPP

#endif // CPP

#ifdef __cplusplus
extern "C" {
#endif

	// ------------------------------- FILE -------------------------------------------------

	// Ожидает ответа от потока ввода.
	// const char * message: Сообщение, которое будет выведено в поток вывода. В случае, если не надо выводить, следует отправить NULL.
	// FILE * fpIN: Поток ввода информации. Программа ждёт команды от этого потока. Если отправить NULL, программа сделает паузу на пересчёт sizeof(int) байт информации.
	// FILE * fpOUT: Поток вывода информации. Если отправить NULL, сообщение message не будет отправлено.
	void UserInterface_fPause(const char * message, FILE * fpIN, FILE * fpOUT)
	{
#ifdef _MSC_VER
		if (fpOUT != NULL && message != NULL)
			fprintf_s(fpOUT, "%s", message);
		if (fpIN == NULL)
			for (unsigned int i = ~0u; i != 0; --i);
		else
			fscanf_s(fpIN, "%*c");
#else
		if (fpOUT != NULL && message != NULL)
			fprintf(fpOUT, "%s", message);
		if (fpIN == NULL)
			for (unsigned int i = ~0u; i != 0; --i);
		else
			fscanf(fpIN, "%*c");
#endif // _MSC_VER
	}

	// Функция получает неотрицательное число от потока ввода. Если пользователь ошибётся 255 раз, то функция вернёт ~0.
	// const char * message: Сообщение, которое будет выведено перед запросом числа. Если отправить NULL, сообщение не будет выведено.
	// FILE * fpIN: Поток ввода информации. Отсюда будет получено неотрицательное число.
	// FILE * fpOUT: Поток вывода информации. Сюда отправится message. Если будет NULL, то сообщение не будет отправлено.
	// Возвращает: Неотрицательное число, введённое пользователем.
	unsigned UserInterface_fGetUnsignedInt(const char * message, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return 0;
		unsigned buffer = ~0u;
		unsigned char tryLimit = 0;
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL && message != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%u", &buffer)
#else
				fscanf(fpIN, "%u", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				)
			{
				break;
			}
#ifdef _MSC_VER
			fscanf_s(fpIN, "%*s");
#else
			fscanf(fpIN, "%*s");
#endif // _MSC_VER
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}

	// Функция получает неотрицательное число от потока ввода. Функция вернёт ~0 если не удалось прочитать число 255 раз.
	// const char * message: Сообщение, которое будет выведено перед запросом числа. Если отправить NULL, сообщение не будет выведено.
	// FILE * fpIN: Поток ввода информации. Отсюда будет получено неотрицательное число.
	// FILE * fpOUT: Поток вывода информации. Сюда отправится message. Если будет NULL, то сообщение не будет отправлено.
	// Возвращает: Неотрицательное число, введённое пользователем.
	unsigned long UserInterface_fGetUnsignedLongInt(const char * message, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return 0;
		unsigned long buffer = ~0UL;
		unsigned char tryLimit = 0;
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL && message != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%lu", &buffer)
#else
				fscanf(fpIN, "%lu", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				)
			{
				break;
			}
#ifdef _MSC_VER
			fscanf_s(fpIN, "%*s");
#else
			fscanf(fpIN, "%*s");
#endif // _MSC_VER
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}

	// Функция получает неотрицательное число от потока ввода. Функция вернёт ~0 если не удалось прочитать число 255 раз.
	// const char * message: Сообщение, которое будет выведено перед запросом числа. Если отправить NULL, сообщение не будет выведено.
	// FILE * fpIN: Поток ввода информации. Отсюда будет получено неотрицательное число.
	// FILE * fpOUT: Поток вывода информации. Сюда отправится message. Если будет NULL, то сообщение не будет отправлено.
	// Возвращает: Неотрицательное число, введённое пользователем.
	unsigned long long UserInterface_fGetUnsignedLongLongInt(const char * message, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return 0;
		unsigned long long buffer = ~0ULL;
		unsigned char tryLimit = 0;
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL && message != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%llu", &buffer)
#else
				fscanf(fpIN, "%llu", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				)
			{
				break;
			}
#ifdef _MSC_VER
			fscanf_s(fpIN, "%*s");
#else
			fscanf(fpIN, "%*s");
#endif // _MSC_VER
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}

	// Получает от fpIN число с плавающей точкой.
	// const char * message: Сообщение, которое необходимо вывести. В Случае NULL сообщение не будет выведено.
	// FILE * fpIN: Поток ввода информация. В случае NULL функция вернёт NaN.
	// FILE * fpOUT: Поток вывода информации. В случае NULL сообщение не будет выведено.
	// Возвращает: прочитанное от пользователя число.
	// Ошибки: если пользователь ошибётся 255 раз, то функция вернёт NaN.
	float UserInterface_fGetFloat(const char * message, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return nanf(NULL);
		unsigned char tryLimit = 0;
		float buffer = nanf(NULL);
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%f", &buffer)
#else
				fscanf(fpIN, "%f", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				)
			{
				break;
			}
#ifdef _MSC_VER
			fscanf_s(fpIN, "%*s");
#else
			fscanf(fpIN, "%*s");
#endif // _MSC_VER
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}

	// Получает от fpIN число с плавающей точкой двойной точности.
	// const char * message: Сообщение, которое необходимо вывести. В Случае NULL сообщение не будет выведено.
	// FILE * fpIN: Поток ввода информация. В случае NULL функция вернёт NaN.
	// FILE * fpOUT: Поток вывода информации. В случае NULL сообщение не будет выведено.
	// Возвращает: прочитанное от пользователя число.
	// Ошибки: если пользователь ошибётся 255 раз, то функция вернёт NaN.
	double UserInterface_fGetDouble(const char * message, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return nan(NULL);
		unsigned char tryLimit = 0;
		double buffer = nan(NULL);
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%lf", &buffer)
#else
				fscanf(fpIN, "%lf", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				)
			{
				break;
			}
#ifdef _MSC_VER
			fscanf_s(fpIN, "%*s");
#else
			fscanf(fpIN, "%*s");
#endif // _MSC_VER
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}

	// Запрашивает текстовые данные от пользователя. Для того, чтобы закончить набирать текстовую информацию, пользователю необходимо отправить любой из следующий символов: \0 \1 \2 \3 \4 \5 \6 \7 \8.
	// const char * message: Сообщение, которое нужно вывести перед запросом. В случае NULL сообщение не будет выведено.
	// char * To: Указатель на первый доступный символ для записи. В случае NULL функция вернёт 0.
	// size_t countTo: Количество доступных символов для записи в To; В случае 0 функция выведет message и вернёт 0.
	// FILE * fpIN: Поток ввода информации. Отсюда будет браться текст. В случае NULL функция вернёт 0.
	// FILE * fpOUT: Поток вывода информации. В случае NULL сообщение выведено не будет.
	// Возвращает: количество прочтённых символов.
	size_t UserInterface_fGetStrln(const char * message, char * To, size_t countTo, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return 0u;
		if (fpOUT != NULL && message != NULL) fprintf(fpOUT, "%s Use CTRL + 2 or CTRL + A, CTRL + B, CTRL + D for end.\n", message);
		if (To == NULL) return 0u;
		if (countTo == 0u) return 0u;
		char * i = To - 1; // Уменьшаем на 1, так как в цикле будет прибавление
		do {
#ifdef _MSC_VER // Для компилятора Visual Studio
			fread_s(++i, i - To + 1, sizeof(char), 1, fpIN);
#else // Для компиляторов ANSI C.
			fread(++i, sizeof(char), 1, fpIN); // На каждой новой итерации прибавляем i и записываем 1 символ в To.
#endif // _MSC_VER
		} while (*i >= 0x09 && i != To + countTo - 2); // -1 из-за того, что sizeof возвращает количество, а не последний элемент.
													   // -1 из-за того, чтобы не вошёл в цикл и не стал записьвать в 256 элемент.
													   // неактуально: -1 из-за того, чтобы добавить ещё нулевой символ.
													   // Цикл повторяется, пока пользователь не введёт какой-либо управляющий символ.
													   // https://ru.wikipedia.org/wiki/Управляющие_символы

		*(i) = '\0'; // Добавляем последний символ.
		return i - To + 1; // +1 так как нужно количество элементов, а не расстояние между первым и последним.
	}

	// Запрашивает текстовые данные от пользователя. Для того, чтобы закончить набирать текстовую информацию, пользователю необходимо отправить любой из следующий символов: \0 \1 \2 \3 \4 \5 \6 \7 \8 ... \31.
	// const char * message: Сообщение, которое нужно вывести перед запросом. В случае NULL сообщение не будет выведено.
	// char * To: Указатель на первый доступный символ для записи. В случае NULL функция вернёт 0.
	// size_t countTo: Количество доступных символов для записи в To; В случае 0 функция выведет message и вернёт 0.
	// FILE * fpIN: Поток ввода информации. Отсюда будет браться текст. В случае NULL функция вернёт 0.
	// FILE * fpOUT: Поток вывода информации. В случае NULL сообщение выведено не будет.
	// Возвращает: количество прочтённых символов.
	size_t UserInterface_fGetStr(const char * message, char * To, size_t countTo, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return 0u;
		if (fpOUT != NULL && message != NULL) fprintf(fpOUT, "%s", message);
		if (To == NULL) return 0u;
		if (countTo == 0u) return 0u;
		char * i = To - 1; // Уменьшаем на 1, так как в цикле будет прибавление
		do {
#ifdef _MSC_VER // Для компилятора Visual Studio
			fread_s(++i, i - To + 1, sizeof(char), 1, fpIN);
#else // Для компиляторов ANSI C.
			fread(++i, sizeof(char), 1, fpIN); // На каждой новой итерации прибавляем i и записываем 1 символ в To.
#endif // _MSC_VER
		} while (*i >= 32 && i != To + countTo - 2); // -1 из-за того, что sizeof возвращает количество, а не последний элемент.
													   // -1 из-за того, чтобы не вошёл в цикл и не стал записьвать в 256 элемент.
													   // неактуально: -1 из-за того, чтобы добавить ещё нулевой символ.
													   // Цикл повторяется, пока пользователь не введёт какой-либо управляющий символ.
													   // https://ru.wikipedia.org/wiki/Управляющие_символы

		*(i) = '\0'; // Добавляем последний символ.
		return i - To + 1; // +1 так как нужно количество элементов, а не расстояние между первым и последним.
	}

	// Получает из заданного потока целое знаковое число.
	// const char * message: сообщение, которое будет отправлено перед запросом числа. Оно не защищено от NULL.
	// FILE * fpIN: Поток ввода информации, откуда получается целое знаковое число. Если отправить NULL, функция вернёт 0, не отправляя message.
	// FILE * fpOUT: Поток вывода информации. Если равно NULL, то сообщение message не будет напечатано.
	int UserInterface_fGetInt(const char * message, FILE * fpIN, FILE * fpOUT)
	{
		if (fpIN == NULL) return 0;
		signed buffer = 0;
		unsigned char tryLimit = 0;
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%d", &buffer)
#else
				fscanf(fpIN, "%d", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				)
			{
				break;
			}
#ifdef _MSC_VER
			fscanf_s(fpIN, "%*s");
#else
			fscanf(fpIN, "%*s");
#endif // _MSC_VER
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}

	// Спрашивает у пользователя 1 байт информации от 0 включительно до максимально допустимого.
	// const char * message: Сообщение, которое будет выведено перед запросом. Если отправить NULL, сообщение не будет выведено.
	// FILE * fpIN: Поток ввода информации, откуда байт считывается.
	// FILE * fpOUT: Поток вывода информации, куда будет отправлен message.
	// Возвращает: Целое неотрицательное число, которое было получено из потока ввода.
	unsigned char UserInterface_fGetChek(const char * message, unsigned char maxAccess, FILE * fpIN/* = stdin*/, FILE * fpOUT/* = stdout*/)
	{
		unsigned char buffer = ~(unsigned char)0u;
		unsigned char tryLimit = 0;
		if (fpIN == NULL) return 0;
		while (--tryLimit != 0)
		{
			if (fpOUT != NULL && message != NULL)
#ifdef _MSC_VER
				fprintf_s(fpOUT, "%s", message);
#else
				fprintf(fpOUT, "%s", message);
#endif // _MSC_VER
			if (
#ifdef _MSC_VER
				fscanf_s(fpIN, "%hhu", &buffer)
#else
				fscanf(fpIN, "%hhu", &buffer)
#endif // _MSC_VER
				>= 1 // Не совсем уверен, как работает %*s. Поэтому знак >=.
				&& // Сработает, если компилатор проверяет слева на право выражения. Иначе: будет баг в использовании.
				buffer <= maxAccess)
			{
				break;
			}
			if (buffer == ~'\0')
			{
#ifdef _MSC_VER
				fscanf_s(fpIN, "%*s");
#else
				fscanf(fpIN, "%*s");
#endif // _MSC_VER
			}
			buffer = 255u;
		}
#ifdef _MSC_VER
		fscanf_s(fpIN, "%*c");
#else
		fscanf(fpIN, "%*c");
#endif // _MSC_VER
		return buffer;
	}


	// --------------------------- STDIN / STDOUT ---------------------------------------------


	// Функция получает неотрицательное число от потока ввода. Функция вернёт ~0 если не удалось прочитать число 255 раз.
	// const char * message: Сообщение, которое будет выведено перед запросом числа. Если отправить NULL, сообщение не будет выведено.
	// Возвращает: Неотрицательное число, введённое пользователем.
	unsigned long UserInterface_GetUnsignedLongInt(const char * message)
	{
		return UserInterface_fGetUnsignedLongInt(message, stdin, stdout);
	}

	// Функция получает неотрицательное число от потока ввода. Функция вернёт ~0 если не удалось прочитать число 255 раз.
	// const char * message: Сообщение, которое будет выведено перед запросом числа. Если отправить NULL, сообщение не будет выведено.
	// Возвращает: Неотрицательное число, введённое пользователем.
	unsigned long long UserInterface_GetUnsignedLongLongInt(const char * message)
	{
		return UserInterface_fGetUnsignedLongLongInt(message, stdin, stdout);
	}


	// Функция получает неотрицательное число от потока ввода.
	// const char * message: Сообщение, которое будет выведено перед запросом числа. Если отправить NULL, сообщение не будет выведено.
	// Возвращает: Неотрицательное число, введённое пользователем.
	unsigned UserInterface_GetUnsignedInt(const char * message)
	{
		return UserInterface_fGetUnsignedInt(message, stdin, stdout);
	}

	// Получает от стандартного ввода-вывода число с плавающей точкой одинарной точности.
	// const char * message: Сообщение, которое необходимо вывести. В случае NULL сообщение не будет выведено.
	// Возвращает: прочитанное от пользователя число.
	// Ошибки: если пользователь ошибётся 255 раз, то функция вернёт NaN.
	float UserInterface_GetFloat(const char * message)
	{
		return UserInterface_fGetFloat(message, stdin, stdout);
	}

	// Получает от стандартного ввода-вывода число с плавающей точкой двойной точности.
	// const char * message: Сообщение, которое необходимо вывести. В случае NULL сообщение не будет выведено.
	// Возвращает: Прочитанное от пользователя число.
	// Ошибки: если пользователь ошибётся 255 раз, то функция вернёт NaN.
	double UserInterface_GetDouble(const char * message)
	{
		return UserInterface_fGetDouble(message, stdin, stdout);
	}

	// Получает от стандартного ввода-вывода число с плавующей точкой двойной точности в заданных границах.
	// const char * message: Сообщение, которое необходимо вывести. В случае NULL сообщение не будет выведено.
	// double min: Минимальное разрешённое число для ввода. Если отправить NaN, функция вернёт NaN без вывода сообщения. Если минимальное число больше максимального, то возвращается NaN.
	// double max: Максимальное разрешённое число для ввода. Если отправить NaN, функция вернёт NaN без вывода сообщения.
	// Возвращает: Прочитанное от пользователя число. 
	// Ошибки: если пользователь ошибётся 255 раз, то функция вернёт NaN.
	double UserInterface_GetDoubleLimit(const char * message, double min, double max)
	{
		if (isnan(min) || isnan(max) || min > max)
			return nan(NULL);
		double buffer;
		do
		{
			buffer = UserInterface_GetDouble(message);
		} while (buffer < min || buffer > max || isnan(buffer));
		return buffer;
	}

	// Запрашивает текстовые данные от пользователя через стандартные потоки ввода-вывода. Для того, чтобы закончить набирать текстовую информацию, пользователю необходимо отправить любой из следующий символов: \0 \1 \2 \3 \4 \5 \6 \7 \8.
	// const char * message: Сообщение, которое нужно вывести перед запросом. В случае NULL сообщение не будет выведено.
	// char * To: Указатель на первый доступный символ для записи. В случае NULL функция вернёт 0.
	// size_t countTo: Количество доступных символов для записи в To; В случае 0 функция выведет message и вернёт 0.
	// Возвращает: количество прочтённых символов.
	size_t UserInterface_GetStrln(const char * message, char * To, size_t countTo)
	{
		return UserInterface_fGetStrln(message, To, countTo, stdin, stdout);
	}

	// Запрашивает текстовые данные от пользователя через стандартные потоки ввода-вывода. Для того, чтобы закончить набирать текстовую информацию, пользователю необходимо отправить любой из следующий символов: \0 \1 \2 \3 \4 \5 \6 \7 \8 ... 0x7F.
	// const char * message: Сообщение, которое нужно вывести перед запросом. В случае NULL сообщение не будет выведено.
	// char * To: Указатель на первый доступный символ для записи. В случае NULL функция вернёт 0.
	// size_t countTo: Количество доступных символов для записи в To; В случае 0 функция выведет message и вернёт 0.
	// Возвращает: количество прочтённых символов.
	size_t UserInterface_GetStr(const char * message, char * To, size_t countTo)
	{
		return UserInterface_fGetStr(message, To, countTo, stdin, stdout);
	}

	// Получает от пользователя unsigned int число в допустимом диапазоне.
	// const char * message: сообщение, которое будет выведено перед запросом.
	// unsigned min: минимально допустимое число, которое будет возвращено. Если min > max, то будет возвращено min без ожидания ответа пользователя и не печатая message.
	// unsigned max: максимально допустимое число, которое будет возвращено.
	// Возвращает: unsigned int от пользователя.
	unsigned UserInterface_GetUnsignedIntLimit(const char * message, unsigned min, unsigned max)
	{
		if (min > max) return min;
		unsigned buffer;
		do
		{
			buffer = UserInterface_GetUnsignedInt(message);
		} while (buffer < min || buffer > max);
		return buffer;
	}

	// Получает от пользователя int число в допустимом диапазоне.
	// const char * message: сообщение, которое будет выведено перед запросом.
	// int min: минимально допустимое число, которое будет возвращено. Если min > max, то будет возвращено min без ожидания ответа пользователя и не печатая message.
	// int max: максимально допустимое число, которое будет возвращено.
	// Возвращает: unsigned int от пользователя.
	int UserInterface_GetIntLimit(const char * message, int min, int max)
	{
		if (min > max) return min;
		int buffer;
		do
		{
			buffer = UserInterface_GetUnsignedInt(message);
		} while (buffer < min || buffer > max);
		return buffer;
	}

	// Получает от пользователя unsigned long long int число в допустимом диапазоне.
	// const char * message: сообщение, которое будет выведено перед запросом.
	// unsigned long long min: минимально допустимое число, которое будет возвращено. Если min > max, то будет возвращено min без ожидания ответа пользователя и не печатая message.
	// unsigned long long max: максимально допустимое число, которое будет возвращено.
	// Возвращает: unsigned int от пользователя.
	unsigned long long UserInterface_GetUnsignedLongLongIntLimit(const char * message, unsigned long long min, unsigned long long max)
	{
		if (min > max) return min;
		unsigned long long int buffer;
		do
		{
			buffer = UserInterface_GetUnsignedLongLongInt(message);
		} while (buffer < min || buffer > max);
		return buffer;
	}

	// Ожидает ответа от пользователя.
	// const char * message: Сообщение, которое будет выведено перед паузой. Если отправить NULL, сообщение не будет выведено.
	void UserInterface_Pause(const char * message)
	{
		UserInterface_fPause(message, stdin, stdout);
	}

	FILE * UserInterface_OpenFile(const char * message, const char * mode)
	{
		if (mode == NULL) return NULL;
		FILE * output;
		char buffer[1024];
		do {
			UserInterface_GetStr(message, buffer, sizeof(buffer));
			if (buffer[0] == 'c' && buffer[1] == 'o' && buffer[2] == 'n')
				if (mode[0] == 'r')
					return stdin;
				else if (mode[0] == 'w')
					return stdout;
		} while (
#ifdef _MSC_VER
			fopen_s(&output, buffer, mode) != 0);
#else
			(output = fopen(buffer, mode)) != NULL);
#endif
		return output;
	}

	// Спрашивает у пользователя 1 байт информации от 0 включительно до максимально допустимого.
	// const char * message: Сообщение, которое будет выведено перед запросом. Если отправить NULL, сообщение не будет выведено.
	// Возвращает: Целое неотрицательное число, которое было получено из потока ввода.
	unsigned char UserInterface_GetChek(const char * message, unsigned char maxAccess)
	{
		return UserInterface_fGetChek(message, maxAccess, stdin, stdout);
	}

#ifdef __cplusplus
}
#endif

#endif //_USERINTERFACE_H_
