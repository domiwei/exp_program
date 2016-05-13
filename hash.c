#include <stdio.h>

unsigned long dbhash1(char *str)
{
	unsigned long long hash = 5381;
	int c;

	while(c = *str++)
		hash = (((hash<<5)+hash) + c);
	printf("hash num = %lld\n", hash);
	return hash % 57;
}

unsigned long dbhash2(char *input)
{
	unsigned hash = 5381;
	int index;

	index = 0;
	while(input[index]) { 
		hash = (((hash << 5) + hash + input[index]) % 57);
		index++;
	}

	return hash;


}

int main()
{
	char str[256];
	
	while(scanf("%s", str)){
		printf("hash1 = %d\n", dbhash1(str));
		printf("hash2 = %d\n", dbhash2(str));
	}
}
