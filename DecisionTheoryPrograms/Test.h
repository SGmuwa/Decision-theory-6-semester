#pragma once

#include <stdio.h>

int Test_assertEqualsInt(const wchar_t * message, int expect, int actual) {
	if (expect == actual)
		return 0;
	else {
		wprintf(L"%s\nexpect:%d\nactual:%d\n", message, expect, actual);
		return 1;
	}
}

int Test_assertEqualsDouble(const wchar_t * message, double expect, double actual, double accuracy) {
	if (accuracy < 0) {
		wprintf(L"accuracy must be 0 or more!\n%s\nexpect:%lf\nactual:%lf\naccuracy:%lf\n", message, expect, actual, accuracy);
		return 2;
	}
	if (actual - accuracy <= expect
		&& actual + accuracy >= expect)
		return 0;
	else {
		wprintf(L"%s\nexpect:%lf\nactual:%lf\naccuracy:%lf\n", message, expect, actual, accuracy);
		return 1;
	}
}
