/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:57:45 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/03/01 16:02:13 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	parse_map(t_data *data, char *line)
{
	static int	map_started;
	char		*trimmed_line;
	char		**new_map;
	int			i;

	(1) && (i = 0, new_map = NULL, trimmed_line = NULL);
	if (!map_started)
	{
		data->map = malloc(sizeof(char *) * 100);
		if (!data->map)
			error_exit(data, "Failed to allocate memory for map");
		data->map_height = 0;
		data->map_capacity = 10;
		map_started = 1;
	}
	trimmed_line = ft_strtrim(line, "\n");
	if (!trimmed_line)
		error_exit(data, "Failed to trim map line");
	if (data->map_height >= data->map_capacity)
	{
		data->map_capacity *= 2;
		new_map = malloc(sizeof(char *) * data->map_capacity);
		if (!new_map)
			error_exit(data, "Failed to resize map array");
		while (i < data->map_height)
		{
			new_map[i] = data->map[i];
			i++;
		}
		free(data->map);
		data->map = new_map;
	}
	data->map[data->map_height++] = trimmed_line;
}

void	validate_map(t_data *data)
{
	int	player_count ;
	int i;
	int j;
	char c;
	
	(1) && (player_count = 0, i = 0, j = 0, c = 0);
	while (i < data->map_height)
	{
		j = 0;
		while (j < (int)ft_strlen(data->map[i]))
		{
			c = data->map[i][j];
			if (ft_strchr("NSEW", c))
			{
				player_count++;
				data->player_x = j;
				data->player_y = i;
				data->player_dir = c;
			}
			else if (!ft_strchr("01 NSEW", c))
				error_exit(data, "Invalid character in map");
			j++;
		}
		i++;
	}

	if (player_count != 1)
		error_exit(data, "Map must contain exactly one player spawn point");

	i = 0;
	while (i < (int)ft_strlen(data->map[0]))
	{
		if (data->map[0][i] != '1' && data->map[0][i] != ' ')
			error_exit(data, "First line must contain only walls and spaces");
		i++;
	}
	i = 0;
	while (i < (int)ft_strlen(data->map[data->map_height - 1]))
	{
		if (data->map[data->map_height - 1][i] != '1' && data->map[data->map_height - 1][i] != ' ')
			error_exit(data, "Last line must contain only walls and spaces");
		i++;
	}

	i = 0;
	while (i < data->map_height)
	{
		int start = 0;
		int end = ft_strlen(data->map[i]) - 1;

		while (data->map[i][start] == ' ')
			start++;

		if (data->map[i][start] != '1' || data->map[i][end] != '1')
			error_exit(data, "Map must be surrounded by walls");

		j = start;
		while (j <= end)
		{
			if (data->map[i][j] == '0')
			{
				if ((j > start && data->map[i][j - 1] == ' ') || data->map[i][j - 1] == '\0')
					error_exit(data, "Empty space adjacent to space in the same line");

				if ((j < end && data->map[i][j + 1] == ' ') || data->map[i][j + 1] == '\0')
					error_exit(data, "Empty space adjacent to space in the same line");

				if ((i > 0 && data->map[i - 1][j] == ' ' )|| data->map[i - 1][j] == '\0')
					error_exit(data, "Empty space adjacent to space in the line above");

				if ((i < data->map_height - 1 && data->map[i + 1][j] == ' ' )|| data->map[i + 1][j] == '\0')
					error_exit(data, "Empty space adjacent to space in the line below");
			}
			j++;
		}
		i++;
	}

	size_t max_len = 0;
	i = 0;
	while (i < data->map_height)
	{
		size_t len = ft_strlen(data->map[i]);
		if (len > max_len)
			max_len = len;
		i++;
	}

	i = 0;
	while (i < data->map_height)
	{
		size_t len = ft_strlen(data->map[i]);
		if (len < max_len)
		{
			char *padded_line = malloc((max_len + 1) * sizeof(char));
			if (!padded_line)
				error_exit(data, "Failed to allocate memory for padded line");

			ft_strlcpy(padded_line, data->map[i], len + 1);
			while (len < max_len)
			{
				padded_line[len] = ' ';
				len++;
			}
			padded_line[len] = '\0';

			free(data->map[i]);
			data->map[i] = padded_line;
		}
		i++;
	}
}