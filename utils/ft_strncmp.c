#include "../includes/cub3d.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*p1;
	unsigned char	*p2;

	i = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (p1[i] == p2[i] && (p1[i] != '\0' && p2[i] != '\0') && i < n - 1)
		i++;
	return (p1[i] - p2[i]);
}