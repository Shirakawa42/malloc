/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_in_zone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/17 15:24:16 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/17 15:24:52 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	*search_zone(void *ptr, t_zone *zone)
{
	while (zone)
	{
		if (zone == ptr)
			return (zone);
		zone = zone->next;
	}
	return (NULL);
}

void		*search_zonebig(void *ptr)
{
	void	*ret;

	if ((ret = search_zone(ptr, g_e.tiny)) != NULL)
		return (ret);
	else if ((ret = search_zone(ptr, g_e.medium)) != NULL)
		return (ret);
	else if ((ret = search_zone(ptr, g_e.large)) != NULL)
		return (ret);
	return (NULL);
}
