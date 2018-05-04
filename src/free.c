#include "malloc.h"

static void	fusion(t_zone *first, t_zone *second)
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

static void	look_for_fusion(t_zone *zone)
{
	if (zone->prev && !zone->prevbig)
		if (zone->prev->free == 1)
			fusion(zone->prev, zone);
	if (zone->next && !zone->next->prevbig)
		if (zone->next->free == 1)
			fusion(zone, zone->next);
}

static void	look_for_large_destruction(t_zone *zone)
{
	if (zone->prev)
		zone->prev->next = zone->next;
	if (zone->next && zone->prev)
		zone->next->prev = zone->prev;
	if (!zone->prev)
		g_e.large = zone->next;
	munmap(zone, largetopage(zone->size));
}

static void	look_for_destruction(t_zone *zone)
{
	t_zone	*tmp;

	tmp = zone;
	while (tmp)
	{
		if (tmp->free == 0)
			return ;
		tmp = tmp->next;
		if (tmp && tmp->prevbig)
			break ;
	}
	tmp = zone;
	while (tmp && !tmp->lastbig && !tmp->nextbig)
	{
		if (tmp->free == 0)
			return ;
		if (tmp->prev && !tmp->prevbig)
			tmp = tmp->prev;
		else
			break ;
	}
	if (tmp->lastbig)
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
		return ;
	}
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

void		free(void *ptr)
{
	t_zone	*zone;

	if (ptr == NULL)
		return ;
	zone = (t_zone*)ptr - 1;
	zone->free = 1;
	if (zone->type == 'T' || zone->type == 'M')
	{
		look_for_fusion(zone);
		look_for_destruction(zone);
	}
	else
		look_for_large_destruction(zone);
}
