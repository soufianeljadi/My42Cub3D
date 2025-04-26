/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:46:31 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 11:46:31 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	validate_vertical_walls(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->map_height)
	{
		validate_horizontal_walls(data, i);
		j = 0;
		while (j < (int)ft_strlen(data->map[i]))
		{
			validate_adjacent_spaces(data, i, j);
			j++;
		}
		i++;
	}
}

size_t	get_max_line_length(t_data *data)
{
	size_t	max_len;
	int		i;
	size_t	len;

	max_len = 0;
	i = 0;
	len = 0;
	while (i < data->map_height)
	{
		len = ft_strlen(data->map[i]);
		if (len > max_len)
			max_len = len;
		i++;
	}
	return (max_len);
}

void	pad_map_line(t_data *data, int i, size_t max_len)
{
	size_t	len;
	char	*padded_line;

	len = ft_strlen(data->map[i]);
	if (len < max_len)
	{
		padded_line = malloc((max_len + 1) * sizeof(char));
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
}

void	pad_map_lines(t_data *data)
{
	size_t	max_len;
	int		i;

	max_len = get_max_line_length(data);
	i = 0;
	while (i < data->map_height)
	{
		pad_map_line(data, i, max_len);
		i++;
	}
}

void	validate_map(t_data *data)
{
	validate_map_characters(data);
	validate_border_line(data, 0, \
		"First line must contain only walls and spaces");
	validate_border_line(data, data->map_height - 1, \
		"Last line must contain only walls and spaces");
	validate_vertical_walls(data);
	pad_map_lines(data);
}
