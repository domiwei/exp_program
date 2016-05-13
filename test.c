#include <stdio.h>
#define macro(x) \
	static int a;\
	a = foo(x); \
	if(a==1) \
		printf("yaya\n"); \
	else \
		printf("%d\n", foo(x));


int foo(int k)
{
	return k==0 ? -1: 1;
}

int main()
{
	macro(0);
	macro(1);
}
