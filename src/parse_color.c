/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:33:01 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 14:52:01 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	check_colors(t_data *data)
{
	if (!data->has_floor_color)
		error_exit(data, "Missing floor color");
	if (!data->has_ceiling_color)
		error_exit(data, "Missing ceiling color");
}

int	check_commas(char *str)
{
	int	commas;
	int	i;

	i = 0;
	commas = 0;
	while (str[i])
	{
		if (str[i] == ',')
			commas++;
		i++;
	}
	if (commas != 2)
		return (1);
	return (0);
}

char	**validate_and_split_rgb(t_data *data, char *ptr)
{
	char	**rgb;

	while (*ptr == ' ')
		ptr++;
	if (check_commas(ptr))
		error_exit(data, "Invalid RGB format");
	rgb = ft_split(ptr, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
	{
		ft_free_tab(rgb);
		error_exit(data, "Invalid RGB format");
	}
	if (ft_digits(rgb) == 0)
	{
		ft_free_tab(rgb);
		error_exit(data, "Invalid RGB format");
	}
	return (rgb);
}

static void	parse_rgb_values(t_data *data, char *ptr, int *color)
{
	char	**rgb;
	int		i;
	int		value;

	rgb = validate_and_split_rgb(data, ptr);
	i = 0;
	while (i < 3)
	{
		if (!rgb[i] || !*rgb[i] || rgb[i][0] == '\n')
		{
			ft_free_tab(rgb);
			error_exit(data, "Missing RGB value");
		}
		value = ft_atoi(rgb[i]);
		if (value < 0 || value > 255)
		{
			ft_free_tab(rgb);
			error_exit(data, "RGB value out of range");
		}
		color[i] = value;
		i++;
	}
	ft_free_tab(rgb);
}

void	parse_color(t_data *data, char *line)
{
	int		*color;
	char	*ptr;

	if (ft_strncmp(line, "C ", 2) == 0)
	{
		if (data->has_ceiling_color)
			error_exit(data, "Duplicate ceiling color");
		color = data->ceiling_color;
		data->has_ceiling_color = 1;
		ptr = line + 2;
	}
	else if (ft_strncmp(line, "F ", 2) == 0)
	{
		if (data->has_floor_color)
			error_exit(data, "Duplicate floor color");
		color = data->floor_color;
		data->has_floor_color = 1;
		ptr = line + 2;
	}
	else
		error_exit(data, "Invalid color line");
	parse_rgb_values(data, ptr, color);
}
