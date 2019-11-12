/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 13:42:40 by lvasseur          #+#    #+#             */
/*   Updated: 2019/11/12 15:00:20 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_node	*create_new_zone(int *id, t_node *node, size_t size)
{
	if ((node = allocate(size)) == (void*)-1)
		return (NULL);
	node->free = 1;
	node->next = NULL;
	node->zone_id = *id;
	if (size + sizeof(t_node) <= TINY * getpagesize())
		node->size = (TINY * getpagesize() * 100) - sizeof(t_node);
	else if (size + sizeof(t_node) <= MEDIUM * getpagesize())
		node->size = (MEDIUM * getpagesize() * 100) - sizeof(t_node);
	else
		node->size = (size - (size % getpagesize()) + getpagesize())
				- sizeof(t_node);
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
	if (node == NULL)
		return (NULL);
	while (node->next && (node->free == 0 || node->size < size))
		node = node->next;
	if (!node->next && (node->free == 0 || node->size < size))
	{
		node->next = create_new_zone(&id, node->next, size);
		node = node->next;
	}
	return (claim_node(&node, size));
}

static void		*malloc_medium(size_t size)
{
	static int	id = 0;
	t_node		*node;

	if (g_stock.medium == NULL)
		g_stock.medium = create_new_zone(&id, g_stock.medium, size);
	node = g_stock.medium;
	if (node == NULL)
		return (NULL);
	while (node->next && (node->free == 0 || node->size < size))
		node = node->next;
	if (!node->next && (node->free == 0 || node->size < size))
	{
		node->next = create_new_zone(&id, node->next, size);
		node = node->next;
	}
	return (claim_node(&node, size));
}

static void		*malloc_large(size_t size)
{
	static int	id = 0;
	t_node		*node;

	if (g_stock.large == NULL)
		g_stock.large = create_new_zone(&id, g_stock.large, size);
	node = g_stock.large;
	if (node == NULL)
		return (NULL);
	while (node->next)
		node = node->next;
	if (node->free == 0)
	{
		node->next = create_new_zone(&id, node->next, size);
		node = node->next;
	}
	return (claim_node(&node, size));
}

void			*malloc(size_t size)
{
	void	*ret;

	ret = NULL;
	if (size >= ULONG_MAX - 16 - sizeof(t_node))
		return (NULL);
	if (size % 16 != 0)
		size += (16 - (size % 16));
	pthread_mutex_lock(&g_mutex);
	if (size + sizeof(t_node) <= TINY * getpagesize())
		ret = malloc_tiny(size);
	else if (size + sizeof(t_node) <= MEDIUM * getpagesize())
		ret = malloc_medium(size);
	else
		ret = malloc_large(size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
