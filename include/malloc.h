#include <sys/mman.h>
#include <unistd.h>

# define PAGE_SIZE (size_t)getpagesize()
# define TINY (size_t)(10 * PAGE_SIZE)
# define MEDIUM (size_t)(200 * PAGE_SIZE)

typedef struct	s_zone
{
	size_t			size;
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
	t_zone			*tiny;
	t_zone			*medium;
	t_zone			*large;
	t_zone			*lasttiny;
	t_zone			*lastmedium;
}				t_e;

t_e	g_e;

void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	free(void *ptr);

void	*search_for_free_space(size_t size, t_zone *zone, t_zone *last);

size_t	largetopage(size_t size);
size_t	zonetopage(void);
void	*allocate(size_t size);

void	*init_large(size_t size, t_zone *zone);
void	*init(size_t size, t_zone *zone, size_t max);

void	*concatenate_tiny(t_zone *zone, size_t size);
void	*concatenate_medium(t_zone *zone, size_t size);
void	*concatenate_new_large(t_zone *zone, size_t size);

void	*ft_memcpy(void *dest, const void *src, size_t n);
void	ft_putchar(char c);
void	ft_putnbr(int n);
void	ft_putstr(const char *s);
size_t	ft_strlen(const char *str);
