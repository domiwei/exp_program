#include <stdio.h>

typedef struct delta {
	int da;
	int db;
	int dc;
} Delta = {0,0,0};

int update_delta(Delta delta)
{
	printf("da = %d\n", delta.da);
	printf("db = %d\n", delta.db);
	printf("dc = %d\n", delta.dc);
	return 0;
}

int main()
{
	update_delta((Delta) {.db = 5, .dc = 7, .da = 3});
}
