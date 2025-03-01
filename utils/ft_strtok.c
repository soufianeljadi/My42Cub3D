/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:08:33 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/03/01 16:12:30 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*save_ptr;
	char		*token_start;

	(1) && (token_start = NULL);
	if (str)
		save_ptr = str;
	if (!save_ptr || *save_ptr == '\0')
		return (NULL);
	while (*save_ptr && ft_strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr == '\0')
	{
		save_ptr = NULL;
		return (NULL);
	}
	token_start = save_ptr;
	while (*save_ptr && !ft_strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr)
	{
		*save_ptr = '\0';
		save_ptr++;
	}
	else
		save_ptr = NULL;
	return (token_start);
}
