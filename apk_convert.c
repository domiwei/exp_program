#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int32_t _convert_origin_apk(char *apkname, const char *minapk_name)
{
	size_t name_len;

	name_len = strlen(minapk_name);

	/* Could not be an apk */
	if (name_len < 5)
		return -EINVAL;

	/* From .<x>min to <x>.apk */
	memcpy(apkname, minapk_name + 1, name_len - 4);
	memcpy(apkname + name_len - 4, ".apk", 4);
	apkname[name_len] = '\0';
	return 0;
}

int main(void)
{
	char *minapk = ".aab bmin";
	char apk[300];
	int ret;

	ret = _convert_origin_apk(apk, minapk);
	printf("ret = %d, apk name = %s\n", ret, apk);

}
