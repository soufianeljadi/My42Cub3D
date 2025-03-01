/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:52:56 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/03/01 15:56:57 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	check_textures(t_data *data)
{
	if (!data->has_north_texture)
		error_exit(data, "Missing north texture");
	if (!data->has_south_texture)
		error_exit(data, "Missing south texture");
	if (!data->has_west_texture)
		error_exit(data, "Missing west texture");
	if (!data->has_east_texture)
		error_exit(data, "Missing east texture");
}

void	parse_texture(t_data *data, char *line)
{
	char	*path;
	char	*identifier;
	int		fd;
	char	*trimmed_path;
	char	*full_path;
	char	*dot;
	char	*temp;

	(1) && (path = NULL, identifier = NULL, fd = 0, trimmed_path = NULL, full_path = NULL, dot = NULL, temp = NULL);
	identifier = ft_strtok(line, " ");
	if (!identifier)
		error_exit(data, "Invalid texture line: missing identifier");
	path = ft_strtok(NULL, " ");
	if (!path)
		error_exit(data, "Invalid texture line: missing path");
	full_path = ft_strdup(path);
	while ((path = ft_strtok(NULL, " ")))
	{
		temp = ft_strjoin(full_path, " ");
		free(full_path);
		full_path = ft_strjoin(temp, path);
		free(temp);
	}
	trimmed_path = ft_strtrim(full_path, " ");
	if (!trimmed_path)
		error_exit(data, "Failed to trim texture path");
	free(full_path);
	full_path = ft_strtrim(trimmed_path,"\n");
	if (!full_path)
		error_exit(data, "Failed to trim texture path");
	free(trimmed_path);
	trimmed_path = full_path;
	if (ft_strchr(trimmed_path, ' '))
	{
		free(trimmed_path);
		error_exit(data, "Texture path contains spaces");
	}
	dot = ft_strrchr(trimmed_path, '.');
	if (ft_strcmp(dot, ".xpm") != 0 || trimmed_path[ft_strlen(trimmed_path) - 5] == '/')
	{
		free(trimmed_path);
		error_exit(data, "Invalid texture file extension");
	}
	fd = open(trimmed_path, O_RDONLY);
	if (fd == -1)
	{
		free(trimmed_path);
		error_exit(data, "Failed to open texture file");
	}
	close(fd);
	if (ft_strcmp(identifier, "NO") == 0)
	{
		if (data->has_north_texture)
			error_exit(data, "Duplicate north texture");
		free(data->north_texture);
		data->north_texture = trimmed_path;
		data->has_north_texture = 1;
	}
	else if (ft_strcmp(identifier, "SO") == 0)
	{
		if (data->has_south_texture)
			error_exit(data, "Duplicate south texture");
		free(data->south_texture);
		data->south_texture = trimmed_path;
		data->has_south_texture = 1;
	}
	else if (ft_strcmp(identifier, "WE") == 0)
	{
		if (data->has_west_texture)
			error_exit(data, "Duplicate west texture");
		free(data->west_texture);
		data->west_texture = trimmed_path;
		data->has_west_texture = 1;
	}
	else if (ft_strcmp(identifier, "EA") == 0)
	{
		if (data->has_east_texture)
			error_exit(data, "Duplicate east texture");
		free(data->east_texture);
		data->east_texture = trimmed_path;
		data->has_east_texture = 1;
	}
	else
	{
		free(trimmed_path);
		error_exit(data, "Invalid texture identifier");
	}
}
