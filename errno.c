#include <errno.h>
#include <sys/types.h>

int main()
{
	ssize_t size;

	size = -EIO;

	printf("size = %zu\n", size);
}
