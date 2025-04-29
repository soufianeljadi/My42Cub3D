/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:47:38 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/27 15:03:13 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	has_cub_extension(const char *filename)
{
	char	*dot;

	dot = ft_strrchr(filename, '.');
	if (!dot || dot == filename)
		return (0);
	if (filename[ft_strlen(filename) - 5] == '/')
		return (0);
	return (ft_strcmp(dot, ".cub") == 0);
}

void	process_cub_line(t_data *data, char *line, int *map_started, int fd)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
	{
		parse_texture(data, line, fd);
	}
	else if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
	{
		parse_color(data, line, fd);
	}
	else if (ft_strchr("01NSEW ", line[0]))
	{
		if (!*map_started)
			*map_started = 1;
		parse_map(data, line, fd);
	}
	else if (line[0] != '\n')
		(close(fd)), (free(line)), error_exit(data, "Invalid line in .cub");
	else if (*map_started && line[0] == '\n')
		(close(fd)), (free(line)), error_exit(data, "Newline in invalid place");
}

void	open_cub_file(t_data *data, int fd)
{
	char	*line;
	int		map_started;

	(1) && (line = NULL, map_started = 0);
	line = get_next_line(fd);
	while (line != NULL)
	{
		process_cub_line(data, line, &map_started, fd);
		if (map_started && !ft_strchr("01 ", line[0]))
			(close(fd)), (free(line)),
			error_exit(data, "map should be at the bottom ");
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	if (!map_started)
		(close(fd)), error_exit(data, "Map not found in .cub file");
}

void	parse_cub_file(t_data *data, const char *filename)
{
	int		fd;

	if (!has_cub_extension(filename))
	{
		write(2, "Error: File ", 12);
		write(2, filename, ft_strlen(filename));
		write(2, " does not have a .cub extension\n", 32);
		exit(1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		error_exit(data, "Failed to open file");
	open_cub_file(data, fd);
	close(fd);
	check_textures(data);
	check_colors(data);
	validate_map(data);
}
