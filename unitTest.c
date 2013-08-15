#include <stdio.h>

int main(int argc, char *argv[]) {
	int tests = 0;
	int testsPassed = 0;
	testGetBytes(&tests, &testsPassed);
	testGetPixelAndPutPixel(&tests, &testsPassed);
	printf("Tests Attempted: %i\n", tests);
	printf("Tests Passed: %i\n", testsPassed);
	return 0;
}
