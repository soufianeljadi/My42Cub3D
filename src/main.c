/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:54:20 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/26 18:47:44 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	on_window_close(void *param)
{
	t_params	*params;

	params = (t_params *)param;
	mlx_delete_texture(params->south_t);
	mlx_delete_texture(params->north_t);
	mlx_delete_texture(params->west_t);
	mlx_delete_texture(params->east_t);
	if (params->img)
		mlx_delete_image(params->mlx, params->img);
	free_data(&params->data);
	mlx_terminate(params->mlx);
	exit(EXIT_SUCCESS);
}

int	init_mlx(t_params *params)
{
	params->img = NULL;
	params->mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "3D raycasting", false);
	if (!params->mlx)
		return (0);
	params->width = TILE * ft_strlen(*params->map);
	params->height = TILE * params->data.map_height;
	params->img = mlx_new_image(params->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!params->img)
		return (0);
	mlx_loop_hook(params->mlx, key_hook, params);
	mlx_close_hook(params->mlx, on_window_close, (void *)params);
	mlx_loop(params->mlx);
	return (1);
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
	if (!init_mlx(&params))
		on_window_close(&params);
	return (0);
}
