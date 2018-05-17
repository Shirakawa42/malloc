/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_for_free_space.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 15:37:45 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/17 15:44:58 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_zone	*no_zone_next(t_zone *zone, size_t i, size_t size)
{
	if (i >= (sizeof(t_zone) + 1))
	{
		i -= sizeof(t_zone) + 1;
		zone->next = (t_zone*)((char*)(zone + 1) + size);
		zone->next->size = i;
		zone->next->free = 1;
		zone->next->prev = zone;
		zone->next->lastbig = NULL;
		zone->next->nextbig = NULL;
		zone->next->prevbig = NULL;
		zone->next->type = (size <= TINY) ? 'T' : 'M';
		zone->next->next = NULL;
		if (size <= TINY)
			g_e.lasttiny = zone->next;
		else
			g_e.lastmedium = zone->next;
	}
	return (zone);
}

static	t_zone	*free_space_found(t_zone *zone, size_t size)
{
	zone->next->prev = (t_zone*)((char*)(zone + 1) + size);
	zone->next->prev->prev = zone;
	zone->next->prev->next = zone->next;
	zone->next = zone->next->prev;
	zone->next->size = zone->size - size - sizeof(t_zone);
	zone->next->free = 1;
	zone->next->type = (size <= TINY) ? 'T' : 'M';
	return (zone);
}

void			*search_for_free_space(size_t size, t_zone *zone, t_zone *last)
{
	size_t	i;

	if (last->size >= size)
		zone = last;
	while (zone->free != 1 || zone->size < size)
	{
		if (!zone->next)
		{
			return ((zone->type == 'T') ? (concatenate_tiny(zone, size)) :
					(concatenate_medium(zone, size)));
		}
		zone = zone->next;
	}
	if (zone->next && zone->size >= size + sizeof(t_zone) + 1)
		zone = free_space_found(zone, size);
	i = zone->size - size;
	zone->free = 0;
	zone->size = size;
	if (!zone->next)
		zone = no_zone_next(zone, i, size);
	return ((t_zone*)((char*)(zone + 1)));
}
