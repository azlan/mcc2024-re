#include <stdio.h>

int calculate(int a, int b);
int calculate2(int a, int b);
int calculate3(int a, int b, int c);

int main() {
	int sum = 0;
	int result = calculate(5, 6);
	sum += result;

	printf("sum: %d\n", sum);

	return 0;
}

int calculate(int a, int b) {
	int result = a + b - 2;
	result += calculate2(result, 10);

	return result;
}

int calculate2(int a, int b) {
	int result = calculate3(a, 8, b);
	result -= a + b * 2;

	return result;
}

int calculate3(int a, int b, int c) {
	int x = 3 + a;
	int y = 4;
	int mul = x * y;
	int result = a * b * 2 + c;
	result -= mul;

	return result;
}