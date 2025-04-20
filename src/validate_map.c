
#include "../includes/cub3d.h"

static void	validate_player(t_data *data, char c, int i, int j)
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
		|| (data->map[data->map_height - 1] == data->map[i] && player_count == 0))
		error_exit(data, "Map must contain exactly one player spawn point");
}

static void	validate_map_characters(t_data *data)
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

static void	validate_border_line(t_data *data, int line_index, char *error_msg)
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

static void	validate_horizontal_walls(t_data *data, int i)
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

static void	validate_adjacent_spaces(t_data *data, int i, int j)
{
	int	end;

	end = ft_strlen(data->map[i]) - 1;
	if (data->map[i][j] == '0' || ft_strchr("NSEW", data->map[i][j]))
	{
		if ((j > 0 && data->map[i][j - 1] == ' ') || data->map[i][j - 1] == '\0')
			error_exit(data, "Empty space adjacent to space in the same line");
		if ((j < end && data->map[i][j + 1] == ' ') || data->map[i][j + 1] == '\0')
			error_exit(data, "Empty space adjacent to space in the same line");
		if ((i > 0 && data->map[i - 1][j] == ' ') || data->map[i - 1][j] == '\0')
			error_exit(data, "Empty space adjacent to space in the line above");
		if ((i < data->map_height - 1 && data->map[i + 1][j] == ' ') 
			|| data->map[i + 1][j] == '\0')
			error_exit(data, "Empty space adjacent to space in the line below");
	}
}

static void	validate_vertical_walls(t_data *data)
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

static size_t	get_max_line_length(t_data *data)
{
	size_t	max_len;
	int		i;
	size_t len;

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

static void	pad_map_line(t_data *data, int i, size_t max_len)
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

static void	pad_map_lines(t_data *data)
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
	validate_border_line(data, 0, "First line must contain only walls and spaces");
	validate_border_line(data, data->map_height - 1, 
		"Last line must contain only walls and spaces");
	validate_vertical_walls(data);
	pad_map_lines(data);
}