/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:52:56 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 11:58:29 by sel-jadi         ###   ########.fr       */
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
	free(full_path);
	result = ft_strtrim(trimmed_path, "\n");
	if (!result)
		error_exit(data, "Failed to trim texture path");
	free(trimmed_path);
	return (result);
}

void	validate_texture_path(t_data *data, char *path)
{
	char	*dot;

	if (ft_strchr(path, ' '))
		error_exit(data, "Texture path contains spaces");
	dot = ft_strrchr(path, '.');
	if (!dot || ft_strcmp(dot, ".png") != 0 || path[ft_strlen(path) - 5] == '/')
		error_exit(data, "Invalid texture file extension");
	if (open(path, O_RDONLY) == -1)
		error_exit(data, "Failed to open texture file");
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
		error_exit(data, "Invalid texture identifier");
	}
}

void	parse_texture(t_data *data, char *line)
{
	char	*identifier;
	char	*full_path;
	char	*clean_path;

	identifier = ft_strtok(line, " ");
	if (!identifier)
		error_exit(data, "Invalid texture line: missing identifier");
	full_path = extract_texture_path(data, line);
	clean_path = clean_texture_path(data, full_path);
	validate_texture_path(data, clean_path);
	assign_texture(data, identifier, clean_path);
}
