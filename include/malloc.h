#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include "libft.h"

# define PAGE_SIZE getpagesize()
# define TINY 10 * PAGE_SIZE
# define MEDIUM 200 * PAGE_SIZE

typedef struct	s_zone
{
	int				size;
	int				free;
	struct s_zone	*next;
	struct s_zone	*prev;
	struct s_zone	*lastbig;
	struct s_zone	*nextbig;
	struct s_zone	*prevbig;
	char			type;
}				t_zone;

typedef struct	s_e
{
	t_zone	*tiny;
	t_zone	*medium;
	t_zone	*large;
	t_zone	*lasttiny;
	t_zone	*lastmedium;
}				t_e;

void	*malloc2(size_t size);
