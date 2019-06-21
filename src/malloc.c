#include "malloc.h"

static void		*claim_node(t_node **node, size_t size)
{
	t_node	*n;
	t_node	*tmp;

	n = *node;
	n->free = 0;
	if (n->size > size + sizeof(t_node) && size + sizeof(t_node) <= MEDIUM * getpagesize())
	{
		tmp = n->next;
		n->next = (t_node*)((char*)(n + 1) + size);
		n->next->next = tmp;
		n->next->size = n->size - size - sizeof(t_node);
		n->size = size;
		n->next->zone_id = n->zone_id;
		n->next->free = 1;
	}
	return ((t_node*)((char*)(n + 1)));
}

static t_node	*create_new_zone(int *id, t_node *node, size_t size)
{
	if ((node = allocate(size)) == (void*)-1)
			return NULL;
	node->free = 1;
	node->next = NULL;
	node->zone_id = *id;
	if (size + sizeof(t_node) <= TINY * getpagesize())
		node->size = (TINY * getpagesize() * 100) - sizeof(t_node);
	else if (size + sizeof(t_node) <= MEDIUM * getpagesize())
		node->size = (MEDIUM * getpagesize() * 100) - sizeof(t_node);
	else
		node->size = (size - (size % getpagesize()) + getpagesize()) - sizeof(t_node);
	(*id)++;
	return (node);
}

static void		*malloc_tiny(size_t size)
{
	static int	id = 0;
	t_node		*node;

	if (g_stock.tiny == NULL)
		g_stock.tiny = create_new_zone(&id, g_stock.tiny, size);
	node = g_stock.tiny;
	while (node->next && (node->free == 0 || node->size < size))
		node = node->next;
	if (!node->next && (node->free == 0 || node->size < size))
	{
		node->next = create_new_zone(&id, node->next, size);
		node = node->next;
	}
	if (node == NULL)
		return NULL;
	return (claim_node(&node, size));
}

static void		*malloc_medium(size_t size)
{
	static int	id = 0;
	t_node		*node;

	if (g_stock.medium == NULL)
		g_stock.medium = create_new_zone(&id, g_stock.medium, size);
	node = g_stock.medium;
	while (node->next && (node->free == 0 || node->size < size))
		node = node->next;
	if (!node->next && (node->free == 0 || node->size < size))
	{
		node->next = create_new_zone(&id, node->next, size);
		node = node->next;
	}
	if (node == NULL)
		return NULL;
	return (claim_node(&node, size));
}

static void		*malloc_large(size_t size)
{
	static int	id = 0;
	t_node		*node;

	if (g_stock.large == NULL)
		g_stock.large = create_new_zone(&id, g_stock.large, size);
	node = g_stock.large;
	while (node->next)
		node = node->next;
	if (node->free == 0)
	{
		node->next = create_new_zone(&id, node->next, size);
		node = node->next;
	}
	if (node == NULL)
		return NULL;
	return (claim_node(&node, size));
}

void			*malloc(size_t size)
{
	if (size + sizeof(t_node) <= TINY * getpagesize())
		return (malloc_tiny(size));
	else if (size + sizeof(t_node) <= MEDIUM * getpagesize())
		return (malloc_medium(size));
	else
		return (malloc_large(size));
	return NULL;
}