

static __attribute__((constructor)) void test()
{
	ino_t ino;
	int32_t i, ret;
	char name[10000];
	int32_t check[10000] = {0};

	create_minapk_table();
	printf("create :)\n");
	for (i = 0; i < 10000; i++) {
		sprintf(name, "name_%d", i);
		insert_minapk_data(i, name, i * 17);
		query_minapk_data(i, name, &ino);
		if (ino != (uint32_t)i * 17)
			printf("%d failed", i);

		ret = insert_minapk_data(i, name, i * 15);
		if (ret != -EEXIST)
			printf("%d failed", i);

		query_minapk_data(i, name, &ino);
		if (ino != (uint32_t)i * 17)
			printf("%d failed", i);
	}

	printf("iterate\n");
	init_iterate_minapk_table();
	int32_t count = 0;
	ino_t parent, self;
	while (iterate_minapk_table(&parent, name, &self) != -ENOENT) {
		check[parent] = 1;
		if (self != parent * 17)
			printf("%ld failed\n", parent);
		count ++;
	}
	for (i = 0; i < 10000; i++)
		if(check[i] != 1)
			printf("%d failed\n", i);

	end_iterate_minapk_table();
	if (count != 10000)
		printf("iterator failed\n");
	printf("iterate done\n");
	exit(0);


	for (i = 10000 - 1; i >= 0; i--) {
		sprintf(name, "name_%d", i);
		ret = remove_minapk_data(i, name);
		if (ret != 0)
			printf("%d failed", i);
		ret = query_minapk_data(i, name, &ino);
		if (ret != -ENOENT)
			printf("%d failed", i);

		ret = remove_minapk_data(i, name);
		if (ret != -ENOENT)
			printf("%d failed", i);

	// check structure
		int32_t j;
		if (i % 1000 == 0)
			printf("check %d\n", i);
		for (j = 0; j < i-1; j++) {
			sprintf(name, "name_%d", j);
			ret = query_minapk_data(j, name, &ino);
			if (ret != 0)
				printf("%d failed", j);

			if (ino != (uint32_t)j * 17)
				printf("%d failed", j);
		}
	}
	destroy_minapk_table();
	printf("destroy :)\n");
	exit(0);
	sleep(100);
}
