#include <stdio.h>
extern int Simplex_test_main();

int main(int argc, char argv[]) {
	int err = Simplex_test_main();
	scanf_s("%*s");
	return err;
}

