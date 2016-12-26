#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	char *path = argv[1];
	char *path_ptr, *token, *rest_ptr;

	path_ptr = path + strlen("/data/data");
	token = strtok_r(path_ptr, "/", &rest_ptr);
	while (token) {
		printf("token is %s\n", token);
		token = strtok_r(rest_ptr, "/", &rest_ptr);
	}

	return 0;
}
