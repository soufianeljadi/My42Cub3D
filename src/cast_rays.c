/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:33:34 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/21 20:33:36 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include <string.h>

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
			if (ft_strchr("NSEW", map[y][x]))
			{
				player->x = (x * TILE) + TILE / 2;
				player->y = (y * TILE) + TILE / 2;
				player->dir = 0;
				if (map[y][x] == 'N')
					player->dir = 3 * M_PI / 2;
				else if (map[y][x] == 'S')
					player->dir = M_PI / 2;
				else if (map[y][x] == 'W')
					player->dir = M_PI;
				return ;
			}
			x++;
		}
		y++;
	}
}

void	get_hors_inter(t_params *params, t_ray *ray)
{
	double	a_y;
	double	a_x;
	double	y_step;
	double	x_step;
	int		map_y;
	int		map_x;

	a_y = floor(params->player.y / TILE) * TILE;
	if (!ray->facing_up)
		a_y += TILE;
	else
		a_y -= 10e-5;
	a_x = ((a_y - params->player.y) / tan(ray->angle)) + params->player.x;
	y_step = TILE;
	if (ray->facing_up)
		y_step = -y_step;
	x_step = y_step / tan(ray->angle);
	while (a_y > 0 && a_y < params->height && a_x > 0 && a_x < params->width)
	{
		map_y = a_y / TILE;
		map_x = a_x / TILE;
		if (params->map[map_y][map_x] == '1')
		{
			ray->hor_dis = sqrt(pow(a_y - params->player.y, 2) \
							+ pow(a_x - params->player.x, 2));
			return ;
		}
		a_y += y_step;
		a_x += x_step;
	}
}

void	get_verts_inter(t_params *params, t_ray *ray)
{
	double	a_y;
	double	a_x;
	double	y_step;
	double	x_step;
	int		map_y;
	int		map_x;

	a_x = floor(params->player.x / TILE) * TILE;
	if (ray->facing_right)
		a_x += TILE;
	else
		a_x -= 10e-5;
	a_y = ((a_x - params->player.x) * tan(ray->angle)) + params->player.y;
	x_step = TILE;
	if (!ray->facing_right)
		x_step = -x_step;
	y_step = x_step * tan(ray->angle);
	while (a_y > 0 && a_y < params->height && a_x > 0 && a_x < params->width)
	{
		map_y = a_y / TILE;
		map_x = a_x / TILE;
		if (params->map[map_y][map_x] == '1')
		{
			ray->ver_dis = sqrt(pow(a_y - params->player.y, 2) \
							+ pow(a_x - params->player.x, 2));
			return ;
		}
		a_y += y_step;
		a_x += x_step;
	}
}

static void	init_draw_data(t_draw_data *data, t_params *params, t_ray ray,
			mlx_image_t *img, t_wall_data wall)
{
	data->params = params;
	data->ray = ray;
	data->img = img;
	data->wall = wall;
	data->text = params->west_t;
}

static void	calculate_texture_coordinates(t_draw_data *data)
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

static void	handle_flip_texture(t_draw_data *data)
{
	if ((data->ray.hor_dis < data->ray.ver_dis && !data->ray.facing_up)
		|| (data->ray.hor_dis >= data->ray.ver_dis && !data->ray.facing_right))
		data->text_x = data->text->width - data->text_x - 1;
	if (data->text_x < 0)
		data->text_x = 0;
	if (data->text_x >= data->text->width)
		data->text_x = data->text->width - 1;
}

static void	draw_ceiling(t_draw_data *data)
{
	int	k;
	int	color;

	k = 0;
	color = (data->params->data.ceiling_color[0] << 24)
		| (data->params->data.ceiling_color[1] << 16)
		| (data->params->data.ceiling_color[2] << 8) | 255;
	while (k < data->wall.start)
	{
		mlx_put_pixel(data->img, data->wall.x, k++, color);
	}
}

static void	draw_wall_column(t_draw_data *data)
{
	int		y;
	double	pos;
	int		tex_y;
	int		color;
	uint8_t	*pixel;

	y = data->wall.start;
	while (y < data->wall.end)
	{
		pos = (double)(y - (SCREEN_HEIGHT / 2 - data->wall.height / 2))
			/ (double)(data->wall.height);
		tex_y = (int)(pos * data->text->height);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= data->text->height)
			tex_y = data->text->height - 1;
		pixel = &data->text->pixels[((tex_y * data->text->width)
				+ (int)data->text_x) * 4];
		color = (pixel[0] << 24) | (pixel[1] << 16)
			| (pixel[2] << 8) | pixel[3];
		mlx_put_pixel(data->img, data->wall.x, y, color);
		y++;
	}
}

