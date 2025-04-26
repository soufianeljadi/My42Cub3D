/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:51:55 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 12:00:09 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	validate_player(t_data *data, char c, int i, int j)
{
	static int	player_count = 0;

	if (ft_strchr("NSEW", c))
	{
		player_count++;
		data->player_x = j;
		data->player_y = i;
		data->player_dir = c;
	}
	else if (!ft_strchr("01 NSEW", c))
		error_exit(data, "Invalid character in map");
	if (player_count > 1
		|| (data->map[data->map_height - 1] == data->map[i] \
			&& player_count == 0))
		error_exit(data, "Map must contain exactly one player spawn point");
}

void	validate_map_characters(t_data *data)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < data->map_height)
	{
		j = 0;
		while (j < (int)ft_strlen(data->map[i]))
		{
			c = data->map[i][j];
			validate_player(data, c, i, j);
			j++;
		}
		i++;
	}
}

void	validate_border_line(t_data *data, int line_index, char *error_msg)
{
	int	i;

	i = 0;
	while (i < (int)ft_strlen(data->map[line_index]))
	{
		if (data->map[line_index][i] != '1' && data->map[line_index][i] != ' ')
			error_exit(data, error_msg);
		i++;
	}
}

void	validate_horizontal_walls(t_data *data, int i)
{
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(data->map[i]) - 1;
	while (data->map[i][start] == ' ')
		start++;
	if (data->map[i][start] != '1' || data->map[i][end] != '1')
		error_exit(data, "Map must be surrounded by walls");
}

void	validate_adjacent_spaces(t_data *data, int i, int j)
{
	int	end;

	end = ft_strlen(data->map[i]) - 1;
	if (data->map[i][j] == '0' || ft_strchr("NSEW", data->map[i][j]))
	{
		if ((j > 0 && data->map[i][j - 1] == ' ') \
			|| data->map[i][j - 1] == '\0')
			error_exit(data, "Empty space adjacent to space in the same line");
		if ((j < end && data->map[i][j + 1] == ' ') \
			|| data->map[i][j + 1] == '\0')
			error_exit(data, "Empty space adjacent to space in the same line");
		if ((i > 0 && data->map[i - 1][j] == ' ') \
			|| data->map[i - 1][j] == '\0')
			error_exit(data, "Empty space adjacent to space in the line above");
		if ((i < data->map_height - 1 && data->map[i + 1][j] == ' ') \
			|| data->map[i + 1][j] == '\0')
			error_exit(data, "Empty space adjacent to space in the line below");
	}
}
