#include "../includes/cub3d.h"

size_t	ft_strlen(const char *s)
{
	size_t	n;

	if (!s)
		return (0);
	n = 0;
	while (s[n])
		n++;
	return (n);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	len_s1;
	size_t	len_s2;
	size_t	i;
	size_t	j;

	(1) && (len_s1 = 0, len_s2 = 0, i = -1, j = -1);
	if (s1)
		len_s1 = ft_strlen(s1);
	if (s2)
		len_s2 = ft_strlen(s2);
	result = (char *)malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!result)
		return (NULL);
	while (++i < len_s1)
		result[i] = s1[i];
	while (++j < len_s2)
		result[i + j] = s2[j];
	result[i + j] = '\0';
	return (result);
}
