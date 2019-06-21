#include "malloc.h"

int		main()
{
	char	*str;

	malloc(524*getpagesize());
	malloc(211*getpagesize());
	malloc(524*getpagesize());
	malloc(211*getpagesize());
	malloc(524*getpagesize());
	malloc(211*getpagesize());
	show_alloc_mem();
	return (0);
}