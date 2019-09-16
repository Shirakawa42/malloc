/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 13:31:21 by lvasseur          #+#    #+#             */
/*   Updated: 2018/05/18 15:34:01 by lvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H

# define MALLOC_H

# include <sys/mman.h>
# include <unistd.h>
# include <stdint.h>

# define TINY (size_t)16
# define MEDIUM (size_t)128

typedef struct		s_node
{
	short int		free;
	size_t			size;
	size_t			zone_id;
	struct s_node	*next;
}					t_node;

typedef struct		s_stock
{
	t_node			*tiny;
	t_node			*medium;
	t_node			*large;
}					t_stock;

t_stock				g_stock;

void				*allocate(size_t size);
void				*malloc(size_t size);
void				free(void *ptr);
void				ft_putnbr(int n);
void				ft_putstr(char *str);
void				*realloc(void *ptr, size_t size);
void				ft_putaddr(uint64_t addr);
void				ft_putchar(char c);
void				ft_putbigunbr(uint64_t n);
void				show_alloc_mem();
void				*search_zonebig(void *ptr);
void				*claim_node(t_node **node, size_t size);

#endif
