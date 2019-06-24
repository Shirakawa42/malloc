#include "malloc.h"

static void	destroy_zone(t_node *start, t_node *prev, t_node *next, char type)
{
	if (prev)
		prev->next = next;
	else if (type == 'T')
		g_stock.tiny = next;
	else if (type == 'M')
		g_stock.medium = next;
	else
		g_stock.large = next;
	if (type == 'T')
		munmap(start, TINY * getpagesize() * 100);
	else if (type == 'M')
		munmap(start, MEDIUM * getpagesize() * 100);
	else
		munmap(start, ((start->size + sizeof(t_node)) - ((start->size + sizeof(t_node))
				% getpagesize()) + getpagesize()));
}

static int	look_for_destruction(size_t id, char type, t_node *start)
{
	t_node	*prev;
	t_node	*tmp;

	prev = NULL;
	while (start && start->zone_id != id)
	{
		if (!prev && start->next && start->next->zone_id == id)
			prev = start;
		start = start->next;
	}
	tmp = start;
	while (tmp->next && tmp->zone_id == tmp->next->zone_id)
	{
		if (tmp->free == 0)
			return (0);
		tmp = tmp->next;
	}
	if (tmp->free == 0)
		return (0);
	destroy_zone(start, prev, tmp->next, type);
	return (1);
}

static void	look_for_fusion(size_t id)
{
	(void)id;
}

void	free(void *ptr)
{
	t_node	*node;
	int		destroyed;

	if (ptr == NULL)
		return ;
	node = (t_node*)ptr - 1;
	node->free = 1;
	if (node->size + sizeof(t_node) <= TINY * getpagesize())
		destroyed = look_for_destruction(node->zone_id, 'T', g_stock.tiny);
	else if (node->size + sizeof(t_node) <= MEDIUM * getpagesize())
		destroyed = look_for_destruction(node->zone_id, 'M', g_stock.medium);
	else
		destroyed = look_for_destruction(node->zone_id, 'L', g_stock.large);
	if (destroyed == 0)
		look_for_fusion(node->zone_id);
}