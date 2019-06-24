#include "malloc.h"

int		main()
{
	char	*str;

	str = malloc(getpagesize()*350);
	free(str);
	str = malloc(getpagesize()*350);
	malloc(getpagesize()*350);
	malloc(getpagesize()*350);
	free(str);
	show_alloc_mem();
	return (0);
}