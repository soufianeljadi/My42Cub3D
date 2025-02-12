#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

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
    int player_x;
    int player_y;
    char player_dir;
} t_data;






void parse_cub_file(t_data *data, const char *filename);
void parse_texture(t_data *data, char *line);
void parse_color(t_data *data, char *line);
void parse_map(t_data *data, char *line);
void free_data(t_data *data);
void error_exit(t_data *data, const char *msg);

char **ft_split(char const *s, char c);
void	ft_free_tab(char **tab);

#endif