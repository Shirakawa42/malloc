/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   look_for_destruction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:28:36 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/09 14:31:03 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_zone	*destruction_verif(t_zone *zone, t_zone *tmp)
{
	while (tmp)
	{
		if (tmp->free == 0)
			return (NULL);
		tmp = tmp->next;
		if (tmp && tmp->prevbig)
			break ;
	}
	tmp = zone;
	while (tmp && !tmp->lastbig && !tmp->nextbig)
	{
		if (tmp->free == 0)
			return (NULL);
		if (tmp->prev && !tmp->prevbig)
			tmp = tmp->prev;
		else
			break ;
	}
	return (tmp);
}

static void		if_first_big_medium(t_zone *tmp)
{
	if (tmp->nextbig)
	{
		g_e.medium = tmp->nextbig;
		tmp->nextbig->prevbig = NULL;
		tmp->nextbig->prev = NULL;
		tmp->nextbig->lastbig = tmp->lastbig;
		munmap(tmp, (MEDIUM * 100) + (zonetopage() * PAGE_SIZE));
	}
	else
	{
		munmap(tmp, (MEDIUM * 100) + (zonetopage() * PAGE_SIZE));
		g_e.medium = NULL;
	}
}

static void		if_first_big(t_zone *tmp)
{
	if (tmp->type == 'T')
	{
		if (tmp->nextbig)
		{
			g_e.tiny = tmp->nextbig;
			tmp->nextbig->prevbig = NULL;
			tmp->nextbig->prev = NULL;
			tmp->nextbig->lastbig = tmp->lastbig;
			munmap(tmp, (TINY * 100) + (zonetopage() * PAGE_SIZE));
		}
		else
		{
			munmap(tmp, (TINY * 100) + (zonetopage() * PAGE_SIZE));
			g_e.tiny = NULL;
		}
	}
	else
		if_first_big_medium(tmp);
}

static void		destroy_zone_not_only(t_zone *tmp)
{
	if (tmp->type == 'T')
	{
		if (!tmp->nextbig)
		{
			g_e.tiny->lastbig = tmp->prevbig;
			g_e.lasttiny = tmp->prevbig;
		}
		munmap(tmp, (TINY * 100) + (zonetopage() * PAGE_SIZE));
	}
	else
	{
		if (!tmp->nextbig)
		{
			g_e.medium->lastbig = tmp->prevbig;
			g_e.lastmedium = tmp->prevbig;
		}
		munmap(tmp, (MEDIUM * 100) + (zonetopage() * PAGE_SIZE));
	}
}

void			look_for_destruction(t_zone *zone)
{
	t_zone	*tmp;

	tmp = zone;
	if ((tmp = destruction_verif(zone, tmp)) == NULL)
		return ;
	if (tmp->lastbig)
		return (if_first_big(tmp));
	if (tmp->nextbig)
	{
		tmp->nextbig->prevbig = tmp->prevbig;
		tmp->prevbig->nextbig = tmp->nextbig;
		tmp->prev->next = tmp->nextbig;
		tmp->nextbig->prev = tmp->prev;
	}
	else
	{
		tmp->prevbig->nextbig = NULL;
		tmp->prev->next = NULL;
	}
	destroy_zone_not_only(tmp);
}
