#include "malloc.h"

static void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*a;
	const char	*b;

	i = 0;
	a = (char*)dest;
	b = (const char*)src;
	while (i < n)
	{
		a[i] = b[i];
		i++;
	}
	return (a);
}

void	*realloc(void *ptr, size_t size)
{
	t_node	*node;
	t_node	*tmp;
	void	*ret;
	size_t	basesize;

	if (ptr == NULL)
		return (NULL);
	node = (t_node*)ptr - 1;
	basesize = node->size;
	if (node->next && node->next->free == 1 && node->zone_id == node->next->zone_id)
	{
		node->size += node->next->size + sizeof(t_node);
		node->next = node->next->next;
	}
	if (size < node->size && node->size - size > sizeof(t_node))
	{
		tmp = node->next;
		node->next = (t_node*)((char*)(node + 1) + size);
		node->next->next = tmp;
		node->next->zone_id = node->zone_id;
		node->next->size = node->size - size - sizeof(t_node);
		node->next->free = 1;
		node->size = size;
		return (node + 1);
	}
	else if (size > node->size)
	{
		ret = malloc(size);
		ret = ft_memcpy(ret, ptr, basesize);
		free(ptr);
		return (ret);
	}
	return (NULL);
}
