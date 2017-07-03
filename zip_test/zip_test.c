#include <zip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main()
{
	zip_t *src_zip, *tar_zip;
	int zip_errcode = 0;
	int num_entries = 0;
	int i;

	src_zip = zip_open("./base.apk", ZIP_CHECKCONS, &zip_errcode);
	tar_zip = zip_open("./target.apk", ZIP_CREATE, &zip_errcode);
	if (tar_zip == NULL)
		printf("failed to create zip\n");

	num_entries = zip_get_num_entries(src_zip, 0);
	printf("num of entries = %d\n", num_entries);

	for (i = 0; i < num_entries; i++) {
		char *file_name = (char *)zip_get_name(src_zip, i, 0);
		if (strstr(file_name, "abc_ic_menu")) {
		//if (strstr(file_name, "AndroidManifest.xml")) {
			zip_int64_t idx_in_base =
				zip_name_locate(src_zip, file_name, 0);
                	zip_source_t *tmp_zs_t =
                      		zip_source_zip(tar_zip, src_zip, i, 0, 0, 0);
			if (tmp_zs_t == NULL)
				printf("failed to source zip\n");
			if (zip_add(tar_zip, file_name, tmp_zs_t) < 0)
				printf("Failed to add zip\n");
			printf("%s, i = %d, idx = %lld\n", file_name, i, idx_in_base);
                }
	}

	if (zip_close(tar_zip) < 0) {
		printf("fuck tar --- %s!\n", zip_strerror(tar_zip));
	}
	if (zip_close(src_zip) < 0)
		printf("fuck src!\n");

	if (access("./target.apk", F_OK) < 0)
		printf("error code %d\n", errno);
	chmod("./target.apk", 0644);

	//tar_zip = zip_open("empty.zip", ZIP_CREATE, &zip_errcode);
	//zip_close(tar_zip);
	return 0;
}
