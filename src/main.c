#include "malloc.h"
#include <stdint.h>
#include <stdio.h>

void	show_alloc_mem(void)
{
	t_zone		*zone;
	uint64_t	total;

	total = 0;
	if (g_e.tiny)
	{
		ft_putstr("TINY : 0x");
		ft_putaddr((uint64_t)g_e.tiny);
		ft_putchar('\n');
		zone = g_e.tiny;
		while (zone)
		{
			if (zone->free == 0)
			{
				ft_putstr("0x");
				ft_putaddr((uint64_t)((t_zone*)((char*)(zone + 1))));
				ft_putstr(" - 0x");
				ft_putaddr((uint64_t)((t_zone*)((char*)(zone + 1)
						+ zone->size)));
				ft_putstr(" : ");
				ft_putnbr(zone->size);
				ft_putstr(" octets\n");
				total += zone->size;
			}
			zone = zone->next;
		}
	}
	if (g_e.medium)
	{
		ft_putstr("MEDIUM : 0x");
		ft_putaddr((uint64_t)g_e.medium);
		ft_putchar('\n');
		zone = g_e.medium;
		while (zone)
		{
			if (zone->free == 0)
			{
				ft_putstr("0x");
				ft_putaddr((uint64_t)((t_zone*)((char*)(zone + 1))));
				ft_putstr(" - 0x");
				ft_putaddr((uint64_t)((t_zone*)((char*)(zone + 1)
						+ zone->size)));
				ft_putstr(" : ");
				ft_putnbr(zone->size);
				ft_putstr(" octets\n");
				total += zone->size;
			}
			zone = zone->next;
		}
	}
	if (g_e.large)
	{
		ft_putstr("LARGE : 0x");
		ft_putaddr((uint64_t)g_e.large);
		ft_putchar('\n');
		zone = g_e.large;
		while (zone)
		{
			if (zone->free == 0)
			{
				ft_putstr("0x");
				ft_putaddr((uint64_t)((t_zone*)((char*)(zone + 1))));
				ft_putstr(" - 0x");
				ft_putaddr((uint64_t)((t_zone*)((char*)(zone + 1)
						+ zone->size)));
				ft_putstr(" : ");
				ft_putnbr(zone->size);
				ft_putstr(" octets\n");
				total += zone->size;
			}
			zone = zone->next;
		}
	}
	ft_putstr("Total : ");
	ft_putbigunbr(total);
	ft_putstr(" octets\n");
}
