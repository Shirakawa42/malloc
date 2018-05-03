#include "malloc.h"

static t_e	g_e;

void	*allocate(size_t size)
{
	return (mmap(NULL, size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0));
}

size_t	zonetopage(void)
{
	size_t		ret;
	size_t		zonesize;

	ret = 1;
	zonesize = sizeof(t_zone) * 100;
	while (zonesize > PAGE_SIZE)
	{
		zonesize -= PAGE_SIZE;
		ret++;
	}
	return (ret);
}

size_t	largetopage(size_t size)
{
	size_t		ret;

	ret = (size_t)((size + sizeof(t_zone)) / PAGE_SIZE);
	if ((size + sizeof(t_zone)) % PAGE_SIZE != 0)
		ret++;
	return (ret * PAGE_SIZE);
}

void	*init(size_t size, t_zone *zone, size_t max)
{
	zone->size = size;
	zone->free = 0;
	zone->lastbig = zone;
	zone->prev = NULL;
	zone->nextbig = NULL;
	zone->prevbig = NULL;
	zone->next = (t_zone*)((char*)(zone + 1) + size);
	zone->next->nextbig = NULL;
	zone->next->prevbig = NULL;
	zone->next->size = (max * 100) + (zonetopage() * PAGE_SIZE) - size - sizeof(t_zone);
	zone->next->free = 1;
	zone->next->lastbig = NULL;
	zone->next->prev = zone;
	zone->next->next = NULL;
	if (max == TINY)
	{
		zone->type = 'T';
		g_e.lasttiny = zone->next;
	}
	else
	{
		zone->type = 'M';
		g_e.lastmedium = zone->next;
	}
	zone->next->type = zone->type;
	return ((t_zone*)((char*)(zone + 1)));
}

void	*init_large(size_t size, t_zone *zone)
{
	zone->size = size;
	zone->free = 0;
	zone->lastbig = zone;
	zone->next = NULL;
	zone->prev = NULL;
	zone->prevbig = NULL;
	zone->nextbig = NULL;
	zone->type = 'L';
	return ((t_zone*)((char*)(zone + 1)));
}

void	*concatenate_new_zone(t_zone *zone, size_t size)
{
	ft_putstr("concatenating...\n");
	if (size <= TINY)
	{
		if ((zone->next = (t_zone*)allocate((TINY * 100) + (zonetopage() * PAGE_SIZE))) == NULL)
			return (NULL);
		zone->next->next = (t_zone*)((char*)(zone->next + 1) + size);
		zone->next->next->size = (TINY * 100) + (zonetopage() * PAGE_SIZE) - size - sizeof(t_zone);
		zone->next->next->type = 'T';
		zone->next->prevbig = g_e.tiny->lastbig;
		zone->next->nextbig = NULL;
		zone->next->type = 'T';
		g_e.tiny->lastbig->nextbig = zone->next;
		g_e.tiny->lastbig = zone->next;
		g_e.lasttiny = zone->next->next;
	}
	else
	{
		if ((zone->next = (t_zone*)allocate((MEDIUM * 100) + (zonetopage() * PAGE_SIZE))) == (void*)-1)
			return (NULL);
		zone->next->next = (t_zone*)((char*)(zone->next + 1) + size);
		zone->next->next->size = (MEDIUM * 100) + (zonetopage() * PAGE_SIZE) - size - sizeof(t_zone);
		zone->next->next->type = 'M';
		zone->next->prevbig = g_e.medium->lastbig;
		zone->next->nextbig = NULL;
		zone->next->type = 'M';
		g_e.medium->lastbig->nextbig = zone->next;
		g_e.medium->lastbig = zone->next;
		g_e.lastmedium = zone->next->next;
	}
	zone->next->prev = zone;
	zone->next->next->prev = zone->next;
	zone->next->lastbig = NULL;
	zone->next->size = size;
	zone->next->free = 0;
	zone->next->next->free = 1;
	zone->next->next->next = NULL;
	zone->next->next->lastbig = NULL;
	return ((t_zone*)((char*)(zone->next + 1)));
}

