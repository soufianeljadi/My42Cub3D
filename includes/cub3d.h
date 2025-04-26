/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aben-hss <aben-hss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:45:29 by aben-hss          #+#    #+#             */
/*   Updated: 2025/04/26 18:44:37 by aben-hss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include <unistd.h>
# include <limits.h>
# include "../.MLX42/include/MLX42/MLX42.h"
# define MAX_LINE_LENGTH 1024

# define SCREEN_WIDTH 1800
# define SCREEN_HEIGHT 1200
# define TILE  64
# define FOV 1.04

typedef struct s_data {
	char	*north_texture;
	char	*south_texture;
	char	*west_texture;
	char	*east_texture;
	int		floor_color[3];
	int		ceiling_color[3];
	char	**map;
	int		map_width;
	int		map_height;
	int		map_capacity;
	int		player_x;
	int		player_y;
	char	player_dir;
	int		has_north_texture;
	int		has_south_texture;
	int		has_west_texture;
	int		has_east_texture;
	int		has_floor_color;
	int		has_ceiling_color;
}	t_data;

typedef struct s_player {
	double	x;
	double	y;
	double	dir;
}	t_player;

typedef struct s_params{
	int				wall_start;
	int				wall_end;
	int				wall_height;
	mlx_t			*mlx;
	char			**map;
	mlx_image_t		*img;
	mlx_texture_t	*south_t;
	mlx_texture_t	*west_t;
	mlx_texture_t	*east_t;
	mlx_texture_t	*north_t;
	t_player		player;
	t_data			data;
	int				height;
	int				width;
}	t_params;

typedef struct s_ray {
	double	angle;
	double	ver_dis;
	double	hor_dis;
	double	distance;
	int		map_x;
	int		map_y;
	int		facing_up;
	int		facing_right;
}	t_ray;

typedef struct s_wall_data
{
	int	x;
	int	start;
	int	end;
	int	height;
}	t_wall_data;

typedef struct s_draw_data
{
	t_params		*params;
	t_ray			ray;
	mlx_image_t		*img;
	t_wall_data		wall;
	mlx_texture_t	*text;
	float			text_x;
}	t_draw_data;

typedef struct s_intersection {
	double	a_y;
	double	a_x;
	double	y_step;
	double	x_step;
	int		map_y;
	int		map_x;
}	t_intersection;

//paerse_cub
int		check_commas(char *str);
void	check_colors(t_data *data);
void	parse_color(t_data *data, char *line);
int		has_cub_extension(const char *filename);
void	process_cub_line(t_data *data, char *line, int *map_started);
void	open_cub_file(t_data *data, int fd);
void	parse_cub_file(t_data *data, const char *filename);
void	ensure_map_capacity(t_data *data, char *trimmed_line);
void	parse_map(t_data *data, char *line);
char	*clean_texture_path(t_data *data, char *full_path);
int		validate_texture_path(char *path);
void	single_texture(t_data *data, char **ptr, int *flag, char *path);
void	assign_texture(t_data *data, char *identifier, char *path);
void	parse_texture(t_data *data, char *line);
void	check_textures(t_data *data);
char	*extract_texture_path(t_data *data);
void	validate_map(t_data *data);
void	validate_vertical_walls(t_data *data);
size_t	get_max_line_length(t_data *data);
void	pad_map_line(t_data *data, int i, size_t max_len);
void	pad_map_lines(t_data *data);
void	validate_player(t_data *data, char c, int i, int j);
void	validate_map_characters(t_data *data);
void	validate_border_line(t_data *data, int line_index, char *error_msg);
void	validate_horizontal_walls(t_data *data, int i);
void	validate_adjacent_spaces(t_data *data, int i, int j);
void	free_data(t_data *data);
void	error_exit(t_data *data, const char *msg);
char	**ft_split(char const *s, char c);
void	ft_free_tab(char **tab);

//get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*ft_line(char *src);
int		ft_search(char *s, int c);
char	*ft_move_start(char *start);
char	*ft_strjoin_gnl(char *start, char *buff);
char	*ft_join(char *dest, char *s1, char *s2);

// render
void	player_rotation(t_params *params);
void	calc_new_pos(t_params *params, double a_off, double *t_x, double *t_y);
void	forward_backward(t_params *params);
void	move_sideways(t_params *params);
void	player_movement(t_params *params);
void	get_hors_inter(t_params *par, t_ray *ray);
void	get_verts_inter(t_params *par, t_ray *ray);
void	set_to_pos(t_params *params, double x, double y);
void	key_hook(void *data);
void	handle_flip_texture(t_draw_data *data);
void	draw_ceiling(t_draw_data *data);
void	draw_wall_column(t_draw_data *data);
void	draw_floor(t_draw_data *data);
void	draw_wall(t_params *params, t_ray ray, mlx_image_t *img, int x);
void	set_position(t_player *player, char **map, t_data *data);
void	init_draw_data(t_draw_data *data, t_ray ray, \
	mlx_image_t *img, t_wall_data wall);
void	calculate_texture_coordinates(t_draw_data *data);
void	cast_rays(t_params *params, mlx_image_t *img);
void	cast_player(t_params *params);

//utils
char	*ft_strtok(char *str, const char *delim);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		ft_atoi(char *str);
char	*ft_strdup(char *s1);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *str, int c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strlcpy(char *dst, const char *src, int dstsize);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s, char const *set);
int		ft_isdigit(int c);
int		ft_digits(char **str);
#endif