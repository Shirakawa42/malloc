#include "malloc.h"

int		main()
{
	char	*str;
	char	*str2;

	malloc(250);
	malloc(500);
	str = malloc(1000);
	str2 = malloc(2000);
	malloc(3000);
	free(str);
	free(str2);
	malloc(3000);
	show_alloc_mem();
	return (0);
}