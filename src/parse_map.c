/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:57:45 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 12:00:31 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	ensure_map_capacity(t_data *data)
{
	char	**new_map;
	int		i;

	if (data->map_height >= data->map_capacity)
	{
		data->map_capacity *= 2;
		new_map = malloc(sizeof(char *) * data->map_capacity);
		if (!new_map)
			error_exit(data, "Failed to resize map array");
		i = 0;
		while (i < data->map_height)
		{
			new_map[i] = data->map[i];
			i++;
		}
		free(data->map);
		data->map = new_map;
	}
}

void	parse_map(t_data *data, char *line)
{
	char	*trimmed_line;

	if (data->map == NULL)
	{
		data->map = malloc(sizeof(char *) * 10);
		if (!data->map)
			error_exit(data, "Failed to allocate memory for map");
		data->map_height = 0;
		data->map_capacity = 10;
	}
	trimmed_line = ft_strtrim(line, "\n");
	if (!trimmed_line)
		error_exit(data, "Failed to trim map line");
	ensure_map_capacity(data);
	data->map[data->map_height++] = trimmed_line;
}
