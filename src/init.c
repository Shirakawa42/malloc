#include "malloc.h"

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
	zone->next->size = (max * 100) + (zonetopage() * PAGE_SIZE) -
			size - sizeof(t_zone);
	zone->next->free = 1;
	zone->next->lastbig = NULL;
	zone->next->prev = zone;
	zone->next->next = NULL;
	zone->type = (max == TINY) ? 'T' : 'M';
	if (max == TINY)
		g_e.lasttiny = zone->next;
	else
		g_e.lastmedium = zone->next;
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
