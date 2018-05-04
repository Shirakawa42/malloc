#include "malloc.h"

static void	zone_viewer(t_zone *zone)
{
	int		i;
	t_zone	*tmp;

	tmp = zone;
	ft_putstr("\n===========================================================================\n");
	while (zone)
	{
		ft_putstr("size: ");
		ft_putnbr(zone->size);
		ft_putstr(" | free: ");
		ft_putnbr(zone->free);
		ft_putstr("\n");
		zone = zone->next;
		if (zone && zone->prevbig)
			ft_putstr("--------------------------------------------------------------\n");
	}
	ft_putstr("===========================================================================\n");
	if (tmp && tmp->type != 'L')
	{
		ft_putstr("total bigs: ");
		i = 0;
		while (tmp)
		{
			i++;
			tmp = tmp->nextbig;
		}
		ft_putnbr(i);
		ft_putchar('\n');
	}
}

int		main(void)
{
	char	*a;

	for (int i = 0; i < 6; i++)
		malloc(5000);
	a = malloc(7000);
	for (int i = 0; i < 6; i++)
		malloc(5000);
	zone_viewer(g_e.tiny);
	return (0);
}
