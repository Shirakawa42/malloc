/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:33:03 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/18 15:33:50 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		*malloc_tiny(size_t size)
{
	if (g_e.tiny == NULL)
	{
		if ((g_e.tiny = (t_zone*)allocate((TINY * 100) + (zonetopage() *
							PAGE_SIZE))) == (void*)-1)
			return (NULL);
		return (init(size, g_e.tiny, TINY));
	}
	else
		return (search_for_free_space(size, g_e.tiny, g_e.lasttiny));
}

static void		*malloc_medium(size_t size)
{
	if (g_e.medium == NULL)
	{
		if ((g_e.medium = (t_zone*)allocate((MEDIUM * 100) + (zonetopage() *
							PAGE_SIZE))) == (void*)-1)
			return (NULL);
		return (init(size, g_e.medium, MEDIUM));
	}
	else
		return (search_for_free_space(size, g_e.medium, g_e.lastmedium));
}

static void		*malloc_large(size_t size)
{
	if (g_e.large == NULL)
	{
		if ((g_e.large = (t_zone*)allocate(largetopage(size))) == (void*)-1)
			return (NULL);
		return (init_large(size, g_e.large));
	}
	else
		return (concatenate_new_large(g_e.large, size));
}

void			*malloc(size_t size)
{
	if (size <= TINY)
		return (malloc_tiny(size));
	else if (size <= MEDIUM)
		return (malloc_medium(size));
	else
		return (malloc_large(size));
}
