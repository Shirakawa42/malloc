/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:33:03 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/09 14:33:04 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*search_for_free_space(size_t size, t_zone *zone, t_zone *last)
{
	size_t	i;

	if (last->size >= size)
		zone = last;
	while (zone->free != 1 || zone->size < size)
	{
		if (!zone->next)
			return ((zone->type == 'T') ? concatenate_tiny(zone, size) :
					concatenate_medium(zone, size));
		zone = zone->next;
	}
	if (zone->next && zone->size >= size + sizeof(t_zone) + 1)
	{
		zone->next->prev = (t_zone*)((char*)(zone + 1) + size);
		zone->next->prev->prev = zone;
		zone->next->prev->next = zone->next;
		zone->next = zone->next->prev;
		zone->next->size = zone->size - size - sizeof(t_zone);
		zone->next->free = 1;
		zone->next->type = (size <= TINY) ? 'T' : 'M';
	}
	i = zone->size - size;
	zone->free = 0;
	zone->size = size;
	if (!zone->next)
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
	}
	return ((t_zone*)((char*)(zone + 1)));
}

void	*malloc(size_t size)
{
	if (size <= 0)
		return (NULL);
	else if (size <= TINY)
		if (g_e.tiny == NULL)
		{
			if ((g_e.tiny = (t_zone*)allocate((TINY * 100) + (zonetopage() * PAGE_SIZE))) == (void*)-1)
				return (NULL);
			return (init(size, g_e.tiny, TINY));
		}
		else
			return (search_for_free_space(size, g_e.tiny, g_e.lasttiny));
	else if (size <= MEDIUM)
		if (g_e.medium == NULL)
		{
			if ((g_e.medium = (t_zone*)allocate((MEDIUM * 100) + (zonetopage() * PAGE_SIZE))) == (void*)-1)
				return (NULL);
			return (init(size, g_e.medium, MEDIUM));
		}
		else
			return (search_for_free_space(size, g_e.medium, g_e.lastmedium));
	else
		if (g_e.large == NULL)
		{
			if ((g_e.large = (t_zone*)allocate(largetopage(size))) == (void*)-1)
				return (NULL);
			return (init_large(size, g_e.large));
		}
		else
			return (concatenate_new_large(g_e.large, size));
	return (NULL);
}
