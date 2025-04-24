/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:52:03 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/24 18:35:11 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	handle_flip_texture(t_draw_data *data)
{
	if ((data->ray.hor_dis < data->ray.ver_dis && !data->ray.facing_up)
		|| (data->ray.hor_dis >= data->ray.ver_dis && !data->ray.facing_right))
		data->text_x = data->text->width - data->text_x;
	if (data->text_x < 0)
		data->text_x = 0;
	if (data->text_x >= data->text->width)
		data->text_x = data->text->width;
}

void	draw_ceiling(t_draw_data *data)
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

void	draw_wall_column(t_draw_data *data)
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
		if ((uint8_t)tex_y >= data->text->height)
			tex_y = data->text->height - 1;
		pixel = &data->text->pixels[((tex_y * data->text->width)
				+ (int)data->text_x) * 4];
		color = (pixel[0] << 24) | (pixel[1] << 16)
			| (pixel[2] << 8) | pixel[3];
		mlx_put_pixel(data->img, data->wall.x, y, color);
		y++;
	}
}

void	draw_floor(t_draw_data *data)
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
	data.params = params;
	data.text = params->west_t;
	init_draw_data(&data, ray, img, (t_wall_data) \
		{x, wall_start, wall_end, wall_height});
	calculate_texture_coordinates(&data);
	handle_flip_texture(&data);
	draw_ceiling(&data);
	draw_wall_column(&data);
	draw_floor(&data);
}
