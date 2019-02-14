#include "simplex_test.h"
#include "UserInterface.h"
#include <locale.h>

int Program_menu(void) {
	switch (UserInterface_GetChek("Выберите вариант ответа:\n0 - выход.", 0))
	{
	default:
		return 0;
	}
}

int Program_main(int argc, char argv[]) {
	setlocale(LC_ALL, "rus");
	int error = Simplex_test_main();
	while (Program_menu());
	return error;
}

int main(int argc, char argv[]) {
	return Program_main(argc, argv);
}

