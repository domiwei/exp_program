#include <stdio.h>
#include <string.h>

int main()
{
	char c[101], d[101], ans[102];
	int c_len, d_len;

	printf("enter a number\n");
	gets(c);
	printf("enter another number\n");
	gets(d);
	c_len = strlen(c);
	d_len = strlen(d);

	int count = 0;
	int carry = 0;
	while (1) {
		int num_c;
		int num_d;
		int index_c, index_d;

		index_c = c_len - 1 - count;
		index_d = d_len - 1 - count;
		if (index_c < 0 && index_d < 0) {
			if (carry > 0) {
				ans[count] = carry + '0';
				count++;
			}
			break;
		}
		/* index of c */
		if (index_c >= 0)
			num_c = c[index_c] - '0';
		else
			num_c = 0;
		
		/* index of d */
		if (index_d >= 0)
			num_d = d[index_d] - '0';
		else
			num_d = 0;
		/* Add */
		ans[count] = ((num_c + num_d) + carry) % 10 + '0';
		carry = (num_c + num_d + carry) / 10;
		count++;
	}
	ans[count] = '\0';

	int i;
	for (i = 0; i < count / 2; i++) {
		char tmp;
		tmp = ans[i];
		ans[i] = ans[count - 1 - i];
		ans[count - 1 - i] = tmp;
	}

	printf("%s, len = %d, %s, len = %d\n", c, c_len, d, d_len);
	printf("%s\n", ans);
	return 0;
}

