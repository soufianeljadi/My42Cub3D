/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:52:38 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 14:57:05 by sel-jadi         ###   ########.fr       */
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

char	*extract_texture_path(t_data *data, char *line)
{
	char	*path;
	char	*full_path;
	char	*temp;

	full_path = NULL;
	path = ft_strtok(NULL, " ");
	if (!path)
		error_exit(data, "Invalid texture line: missing path");
	full_path = ft_strdup(path);
	if (!full_path)
		error_exit(data, "Memory allocation failed");
	path = ft_strtok(NULL, " ");
	while (path != NULL)
	{
		temp = ft_strjoin(full_path, " ");
		if (!temp)
			return (free(full_path), error_exit(data, "Allocation failed"), 1);
		free(full_path);
		full_path = ft_strjoin(temp, path);
		free(temp);
		if (!full_path)
			error_exit(data, "Memory allocation failed");
		path = ft_strtok(NULL, " ");
	}
	return (full_path);
}
