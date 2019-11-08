#include <stdlib.h>
#include <stdio.h>

int		main()
{
	char *str;

	str = (char*)calloc(sizeof(char) * 10, 1);
	for (int i = 0; i < 10; i++)
		printf("%d\n", (int)str[i]);
	return 0;
}
