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

static char *extract_texture_path(t_data *data, char *line)
{
    char *path;
    char *full_path;
    char *temp;
	
    full_path = NULL;
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
    return (full_path);
}

static char *clean_texture_path(t_data *data, char *full_path)
{
    char *trimmed_path;
    char *result;

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

static void validate_texture_path(t_data *data, char *path)
{
    char *dot;

    if (ft_strchr(path, ' '))
        error_exit(data, "Texture path contains spaces");
    dot = ft_strrchr(path, '.');
    if (!dot || ft_strcmp(dot, ".png") != 0 || path[ft_strlen(path) - 5] == '/')
        error_exit(data, "Invalid texture file extension");
    if (open(path, O_RDONLY) == -1)
        error_exit(data, "Failed to open texture file");
}

static void assign_single_texture(t_data *data, char **texture_ptr, int *has_texture_flag, char *path, const char *error_msg)
{
    if (*has_texture_flag)
        error_exit(data, error_msg);
    free(*texture_ptr);
    *texture_ptr = path;
    *has_texture_flag = 1;
}

static void assign_texture(t_data *data, char *identifier, char *path)
{
    if (ft_strcmp(identifier, "NO") == 0)
        assign_single_texture(data, &data->north_texture, &data->has_north_texture,
                            path, "Duplicate north texture");
    else if (ft_strcmp(identifier, "SO") == 0)
        assign_single_texture(data, &data->south_texture, &data->has_south_texture,
                            path, "Duplicate south texture");
    else if (ft_strcmp(identifier, "WE") == 0)
        assign_single_texture(data, &data->west_texture, &data->has_west_texture,
                            path, "Duplicate west texture");
    else if (ft_strcmp(identifier, "EA") == 0)
        assign_single_texture(data, &data->east_texture, &data->has_east_texture,
                            path, "Duplicate east texture");
    else
    {
        free(path);
        error_exit(data, "Invalid texture identifier");
    }
}

void parse_texture(t_data *data, char *line)
{
    char *identifier;
    char *full_path;
    char *clean_path;

    identifier = ft_strtok(line, " ");
    if (!identifier)
        error_exit(data, "Invalid texture line: missing identifier");
    
    full_path = extract_texture_path(data, line);
    clean_path = clean_texture_path(data, full_path);
    validate_texture_path(data, clean_path);
    assign_texture(data, identifier, clean_path);
}
