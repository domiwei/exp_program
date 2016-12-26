#include <stdio.h>
int foo(int a)
{
	printf("%d\n", a);
}

int main()
{
	int a = 5;
	int (*foo_ptr)(int);
	foo_ptr = foo;

	printf("a = %d\n", (******foo_ptr)(a));

	return (*******************************puts)("Hello"); 
	
}
