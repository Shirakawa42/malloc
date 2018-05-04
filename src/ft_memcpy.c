/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/07 13:18:30 by lvasseur          #+#    #+#             */
/*   Updated: 2016/11/09 16:55:37 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*s1;
	char	*s2;

	s1 = (char*)src;
	s2 = (char*)dest;
	if (n == 0 || src == dest)
		return (dest);
	while (--n)
		*s2++ = *s1++;
	*s2 = *s1;
	return (dest);
}