static void	draw_floor(t_draw_data *data)
{
	int	k;
	int	color;

	k = data->wall.end;
	color = (data->params->data.floor_color[0] << 24)
		| (data->params->data.floor_color[1] << 16)
		| (data->params->data.floor_color[2] << 8) | 255;
	while (k < SCREEN_HEIGHT)
	{
		mlx_put_pixel(data->img, data->wall.x, k++, color);
	}
}

void	draw_wall(t_params *params, t_ray ray, mlx_image_t *img, int x)
{
	int			wall_height;
	int			wall_start;
	int			wall_end;
	t_draw_data	data;

	wall_height = TILE * SCREEN_HEIGHT / ray.distance;
	wall_start = fmax(0, SCREEN_HEIGHT / 2 - wall_height / 2);
	wall_end = fmin(SCREEN_HEIGHT - 1, SCREEN_HEIGHT / 2 + wall_height / 2);
	if (wall_height < 0)
		wall_height = 0;
	init_draw_data(&data, params, ray, img, (t_wall_data) \
		{x, wall_start, wall_end, wall_height});
	calculate_texture_coordinates(&data);
	handle_flip_texture(&data);
	draw_ceiling(&data);
	draw_wall_column(&data);
	draw_floor(&data);
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

void	player_rotation(t_params *params)
{
	if (mlx_is_key_down(params->mlx, MLX_KEY_RIGHT))
		params->player.dir += 0.05;
	if (mlx_is_key_down(params->mlx, MLX_KEY_LEFT))
		params->player.dir -= 0.05;
	if (params->player.dir < 0)
		params->player.dir += M_PI * 2;
	if (params->player.dir > 2 * M_PI)
		params->player.dir -= 2 * M_PI;
}

void	calc_new_pos(t_params *params, double a_off, double *t_x, double *t_y)
{
	double	move_angle;
	double	move_speed;

	move_speed = 3.0;
	move_angle = params->player.dir + a_off;
	*t_x = params->player.x + cos(move_angle) * move_speed;
	*t_y = params->player.y + sin(move_angle) * move_speed;
}

void	forward_backward(t_params *params)
{
	double	t_x;
	double	t_y;

	if (mlx_is_key_down(params->mlx, MLX_KEY_W))
	{
		calc_new_pos(params, 0, &t_x, &t_y);
		set_to_pos(params, t_x, t_y);
	}	
	if (mlx_is_key_down(params->mlx, MLX_KEY_S))
	{
		calc_new_pos(params, M_PI, &t_x, &t_y);
		set_to_pos(params, t_x, t_y);
	}
}

void	move_sideways(t_params *params)
{
	double	t_x;
	double	t_y;

	if (mlx_is_key_down(params->mlx, MLX_KEY_D))
	{
		calc_new_pos(params, M_PI / 2, &t_x, &t_y);
		set_to_pos(params, t_x, t_y);
	}	
	if (mlx_is_key_down(params->mlx, MLX_KEY_A))
	{
		calc_new_pos(params, -M_PI / 2, &t_x, &t_y);
		set_to_pos(params, t_x, t_y);
	}
}

void	player_movement(t_params *params)
{
	player_rotation(params);
	forward_backward(params);
	move_sideways(params);
	if (mlx_is_key_down(params->mlx, MLX_KEY_ESCAPE))
		exit(0);
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

int	main(int ac, char **av)
{
	static t_data	data;
	t_params		params;

	(void)ac;
	parse_cub_file(&data, av[1]);
	params.map = data.map;
	params.data = data;
	set_position(&params.player, params.map, &data);
	params.south_t = mlx_load_png(data.south_texture);
	params.north_t = mlx_load_png(data.north_texture);
	params.east_t = mlx_load_png(data.east_texture);
	params.west_t = mlx_load_png(data.west_texture);
	params.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "3D raycasting", false);
	params.width = TILE * strlen(*params.map);
	params.height = TILE * data.map_height;
	params.img = mlx_new_image(params.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	mlx_loop_hook(params.mlx, key_hook, &params);
	mlx_loop(params.mlx);
	return (0);
}
