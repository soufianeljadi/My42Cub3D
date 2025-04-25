/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:03:27 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/03/01 16:03:30 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	ft_atoi(char *str)
{
	int			i;
	int			s;
	int			r;
	int			x;

	i = 0;
	s = 1;
	r = 0;
	if (!str[0])
		return (-1);
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		s = 44 - str[i++];
	while (str[i] >= '0' && str[i] <= '9')
	{
		x = r * 10 + (str[i++] - 48);
		if (r > x && s == 1)
			return (-1);
		else if (r > x && s == -1)
			return (-1);
		r = x;
	}
	return (r * s);
}
