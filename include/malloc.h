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

typedef struct	s_node
{
	short int		free;
	size_t			size;
	int				zone_id;
	struct s_node	*next;
}				t_node;

typedef struct	s_stock
{
	t_node			*tiny;
	t_node			*medium;
	t_node			*large;
}				t_stock;

t_stock	g_stock;

void	*allocate(size_t size);
void	*malloc(size_t size);
void	show_alloc_mem();

#endif
