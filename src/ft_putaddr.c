#include "malloc.h"

void	ft_putaddr(uint64_t addr)
{
	if (addr > 16)
	{
		ft_putaddr(addr / 16);
		ft_putaddr(addr % 16);
	}
	else if (addr < 10)
		ft_putchar(addr + '0');
	else
		ft_putchar(addr + 'a' - 10);
}
