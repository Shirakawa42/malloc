/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:32:55 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/09 14:42:41 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	print_zone(t_zone *zone, uint64_t *total)
{
	ft_putaddr((uint64_t)zone);
	ft_putchar('\n');
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
			ft_putnbr(zone->size + sizeof(t_zone));
			ft_putstr(" octets\n");
			*total += zone->size + sizeof(t_zone);
		}
		zone = zone->next;
	}
}

void		show_alloc_mem(void)
{
	uint64_t	total;

	total = 0;
	if (g_e.tiny)
	{
		ft_putstr("TINY : 0x");
		print_zone(g_e.tiny, &total);
	}
	if (g_e.medium)
	{
		ft_putstr("MEDIUM : 0x");
		print_zone(g_e.medium, &total);
	}
	if (g_e.large)
	{
		ft_putstr("LARGE : 0x");
		print_zone(g_e.large, &total);
	}
	ft_putstr("Total : ");
	ft_putbigunbr(total);
	ft_putstr(" octets\n");
}
