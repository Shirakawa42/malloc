#include "malloc.h"

static void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
	else
		ft_putchar(n + 48);
}

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void	show_alloc_mem()
{
	t_node	*node;

	node = g_stock.large;
	while (node)
	{
		ft_putstr("==================================================\n");
		ft_putstr("id: ");
		ft_putnbr(node->zone_id);
		ft_putstr("\nsize: ");
		ft_putnbr(node->size);
		ft_putstr("\nfree: ");
		ft_putnbr(node->free);
		ft_putstr("\n==================================================\n");
		node = node->next;
	}
}