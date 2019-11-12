/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:33:15 by lvasseur          #+#    #+#             */
/*   Updated: 2019/11/12 14:50:09 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void		ft_bzero(void *s, size_t n)
{
	char	*mem;
	size_t	i;

	mem = (char*)s;
	i = 0;
	while (i < n)
		mem[i++] = '\0';
}

void			*calloc(size_t nmemb, size_t size)
{
	void	*ret;

	if (!(ret = malloc(nmemb * size)))
		return (NULL);
	pthread_mutex_lock(&g_mutex);
	ft_bzero(ret, nmemb * size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
