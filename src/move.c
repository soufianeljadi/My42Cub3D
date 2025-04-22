/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:44:17 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/22 15:02:53 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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