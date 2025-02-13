#include "../includes/cub3d.h"

void parse_texture(t_data *data, char *line) {
    char *path;
    char *identifier;

    identifier = ft_strtok(line, " ");
    if (!identifier)
        error_exit(data, "Invalid texture line: missing identifier");

    path = ft_strtok(NULL, " ");
    if (!path)
        error_exit(data, "Invalid texture line: missing path");

    char *full_path = ft_strdup(path);
    while ((path = ft_strtok(NULL, " ")) != NULL) {
        char *temp = ft_strjoin(full_path, " ");
        free(full_path);
        full_path = ft_strjoin(temp, path);
        free(temp);
    }

    if (ft_strcmp(identifier, "NO") == 0) {
        free(data->north_texture); 
        data->north_texture = full_path;
    } else if (ft_strcmp(identifier, "SO") == 0) {
        free(data->south_texture); 
        data->south_texture = full_path;
    } else if (ft_strcmp(identifier, "WE") == 0) {
        free(data->west_texture); 
        data->west_texture = full_path;
    } else if (ft_strcmp(identifier, "EA") == 0) {
        free(data->east_texture); 
        data->east_texture = full_path;
    } else {
        free(full_path); 
        error_exit(data, "Invalid texture identifier");
    }
}
void parse_color(t_data *data, char *line) 
{
	int *color;
	int i = 0;

	if (ft_strncmp(line, "F ", 2) == 0)
		color = data->floor_color;
	else if (ft_strncmp(line, "C ", 2) == 0)
		color = data->ceiling_color;
	else
		error_exit(data, "Invalid color line");

	char **rgb = ft_split(line + 2, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
		error_exit(data, "Invalid RGB format");

	while(i < 3)
	{
		color[i] = ft_atoi(rgb[i]);
		if (color[i] < 0 || color[i] > 255)
			error_exit(data, "RGB value out of range");
		i++;
	}
	ft_free_tab(rgb);
}

void parse_map(t_data *data, char *line) {
    static int map_started = 0;

    if (!map_started) {
        data->map = malloc(sizeof(char *) * 10);
        if (!data->map)
            error_exit(data, "Failed to allocate memory for map");
        data->map_height = 0;
        data->map_capacity = 10;
        map_started = 1;
    }

    if (data->map_height >= data->map_capacity) {
        data->map_capacity *= 2;
        char **new_map = malloc(sizeof(char *) * data->map_capacity);
        if (!new_map)
            error_exit(data, "Failed to resize map array");
        for (int i = 0; i < data->map_height; i++)
            new_map[i] = data->map[i];
        free(data->map);
        data->map = new_map;
    }

    data->map[data->map_height++] = ft_strdup(line);
}


void parse_cub_file(t_data *data, const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        error_exit(data, "Failed to open file");

    char *line = NULL;
    while ((line = get_next_line(fd))) {
        if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 ||
            ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
            parse_texture(data, line);
        else if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
            parse_color(data, line);
        else if (ft_strchr("01NSEW ", line[0]))
            parse_map(data, line);
        else if (line[0] != '\n')
            error_exit(data, "Invalid line in .cub file");
        free(line);
    }
    free(line);
    close(fd);
}

void free_data(t_data *data) {
    if (data->north_texture)
        free(data->north_texture);
    if (data->south_texture)
        free(data->south_texture);
    if (data->west_texture)
        free(data->west_texture);
    if (data->east_texture)
        free(data->east_texture);

    if (data->map) {
        for (int i = 0; i < data->map_height; i++)
            free(data->map[i]);
        free(data->map);
    }
}

void error_exit(t_data *data, const char *msg) 
{
	fprintf(stderr, "Error: %s\n", msg);
	free_data(data);
	exit(1);
}