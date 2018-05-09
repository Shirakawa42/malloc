/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 13:37:15 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/09 16:03:03 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		fusion(t_zone *first, t_zone *second)
{
	if (!second->next)
	{
		if (second->type == 'T')
			g_e.lasttiny = first;
		else
			g_e.lastmedium = first;
	}
	else
		second->next->prev = first;
	first->size += second->size + sizeof(t_zone);
	first->next = second->next;
}

static void		look_for_fusion(t_zone *zone)
{
	if (zone->prev && !zone->prevbig)
		if (zone->prev->free == 1)
			fusion(zone->prev, zone);
	if (zone->next && !zone->next->prevbig)
		if (zone->next->free == 1)
			fusion(zone, zone->next);
}

static void		look_for_large_destruction(t_zone *zone)
{
	if (zone->prev)
		zone->prev->next = zone->next;
	if (zone->next && zone->prev)
		zone->next->prev = zone->prev;
	if (!zone->prev)
		g_e.large = zone->next;
	munmap(zone, largetopage(zone->size));
}

void			free(void *ptr)
{
	t_zone	*zone;

	if (ptr == NULL)
		return ;
	zone = (t_zone*)ptr - 1;
	if (!zone->free)
		return ;
	zone->free = 1;
	if (zone->type == 'T' || zone->type == 'M')
	{
		look_for_fusion(zone);
		look_for_destruction(zone);
	}
	else
		look_for_large_destruction(zone);
}
