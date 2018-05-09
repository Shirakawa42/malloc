/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:33:07 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/09 15:03:11 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	split_realloc(t_zone *zone, size_t size)
{
	zone->next->prev = (t_zone*)((char*)(zone + 1) + size);
	zone->next->prev->prev = zone;
	zone->next->prev->next = zone->next;
	zone->next = (t_zone*)((char*)(zone + 1) + size);
	zone->next->size = zone->size - size - sizeof(t_zone);
	zone->next->free = 1;
	zone->next->nextbig = NULL;
	zone->next->prevbig = NULL;
	zone->next->lastbig = NULL;
	zone->next->type = zone->type;
	zone->size = size;
}

static void	*bigger_realloc(t_zone *zone, size_t size, void *ptr)
{
	void	*new;

	if ((new = malloc(size)) == NULL)
		return (NULL);
	ft_memcpy(new, ptr, zone->size);
	free(ptr);
	return (new);
}

void		*realloc(void *ptr, size_t size)
{
	t_zone	*zone;

	if (!ptr)
		return (malloc(size));
	if (size < 1)
		return (NULL);
	zone = (t_zone*)ptr - 1;
	if ((size + sizeof(t_zone) + 1) < zone->size)
		split_realloc(zone, size);
	else if (size > zone->size)
		return (bigger_realloc(zone, size, ptr));
	return (ptr);
}
