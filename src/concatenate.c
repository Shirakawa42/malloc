#include "malloc.h"

void	*concatenate_tiny(t_zone *zone, size_t size)
{
	if ((zone->next = (t_zone*)allocate((TINY * 100) + (zonetopage() * PAGE_SIZE))) == (void*)-1)
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

void	*concatenate_medium(t_zone *zone, size_t size)
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