void	*concatenate_new_large(t_zone *zone, size_t size)
{
	zone = zone->lastbig;
	if ((zone->next = (t_zone*)allocate(largetopage(size))) == (void*)-1)
		return (NULL);
	zone->next->size = size;
	zone->next->free = 0;
	zone->next->prev = zone;
	zone->next->type = 'L';
	zone->next->next = NULL;
	zone->next->nextbig = NULL;
	zone->next->prevbig = NULL;
	g_e.large->lastbig = zone->next;
	g_e.lasttiny = zone->next->next;
	return ((t_zone*)((char*)(zone->next + 1)));
}

void	*search_for_free_space(size_t size, t_zone *zone, size_t max, t_zone *last)
{
	int		i;

	if (last->size >= size)
		zone = last;
	while (zone->free != 1 || zone->size < size)
	{
		if (!zone->next)
			return (concatenate_new_zone(zone, size));
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

void	*malloc2(size_t size)
{
	if (size < 0)
		return (NULL);
	else if (size <= TINY)
		if (g_e.tiny == NULL)
		{
			if ((g_e.tiny = (t_zone*)allocate((TINY * 100) + (zonetopage() * PAGE_SIZE))) == (void*)-1)
				return (NULL);
			return (init(size, g_e.tiny, TINY));
		}
		else
			return (search_for_free_space(size, g_e.tiny, TINY, g_e.lasttiny));
	else if (size <= MEDIUM)
		if (g_e.medium == NULL)
		{
			if ((g_e.medium = (t_zone*)allocate((MEDIUM * 100) + (zonetopage() * PAGE_SIZE))) == (void*)-1)
				return (NULL);
			return (init(size, g_e.medium, MEDIUM));
		}
		else
			return (search_for_free_space(size, g_e.medium, MEDIUM, g_e.lastmedium));
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

void	fusion(t_zone *first, t_zone *second)
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

void	look_for_fusion(t_zone *zone)
{
	if (zone->prev && !zone->prevbig)
		if (zone->prev->free == 1)
			fusion(zone->prev, zone);
	if (zone->next && !zone->next->prevbig)
		if (zone->next->free == 1)
			fusion(zone, zone->next);
}

void	look_for_destruction(t_zone *zone)
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

void	look_for_large_destruction(t_zone *zone)
{
	if (zone->prev)
		zone->prev->next = zone->next;
	if (zone->next && zone->prev)
		zone->next->prev = zone->prev;
	if (!zone->prev)
		g_e.large = zone->next;
	munmap(zone, largetopage(zone->size));
}

void	free2(void *ptr)
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

void	*split_realloc(t_zone *zone, size_t size, void *ptr)
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

void	*bigger_realloc(t_zone *zone, size_t size, void *ptr)
{
	void	*new;

	if ((new = malloc2(size)) == NULL)
		return (NULL);
	ft_memcpy(new, ptr, zone->size);
	free2(ptr);
	return (new);
}

void	*realloc2(void *ptr, size_t size)
{
	t_zone	*zone;

	if (!ptr)
		return (malloc2(size));
	if (size < 1)
		return (NULL);
	zone = (t_zone*)ptr - 1;
	if ((size + sizeof(t_zone) + 1) < zone->size)
		return (split_realloc(zone, size, ptr));
	else if (size > zone->size)
		return (bigger_realloc(zone, size, ptr));
	return (ptr);
}

void	zone_viewer(t_zone *zone)
{
	int		i;
	t_zone	*tmp;

	tmp = zone;
	ft_putstr("\n===========================================================================\n");
	while (zone)
	{
		ft_putstr("size: ");
		ft_putnbr(zone->size);
		ft_putstr(" | free: ");
		ft_putnbr(zone->free);
		ft_putstr("\n");
		zone = zone->next;
		if (zone && zone->prevbig)
			ft_putstr("--------------------------------------------------------------\n");
	}
	ft_putstr("===========================================================================\n");
	if (tmp && tmp->type != 'L')
	{
		ft_putstr("total bigs: ");
		i = 0;
		while (tmp)
		{
			i++;
			tmp = tmp->nextbig;
		}
		ft_putnbr(i);
		ft_putchar('\n');
	}
}

int		main(void)
{
	char	*a;

	for (int i = 0; i < 6; i++)
		malloc2(5000);
	a = malloc2(7000);
	for (int i = 0; i < 6; i++)
		malloc2(5000);
	realloc2(a, 10);
	zone_viewer(g_e.tiny);
	return (0);
}
