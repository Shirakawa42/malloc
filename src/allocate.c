/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 13:39:09 by lvasseur          #+#    #+#             */
/*   Updated: 2019/09/16 13:39:36 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

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
