#define TEST 1

#if TEST == 1
#include "simplex_test.h"
#else
int Simplex_test_main(void) { return 0; }
#endif // TEST = 1

#include "UserInterface.h"
#include "simplex_console.h"
#include <locale.h>

/*
Печатает результаты тестирования.
*/
int Program_printTestResult(void) {
	FILE * testFp = NULL;
#ifdef _MSC_VER
	fopen_s(&testFp, "test.log", "r");
#else
	testFp = fopen("test.log", "r");
#endif
	if (testFp == NULL)
		return 1;
	fseek(testFp, 0, SEEK_END);
	size_t size = ftell(testFp) + 1;
	fseek(testFp, 0, SEEK_SET);
	char * buffer = malloc(size);
	if (buffer == NULL)
	{
		while (!feof(testFp))
		{
			fread(&buffer, sizeof(char), 1, testFp);
			printf("%c", (char)buffer);
		}
	}
	else
	{
		size = fread(buffer, sizeof(char), size, testFp);
		buffer[size] = 0;
		printf("%s", buffer);
		free(buffer);
	}
	fclose(testFp);
	return 0;
}

int Program_menu(void) {
	int error = 0;
	printf("%s", "Hello, world!1\n");
	switch (UserInterface_GetChek("Выберите вариант ответа:\n0 - выход.\n1 - симплекс.\n2 - тестирования\n", 2))
	{
	case 1:
		error = Simplex_Console_main();
		break;
	case 2:
		if (Program_printTestResult())
			printf("Не удалось получить результаты тестирований.");
		break;
	default:
		return 1;
	}
	if(error != 0)
		printf("error exit: %d\n", error);
	return 0;
}

int Program_main(int argc, char argv[]) {
	setlocale(LC_ALL, "ru");
	int error = Simplex_test_main();
	while (Program_menu() == 0);
	return error;
}

int main(int argc, char argv[]) {
	scanf("%*s");
	return Program_main(argc, argv);
}

