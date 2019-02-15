#define TEST 1

#if TEST == 1
#include "simplex_test.h"
#else
int Simplex_test_main(void) { return 0; }
#endif // TEST = 1

#include "UserInterface.h"
#include "simplex_console.h"
#include <locale.h>

int Program_menu(void) {
	int error = 0;
	switch (UserInterface_GetChek("Выберите вариант ответа:\n0 - выход.\n1 - симплекс.\n", 1))
	{
	case 1:
		error = Simplex_Console_main();
		break;
	default:
		return 1;
	}
	if(error != 0)
		printf("error exit: %d\n", error);
	return 0;
}

int Program_main(int argc, char argv[]) {
	setlocale(LC_ALL, "rus");
	int error = Simplex_test_main();
	while (Program_menu() == 0);
	return error;
}

int main(int argc, char argv[]) {
	return Program_main(argc, argv);
}

