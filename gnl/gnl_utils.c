/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:42:32 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/03/01 15:43:02 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	ft_search(char *s, int c)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == (char) c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_join(char *dest, char *start, char *buff)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (start && start[i])
	{
		dest[i] = start[i];
		i++;
	}
	j = 0;
	while (buff && buff[j])
	{
		dest[i + j] = buff[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strjoin_gnl(char *start, char *buff)
{
	char	*ptr;

	if (!start && !buff)
		return (NULL);
	else if (!buff)
		return (ft_strdup(start));
	else if (!start)
		return (ft_strdup(buff));
	ptr = (char *)malloc(1 + ft_strlen(start) + ft_strlen(buff));
	if (!ptr)
		return (NULL);
	ptr = ft_join(ptr, start, buff);
	free(start);
	return (ptr);
}
