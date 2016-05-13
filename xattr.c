#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <attr/xattr.h>

int main()
{
	int size = 70000;
	char value[size];
	mknod("tmpfile", 0700, 0);
	int ret = setxattr("tmpfile", "user.a", value, size, XATTR_CREATE);
	if (ret < 0) {
		int errcode = errno;
		printf("errno = %d, %s\n", errcode, strerror(errcode));
		printf("size = %lld\n", size);
	} else
		printf("pass..1\n");
	ret = setxattr("tmpfile", "user.b", "", 0, 0);
	if (ret < 0) {
		int errcode = errno;
		printf("errno = %d, %s\n", errcode, strerror(errcode));
		printf("size = %lld\n", size);
	} else
		printf("pass..2\n");
	
	unlink("tmpfile");
}
