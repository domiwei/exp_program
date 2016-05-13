#include <stdio.h>

int parse_http_header_retcode(FILE *fptr)
{
	char httpcode[20], retcode[20], retstatus[20];
	int ret, retcodenum;
	long ret_num;
	int errcode;
	char *endptr;

	fseek(fptr, 0, SEEK_SET);
	ret = fscanf(fptr, "%19s %19s %19s", httpcode, retcode, retstatus);
	if (ret < 3)
		return -1;

	ret_num = atol(retcode);
	retcodenum = (int)ret_num;
	printf("retstatus = %s\n", retstatus);

	return retcodenum;

errcode_handle:
	return -1;
}

int main()
{
	char buf[] = "HTTP/1.1 500 nternalServerErrorlallalalalallalalaa............................................................................\n";
	FILE *fptr = fopen("scan_testfile", "w+");

	setbuf(fptr, NULL);
	fwrite(buf, 1, strlen(buf), fptr);
	int ret = parse_http_header_retcode(fptr);
	printf("ret = %d\n", ret);

	close(fptr);
	unlink("scan_testfile");
	return 0;
}
