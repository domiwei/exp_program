#include <pthread.h>
#include <stdio.h>

pthread_t tid;

void tftn(void *ptr)
{
	if (pthread_equal(pthread_self(), tid))
		printf("the same\n");
	else
		printf("diff\n");
	return;
}

int main()
{
	pthread_create(&tid, NULL, (void *)&tftn, NULL);

	sleep(3);
	return 0;
}
