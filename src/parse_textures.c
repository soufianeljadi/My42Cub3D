/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:52:56 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/26 20:40:50 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	*clean_texture_path(t_data *data, char *full_path)
{
	char	*trimmed_path;
	char	*result;

	trimmed_path = ft_strtrim(full_path, " ");
	if (!trimmed_path)
		error_exit(data, "Failed to trim texture path");
	result = ft_strtrim(trimmed_path, "\n");
	if (!result)
	{
		free(trimmed_path);
		error_exit(data, "Failed to trim texture path");
	}
	free(trimmed_path);
	return (result);
}

int	validate_texture_path(char *path)
{
	char			*dot;
	int				fd;
	mlx_texture_t	*text;

	dot = ft_strrchr(path, '.');
	if (!dot || ft_strcmp(dot, ".png") != 0 || path[ft_strlen(path) - 5] == '/')
		return (-1);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (-1);
	close(fd);
	text = mlx_load_png(path);
	if (!text)
		return (-1);
	mlx_delete_texture(text);
	return (0);
}

void	single_texture(t_data *data, char **ptr, int *flag, char *path)
{
	if (*flag)
		error_exit(data, "Duplicate texture");
	free(*ptr);
	*ptr = path;
	*flag = 1;
}

void	assign_texture(t_data *data, char *identifier, char *path)
{
	if (!data || !identifier || !path)
	{
		free(path);
		error_exit(data, "Null pointer in assign_texture");
	}
	if (ft_strcmp(identifier, "NO") == 0)
		single_texture(data, &data->north_texture, \
		&data->has_north_texture, path);
	else if (ft_strcmp(identifier, "SO") == 0)
		single_texture(data, &data->south_texture, \
		&data->has_south_texture, path);
	else if (ft_strcmp(identifier, "WE") == 0)
		single_texture(data, &data->west_texture, \
		&data->has_west_texture, path);
	else if (ft_strcmp(identifier, "EA") == 0)
		single_texture(data, &data->east_texture, \
		&data->has_east_texture, path);
	else
	{
		free(path);
		free(identifier);
		error_exit(data, "Invalid texture identifier");
	}
}

void	parse_texture(t_data *data, char *line, int fd)
{
	char	*identifier;
	char	*full_path;
	char	*clean_path;

	identifier = ft_strtok(line, " ");
	if (!identifier)
		close(fd), error_exit(data, "Invalid texture line: missing identifier");
	full_path = extract_texture_path(data);
	if (!full_path)
		close(fd), (free(identifier)), error_exit(data, "Failed to extract texture path");
	clean_path = clean_texture_path(data, full_path);
	free(full_path);
	if (!clean_path)
		close(fd), (free(identifier)),
			error_exit(data, "Failed to clean texture path");
	if (validate_texture_path(clean_path) == -1)
		return (free(clean_path), free(identifier), close(fd),
			error_exit(data, "Invalid texture path"));
	assign_texture(data, identifier, clean_path);
}
