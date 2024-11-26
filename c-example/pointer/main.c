#include <windows.h>
#include <stdio.h>

void more_pointer();

int main() {
	int a = 1;
	int b = 2;
	int c = 3;

	printf("value - a: %d, b: %d, c: %d\n", a, b, c);
	printf("address - a: %p, b: %p, c: %p\n\n", &a, &b, &c);

	more_pointer();
	return 0;
}

void more_pointer() {
	HMODULE base_mem = GetModuleHandle(0);
	printf("base mem: %p\n", base_mem);

	int val = 0xAAA;
	printf("val: 0x%X, address: %p\n", val, &val);

	// create(allocate) a new pointer variable to hold address of 'val'
	int* ptr = &val;
	printf("ptr: %p, address: %p\n", ptr, &ptr);

	// pointer derefence
	printf("value of *ptr: 0x%X\n", *ptr);

	return;
}