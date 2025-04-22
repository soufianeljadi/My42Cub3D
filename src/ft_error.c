/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:46:39 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 14:51:21 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_data(t_data *data)
{
	int	i;

	if (data->north_texture)
		free(data->north_texture);
	if (data->south_texture)
		free(data->south_texture);
	if (data->west_texture)
		free(data->west_texture);
	if (data->east_texture)
		free(data->east_texture);
	if (data->map)
		ft_free_tab(data->map);
}

void	error_exit(t_data *data, const char *msg)
{
	if (data)
		free_data(data);
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(1);
}
