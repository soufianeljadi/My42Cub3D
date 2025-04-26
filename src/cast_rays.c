/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:33:34 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/26 16:48:26 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	set_position(t_player *player, char **map, t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->map_height)
	{
		x = 0;
		while (map[y][x])
		{
			if (ft_strchr("NSEW", map[y][x++]))
			{
				player->x = ((x - 1) * TILE) + (TILE / 2);
				player->y = (y * TILE) + TILE / 2;
				player->dir = 0;
				if (map[y][x - 1] == 'N')
					player->dir = 3 * M_PI / 2;
				else if (map[y][x - 1] == 'S')
					player->dir = M_PI / 2;
				else if (map[y][x - 1] == 'W')
					player->dir = M_PI;
				return ;
			}
		}
		y++;
	}
}

void	init_draw_data(t_draw_data *data, t_ray ray, \
			mlx_image_t *img, t_wall_data wall)
{
	data->ray = ray;
	data->img = img;
	data->wall = wall;
}

void	calculate_texture_coordinates(t_draw_data *data)
{
	double	wall_x;

	if (data->ray.hor_dis < data->ray.ver_dis)
	{
		if (data->ray.facing_up)
			data->text = data->params->north_t;
		else
			data->text = data->params->south_t;
		wall_x = data->params->player.x + data->ray.hor_dis
			* cos(data->ray.angle);
		wall_x /= TILE;
		wall_x = wall_x - floor(wall_x);
	}
	else
	{
		if (data->ray.facing_right)
			data->text = data->params->east_t;
		wall_x = data->params->player.y + data->ray.ver_dis
			* sin(data->ray.angle);
		wall_x /= TILE;
		wall_x = wall_x - floor(wall_x);
	}
	data->text_x = ((float)data->text->width * wall_x);
}

void	cast_rays(t_params *params, mlx_image_t *img)
{
	int		x;
	t_ray	ray;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		ray.angle = params->player.dir - (FOV / 2.0) \
			+ ((FOV / SCREEN_WIDTH) * x);
		if (ray.angle < 0)
			ray.angle += 2 * M_PI;
		if (ray.angle > 2 * M_PI)
			ray.angle -= 2 * M_PI;
		ray.facing_up = (ray.angle >= M_PI && ray.angle <= M_PI * 2);
		ray.facing_right = (!(ray.angle <= 3 * M_PI / 2 && \
			ray.angle >= M_PI / 2));
		ray.hor_dis = INT_MAX;
		ray.ver_dis = INT_MAX;
		get_hors_inter(params, &ray);
		get_verts_inter(params, &ray);
		ray.distance = fmin(ray.hor_dis, ray.ver_dis);
		ray.distance *= cos(params->player.dir - ray.angle);
		draw_wall(params, ray, img, x);
		x++;
	}
}

void	cast_player(t_params *params)
{
	static mlx_image_t	*img;

	if (img)
		mlx_delete_image(params->mlx, img);
	img = mlx_new_image(params->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	cast_rays(params, img);
	mlx_image_to_window(params->mlx, img, 0, 0);
}
