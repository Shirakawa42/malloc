/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 13:39:09 by lvasseur          #+#    #+#             */
/*   Updated: 2019/11/12 18:02:26 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	*claim_node(t_node **node, size_t size)
{
	t_node	*n;
	t_node	*tmp;

	n = *node;
	n->free = 0;
	n->size = size;
	if (n->size > size + sizeof(t_node) && size +
			sizeof(t_node) <= MEDIUM * getpagesize())
	{
		tmp = n->next;
		n->next = (t_node*)((char*)(n + 1) + size);
		n->next->next = tmp;
		n->next->size = n->size - size - sizeof(t_node);
		n->next->zone_id = n->zone_id;
		n->next->free = 1;
	}
	return (n + 1);
}

void	*allocate(size_t size)
{
	size += sizeof(t_node);
	if (size <= TINY * getpagesize())
		return (mmap(NULL, TINY * getpagesize() * 100, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, 0, 0));
	else if (size <= MEDIUM * getpagesize())
		return (mmap(NULL, MEDIUM * getpagesize() * 100, PROT_READ | PROT_WRITE,
				MAP_PRIVATE | MAP_ANONYMOUS, 0, 0));
	else
		return (mmap(NULL, (size - (size % getpagesize()) + getpagesize()),
				PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0));
}
