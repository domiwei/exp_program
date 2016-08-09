#include <sys/types.h>
#include <attr/xattr.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define assert_eq(TEST, A, B)\
	if ((A) == (B)) {\
		printf("assert-%s pass: %s == %s\n", #TEST, #A, #B);\
	}else{\
		printf("assert-%s failed: line %d. %s = %d. %s = %d\n", #TEST, __LINE__, #A, A, #B, B);\
		exit(-1);\
	}


int main()
{
	char *filepath = "/root/mp/aaa";
	char buf[3000];
	int size;
	int ret_size;

	if (!access(filepath, F_OK))
		unlink(filepath);
	mknod(filepath, 0700, 0);

	/* size = 0, return needed size */
	size = 0;
	ret_size = listxattr(filepath, buf, size);
	assert_eq(NoXattr_GetSelKeySizeSucess, ret_size, strlen("security.selinux") + 1);

	/* buf size is not sufficient, return ERANGE */
	ret_size = listxattr(filepath, buf, ret_size - 1);
	assert_eq(NoXattr_ListSelKeyFail, ret_size, -1);
	assert_eq(NoXattr_SelKeyRangeError, errno, ERANGE);

	/*success*/
	ret_size = listxattr(filepath, buf, ret_size);
	assert_eq(NoXattr_ListSelKeySuccess, ret_size, strlen("security.selinux") + 1);
	if (strcmp("security.selinux", buf) == 0)
		printf("list selinux pass\n");
	else
		printf("error: return xattr = %s\n", buf);


	/*********set another xattr**********/
	ret_size = setxattr(filepath, "user.a", "aa", 2, 0);
	assert_eq(XattrExist_SetXattrSuccess, ret_size, 0);

	/* size = 0, return needed size */
	size = 0;
	ret_size = listxattr(filepath, buf, size);
	assert_eq(XattrExist_ListXattrGetSize, ret_size, strlen("user.a") + strlen("security.selinux") + 2);

	/* buf size is not sufficient, return ERANGE */
	ret_size = listxattr(filepath, buf, ret_size - 1);
	assert_eq(XattrExist_ListRangeError, ret_size, -1);
	assert_eq(XattrExist_ListRangeError, errno, ERANGE);

	/*success*/
	ret_size = listxattr(filepath, buf, ret_size);
	assert_eq(AAA, ret_size, strlen("user.a") + strlen("security.selinux") + 2);
	char str[300] = {0};
	memcpy(str, "user.a", strlen("user.a"));
	memcpy(str + strlen("user.a") + 1, "security.selinux",
		strlen("security.selinux"));
	if (!memcmp(str, buf, strlen("user.a") + strlen("security.selinux") + 2))
		printf("Get xattr pass(user & security)\n");
	else
		printf("Get xattr FAILED!(user & security)\n");


	/*********remove xattr user.a**********/
	ret_size = removexattr(filepath, "user.a");
	assert_eq(AAA, ret_size, 0);

	/* size = 0, return needed size */
	size = 0;
	ret_size = listxattr(filepath, buf, size);
	assert_eq(AAA, ret_size, strlen("security.selinux") + 1);

	/* buf size is not sufficient, return ERANGE */
	ret_size = listxattr(filepath, buf, ret_size - 1);
	assert_eq(AAA, ret_size, -1);
	assert_eq(AAA, errno, ERANGE);

	/*success*/
	ret_size = listxattr(filepath, buf, ret_size);
	assert_eq(AAA, ret_size, strlen("security.selinux") + 1);
	if (strcmp("security.selinux", buf) == 0)
		printf("list selinux pass\n");
	else
		printf("error: return xattr = %s\n", buf);
}
