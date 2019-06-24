#include "malloc.h"

static void	print_zone(t_node *node, uint64_t *total)
{
	ft_putaddr((uint64_t)node);
	ft_putchar('\n');
	while (node)
	{
		if (node->free == 0)
		{
			ft_putstr("0x");
			ft_putaddr((uint64_t)(node + 1));
			ft_putstr(" - 0x");
			ft_putaddr((uint64_t)((char*)(node + 1) + node->size));
			ft_putstr(" : ");
			ft_putnbr(node->size + sizeof(t_node));
			ft_putstr(" octets\n");
			*total += node->size + sizeof(t_node);
		}
		node = node->next;
	}
}

void		show_alloc_mem(void)
{
	uint64_t	total;

	total = 0;
	if (g_stock.tiny)
	{
		ft_putstr("TINY : 0x");
		print_zone(g_stock.tiny, &total);
	}
	if (g_stock.medium)
	{
		ft_putstr("MEDIUM : 0x");
		print_zone(g_stock.medium, &total);
	}
	if (g_stock.large)
	{
		ft_putstr("LARGE : 0x");
		print_zone(g_stock.large, &total);
	}
	ft_putstr("Total : ");
	ft_putbigunbr(total);
	ft_putstr(" octets\n");
}