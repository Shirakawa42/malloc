#include "malloc.h"

static void	*search_zone(void *ptr, t_node *node)
{
	while (node)
	{
		if (node == ptr)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void		*search_zonebig(void *ptr)
{
	void	*ret;

	if ((ret = search_zone(ptr, g_stock.tiny)) != NULL)
		return (ret);
	else if ((ret = search_zone(ptr, g_stock.medium)) != NULL)
		return (ret);
	else if ((ret = search_zone(ptr, g_stock.large)) != NULL)
		return (ret);
	return (NULL);
}