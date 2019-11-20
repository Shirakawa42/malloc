/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 13:52:53 by lvasseur          #+#    #+#             */
/*   Updated: 2019/11/20 16:49:13 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	*big_if(t_node **nodeaddr, size_t size)
{
	t_node	*tmp;
	t_node	*node;

	node = *nodeaddr;
	tmp = node->next;
	node->next = (t_node*)((char*)(node + 1) + size);
	node->next->next = tmp;
	node->next->zone_id = node->zone_id;
	node->next->size = node->size - size - sizeof(t_node);
	node->next->free = 1;
	node->size = size;
	pthread_mutex_unlock(&g_mutex);
	return (node + 1);
}

static void	*big_elseif(size_t size, void *ptr, size_t basesize)
{
	void	*ret;

	if (!(ret = malloc(size)))
	{
		pthread_mutex_unlock(&g_mutex);
		return (NULL);
	}
	ret = ft_memcpy(ret, ptr, basesize);
	free(ptr);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}

static void	*realloc_part_two(void *ptr, size_t size, t_node *node)
{
	size_t	basesize;

	basesize = node->size;
	if (node->next && node->next->free == 1 && node->zone_id ==
			node->next->zone_id)
	{
		node->size += node->next->size + sizeof(t_node);
		node->next = node->next->next;
	}
	if (size < node->size && node->size - size > sizeof(t_node))
		return (big_if(&node, size));
	else if (size > node->size)
		return (big_elseif(size, ptr, basesize));
	else
	{
		pthread_mutex_unlock(&g_mutex);
		return (ptr);
	}
	pthread_mutex_unlock(&g_mutex);
	return (NULL);
}

void		*realloc(void *ptr, size_t size)
{
	t_node	*node;

	if (size >= ULONG_MAX - 16 - sizeof(t_node))
		return (NULL);
	if (size % 16 != 0)
		size += (16 - (size % 16));
	pthread_mutex_lock(&g_mutex);
	if (ptr == NULL)
	{
		pthread_mutex_unlock(&g_mutex);
		return (malloc(size));
	}
	if (size == 0)
	{
		free(ptr);
		pthread_mutex_unlock(&g_mutex);
		return (malloc(16));
	}
	if (!search_zonebig((node = (t_node*)ptr - 1)))
	{
		pthread_mutex_unlock(&g_mutex);
		return (0);
	}
	return (realloc_part_two(ptr, size, node));
}
