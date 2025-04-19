#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>
#include "../MLX42/include/MLX42/MLX42.h"
#define MAX_LINE_LENGTH 1024

#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 1200
#define TILE  64
#define FOV (M_PI/ 3)

typedef struct s_data {
    char *north_texture;
    char *south_texture;
    char *west_texture;
    char *east_texture;
    int floor_color[3];
    int ceiling_color[3];
    char **map;
    int map_width;
    int map_height;
    int map_capacity;
    int player_x;
    int player_y;
    char player_dir;
    int has_north_texture;
    int has_south_texture;
    int has_west_texture;
    int has_east_texture;
    int has_floor_color;
    int has_ceiling_color;
} t_data;

typedef struct s_player {
    double x;
    double y;
    double dir;
    double plane_x;
    double plane_y;
    double move_speed;
    double rot_speed;
} t_player;

typedef struct s_params{
	mlx_t *mlx;
	char **map;
	mlx_image_t *img;
    mlx_texture_t   *text;
	t_player player;
	t_data data;
	int height;
	int width;
}t_params;


typedef struct s_ray {
    double angle;
    double dir_x;
    double dir_y;
    double ver_dis;
    double hor_dis;
    double perp_wall_dist;
    double distance;
    int map_x;
    int map_y;
    int step_x;
    int step_y;
    int facing_up;
    int facing_right;
    int hit;
    int side;
} t_ray;

typedef struct s_map {
    char **grid;
    int width;
    int height;
} t_map;

typedef struct s_render {
    mlx_image_t *img;
    int screen_width;
    int screen_height;
    int floor_color;
    int ceiling_color;
    int *textures[8];
} t_render;

typedef struct s_game {
    t_player player;
    t_map map;
    t_render render;
    mlx_t *mlx;
} t_game;


// mlx_t *mlx;
// char **map;
// mlx_image_t *img;



void parse_cub_file(t_data *data, const char *filename);
void parse_texture(t_data *data, char *line);
void parse_color(t_data *data, char *line);
void parse_map(t_data *data, char *line);
void free_data(t_data *data);
void error_exit(t_data *data, const char *msg);

char **ft_split(char const *s, char c);
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
char    *ft_strtok(char *str, const char *delim);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int	    ft_atoi(char *str);
char	*ft_strdup(char *s1);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strchr(const char *str, int c);
int	    ft_strcmp(const char *s1, const char *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int	    ft_strlcpy(char *dst, const char *src, int dstsize);
char    *ft_strrchr(const char *s, int c);
char    *ft_strtrim(char const *s, char const *set);
int 	ft_isdigit(int c);
int 	ft_all_digits(char **str);
#endif