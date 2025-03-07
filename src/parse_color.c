#include "../includes/cub3d.h"

void check_colors(t_data *data)
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

void parse_color(t_data *data, char *line)
{
	int *color;
	int i;
	char **rgb= NULL;
	char *ptr = NULL;

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

	while (*ptr == ' ')
		ptr++;

	if (check_commas(ptr))
		error_exit(data, "Invalid RGB format");

	rgb = ft_split(ptr, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3] || ft_all_digits(rgb) == 0)
	{
		ft_free_tab(rgb);
		error_exit(data, "Invalid RGB format");
	}

	i = 0;
	while (i < 3)
	{
		if (!rgb[i] || !*rgb[i] || rgb[i][0] == '\n')
		{
			ft_free_tab(rgb);
			error_exit(data, "Missing RGB value");
		}
		int value = ft_atoi(rgb[i]);
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