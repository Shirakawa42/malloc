#include <stdlib.h>
#include <strings.h>

int		main()
{
	char **a;
	int		i = 0;

	while (1)
	{
		a = (char**)malloc(sizeof(char*) * i);
		for (int j = 0; j < i; j++)
		{
			a[j] = (char*)malloc(sizeof(char) * j);
			bzero(a[j], j*10);
		}
		for (int j = 0; j < i; j++)
		{
			free(a[j]);
		}
		free(a);
		i++;
		if (i == 11233300)
			i = 0;
	}
	return 0;
}
