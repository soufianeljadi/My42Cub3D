#include "../includes/cub3d.h"

void parse_texture(t_data *data, char *line)
{
    char *path;
    char *identifier;
    char *full_path;
    char *temp;

    identifier = ft_strtok(line, " ");
    if (!identifier)
        error_exit(data, "Invalid texture line: missing identifier");
    path = ft_strtok(NULL, " ");
    if (!path)
        error_exit(data, "Invalid texture line: missing path");
    full_path = ft_strdup(path);
    while ((path = ft_strtok(NULL, " ")))
    {
        temp = ft_strjoin(full_path, " ");
        free(full_path);
        full_path = ft_strjoin(temp, path);
        free(temp);
    }
    if (ft_strcmp(identifier, "NO") == 0)
    {
        free(data->north_texture);
        data->north_texture = full_path;
    }
    else if (ft_strcmp(identifier, "SO") == 0)
    {
        free(data->south_texture);
        data->south_texture = full_path;
    }
    else if (ft_strcmp(identifier, "WE") == 0)
    {
        free(data->west_texture);
        data->west_texture = full_path;
    }
    else if (ft_strcmp(identifier, "EA") == 0)
    {
        free(data->east_texture);
        data->east_texture = full_path;
    }
    else
    {
        free(full_path);
        error_exit(data, "Invalid texture identifier");
    }
}

void parse_color(t_data *data, char *line)
{
    int *color;
    int i;
    char **rgb;

    i = 0;
    if (ft_strncmp(line, "F ", 2) == 0)
        color = data->floor_color;
    else if (ft_strncmp(line, "C ", 2) == 0)
        color = data->ceiling_color;
    else
        error_exit(data, "Invalid color line");
    rgb = ft_split(line + 2, ',');
    if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
        error_exit(data, "Invalid RGB format");
    while (i < 3)
    {
        color[i] = ft_atoi(rgb[i]);
        if (color[i] < 0 || color[i] > 255)
            error_exit(data, "RGB value out of range");
        i++;
    }
    ft_free_tab(rgb);
}

void parse_map(t_data *data, char *line)
{
    static int map_started;
    char *trimmed_line;
    char **new_map;
    int i;

    if (!map_started)
    {
        data->map = malloc(sizeof(char *) * 100);
        if (!data->map)
            error_exit(data, "Failed to allocate memory for map");
        data->map_height = 0;
        data->map_capacity = 10;
        map_started = 1;
    }
    trimmed_line = ft_strtrim(line, " \n");
    if (!trimmed_line)
        error_exit(data, "Failed to trim map line");
    if (data->map_height >= data->map_capacity)
    {
        data->map_capacity *= 2;
        new_map = malloc(sizeof(char *) * data->map_capacity);
        if (!new_map)
            error_exit(data, "Failed to resize map array");
        i = 0;
        while (i < data->map_height)
        {
            new_map[i] = data->map[i];
            i++;
        }
        free(data->map);
        data->map = new_map;
    }
    data->map[data->map_height++] = trimmed_line;
}

void validate_map(t_data *data)
{
    int player_count;
    int i;
    int j;

    player_count = 0;
    i = 0;
    while (i < data->map_height)
    {
        j = 0;
        while (j < (int)ft_strlen(data->map[i]))
        {
            if (ft_strchr("NSEW", data->map[i][j]))
            {
                player_count++;
                data->player_x = j;
                data->player_y = i;
                data->player_dir = data->map[i][j];
            }
            else if (!ft_strchr("01 NSEW", data->map[i][j]))
                error_exit(data, "Invalid character in map");
            j++;
        }
        i++;
    }
    if (player_count != 1)
        error_exit(data, "Map must contain exactly one player spawn point");
    i = 0;
    while (i < data->map_height)
    {
        if (data->map[i][0] != '1' || data->map[i][ft_strlen(data->map[i]) - 1] != '1')
            error_exit(data, "Map must be surrounded by walls");
        i++;
    }
    j = 0;
    while (j < (int)ft_strlen(data->map[0]))
    {
        if (data->map[0][j] != '1' || data->map[data->map_height - 1][j] != '1')
            error_exit(data, "Map must be surrounded by walls");
        j++;
    }
}

int has_cub_extension(const char *filename)
{
    char *dot;

    dot = ft_strrchr(filename, '.');
    if (!dot || dot == filename)
        return 0;
    return (ft_strcmp(dot, ".cub") == 0);
}

void parse_cub_file(t_data *data, const char *filename)
{
    int fd;
    char *line;
    int map_started;

    if (!has_cub_extension(filename))
    {
        fprintf(stderr, "Error: File '%s' does not have a .cub extension\n", filename);
        exit(1);
    }
    fd = open(filename, O_RDONLY);
    if (fd == -1)
        error_exit(data, "Failed to open file");
    line = NULL;
    map_started = 0;
    while ((line = get_next_line(fd)))
    {
        if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 ||
            ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
            parse_texture(data, line);
        else if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
            parse_color(data, line);
        else if (ft_strchr("01NSEW ", line[0]))
        {
            if (!map_started)
                map_started = 1;
            parse_map(data, line);
        }
        else if (line[0] != '\n')
            error_exit(data, "Invalid line in .cub file");
        else if (map_started && line[0] == '\n')
            error_exit(data, "Newline in the middle of the map");
        free(line);
    }
    free(line);
    if (!map_started)
        error_exit(data, "Map not found in .cub file");
    validate_map(data);
    close(fd);
}

void free_data(t_data *data)
{
    int i;

    if (data->north_texture)
        free(data->north_texture);
    if (data->south_texture)
        free(data->south_texture);
    if (data->west_texture)
        free(data->west_texture);
    if (data->east_texture)
        free(data->east_texture);
    if (data->map)
    {
        i = 0;
        while (i < data->map_height)
        {
            free(data->map[i]);
            i++;
        }
        free(data->map);
    }
}

void error_exit(t_data *data, const char *msg)
{
    write(2, "Error:", 6);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    free_data(data);
    exit(1);
}