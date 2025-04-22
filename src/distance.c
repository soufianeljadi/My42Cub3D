/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distance.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:46:42 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/22 15:02:57 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	set_to_pos(t_params *params, double x, double y)
{
	float	radius;
	int		i;
	int		j;
	int		map_y;
	int		map_x;

	radius = 3.0;
	i = -radius;
	j = i;
	while (i <= radius)
	{
		j = -radius;
		while (j <= radius)
		{
			map_y = (y + j) / TILE;
			map_x = (x + i) / TILE;
			if (params->map[map_y][map_x] == '1')
				return ;
			j += radius;
		}
		i += radius;
	}
	params->player.x = x;
	params->player.y = y;
}

void	get_hors_inter(t_params *par, t_ray *ray)
{
	t_intersection	i;

	i.a_y = floor(par->player.y / TILE) * TILE;
	if (!ray->facing_up)
		i.a_y += TILE;
	else
		i.a_y -= 10e-5;
	i.a_x = ((i.a_y - par->player.y) / tan(ray->angle)) + par->player.x;
	i.y_step = TILE;
	if (ray->facing_up)
		i.y_step = -i.y_step;
	i.x_step = i.y_step / tan(ray->angle);
	while (i.a_y > 0 && i.a_y < par->height && i.a_x > 0 && i.a_x < par->width)
	{
		i.map_y = i.a_y / TILE;
		i.map_x = i.a_x / TILE;
		if (par->map[i.map_y][i.map_x] == '1')
		{
			ray->hor_dis = sqrt(pow(i.a_y - par->player.y, 2) + \
								pow(i.a_x - par->player.x, 2));
			return ;
		}
		i.a_y += i.y_step;
		i.a_x += i.x_step;
	}
}

void	get_verts_inter(t_params *par, t_ray *ray)
{
	t_intersection	i;

	i.a_x = floor(par->player.x / TILE) * TILE;
	if (ray->facing_right)
		i.a_x += TILE;
	else
		i.a_x -= 10e-5;
	i.a_y = ((i.a_x - par->player.x) * tan(ray->angle)) + par->player.y;
	i.x_step = TILE;
	if (!ray->facing_right)
		i.x_step = -i.x_step;
	i.y_step = i.x_step * tan(ray->angle);
	while (i.a_y > 0 && i.a_y < par->height && i.a_x > 0 && i.a_x < par->width)
	{
		i.map_y = i.a_y / TILE;
		i.map_x = i.a_x / TILE;
		if (par->map[i.map_y][i.map_x] == '1')
		{
			ray->ver_dis = sqrt(pow(i.a_y - par->player.y, 2) + \
								pow(i.a_x - par->player.x, 2));
			return ;
		}
		i.a_y += i.y_step;
		i.a_x += i.x_step;
	}
}

void	key_hook(void *data)
{
	t_params	*params;

	params = data;
	if (mlx_is_key_down(params->mlx, MLX_KEY_ESCAPE))
		exit(0);
	player_movement(params);
	cast_player(params);
}
