/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jadi <sel-jadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:43:36 by sel-jadi          #+#    #+#             */
/*   Updated: 2025/04/22 12:26:11 by sel-jadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>

# define MAX_LINE_LENGTH 1024

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

//paerse_cub
int		check_commas(char *str);
void	check_colors(t_data *data);
void	parse_color(t_data *data, char *line);
int		has_cub_extension(const char *filename);
void	process_cub_line(t_data *data, char *line, int *map_started);
void	open_cub_file(t_data *data, int fd);
void	parse_cub_file(t_data *data, const char *filename);
void	ensure_map_capacity(t_data *data);
void	parse_map(t_data *data, char *line);
char	*clean_texture_path(t_data *data, char *full_path);
int		validate_texture_path(t_data *data, char *path);
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