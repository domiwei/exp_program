#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
	char *pathname;
	size_t size = 1000;

	//symlink(file, "s1");
	while(errno != ENAMETOOLONG) {
		printf("size = %lld\n", size);
		pathname = (char *)malloc(sizeof(char) * size);
		memset(pathname, 'a', sizeof(char) * (size - 1));
		pathname[size - 1] = '\0';
		int ret = symlink(pathname, "s2");
		if (ret < 0) {
			int errcode = errno;
			printf("errno = %d, %s\n", errcode, strerror(errcode));
			printf("size = %lld\n", size);
			break;
		}
		free(pathname);
		unlink("s2");
		size++;
	}

	printf("sizeof(size_t) = %d\n", sizeof(size_t));
	//int len = readlink("s2", buf, sizeof(buf));
	//buf[len] = '\0';
	//printf("filename = %s\n", buf);
}
