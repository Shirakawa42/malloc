#include "malloc.h"

void	*allocate(size_t size)
{
	return (mmap(NULL, size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0));
}

size_t	zonetopage(void)
{
	size_t		ret;
	size_t		zonesize;

	ret = 1;
	zonesize = sizeof(t_zone) * 100;
	while (zonesize > PAGE_SIZE)
	{
		zonesize -= PAGE_SIZE;
		ret++;
	}
	return (ret);
}

size_t	largetopage(size_t size)
{
	size_t		ret;

	ret = (size_t)((size + sizeof(t_zone)) / PAGE_SIZE);
	if ((size + sizeof(t_zone)) % PAGE_SIZE != 0)
		ret++;
	return (ret * PAGE_SIZE);
}
