#include "../includes/cub3d.h"

void parse_texture(t_data *data, char *line)
{
    char *path;
    char *identifier;
    int fd;

    identifier = ft_strtok(line, " ");
    if (!identifier)
        error_exit(data, "Invalid texture line: missing identifier");

    path = ft_strtok(NULL, " ");
    if (!path)
        error_exit(data, "Invalid texture line: missing path");

    char *full_path = ft_strdup(path);
    while ((path = ft_strtok(NULL, " ")))
    {
        char *temp = ft_strjoin(full_path, " ");
        free(full_path);
        full_path = ft_strjoin(temp, path);
        free(temp);
    }

    char *trimmed_path = ft_strtrim(full_path, " ");
    if (!trimmed_path)
        error_exit(data, "Failed to trim texture path");
    free(full_path);
    full_path =  ft_strtrim(trimmed_path,"\n");
    if (!full_path)
        error_exit(data, "Failed to trim texture path");
    free(trimmed_path);
    trimmed_path = full_path;
    if (ft_strchr(trimmed_path, ' '))
    {
        free(trimmed_path);
        error_exit(data, "Texture path contains spaces");
    }

    if(ft_strcmp(ft_strrchr(trimmed_path, '.'),".xpm") != 0
     || trimmed_path[ft_strlen(trimmed_path) - 5] == '/')
    {
        free(trimmed_path);
        error_exit(data, "Invalid texture file extension");
    }
    fd = open(trimmed_path, O_RDONLY);
    if (fd == -1)
    {
        free(trimmed_path);
        error_exit(data, "Failed to open texture file");
    }
    close(fd);

    if (ft_strcmp(identifier, "NO") == 0)
    {
        if (data->has_north_texture)
            error_exit(data, "Duplicate north texture");
        free(data->north_texture);
        data->north_texture = trimmed_path;
        data->has_north_texture = 1;
    }
    else if (ft_strcmp(identifier, "SO") == 0)
    {
        if (data->has_south_texture)
            error_exit(data, "Duplicate south texture");
        free(data->south_texture);
        data->south_texture = trimmed_path;
        data->has_south_texture = 1;
    }
    else if (ft_strcmp(identifier, "WE") == 0)
    {
        if (data->has_west_texture)
            error_exit(data, "Duplicate west texture");
        free(data->west_texture);
        data->west_texture = trimmed_path;
        data->has_west_texture = 1;
    }
    else if (ft_strcmp(identifier, "EA") == 0)
    {
        if (data->has_east_texture)
            error_exit(data, "Duplicate east texture");
        free(data->east_texture);
        data->east_texture = trimmed_path;
        data->has_east_texture = 1;
    }
    else
    {
        free(trimmed_path);
        error_exit(data, "Invalid texture identifier");
    }
}

int check_commas(char *str)
{
	int i = 0;
	int commas = 0;
	while (str[i])
	{
		if (str[i] == ',')
			commas++;
		i++;
	}
	if (commas != 2)
		return (1);
	return (0);
}

void parse_color(t_data *data, char *line)
{
    int *color;
    int i;
    char **rgb= NULL;
    char *ptr = NULL;

    if (ft_strncmp(line, "C ", 2) == 0)
    {
        if (data->has_ceiling_color)
            error_exit(data, "Duplicate ceiling color");
        color = data->ceiling_color;
        data->has_ceiling_color = 1;
        ptr = line + 2;
    }
    else if (ft_strncmp(line, "F ", 2) == 0)
    {
        if (data->has_floor_color)
            error_exit(data, "Duplicate floor color");
        color = data->floor_color;
        data->has_floor_color = 1;
        ptr = line + 2;
    }
    else
        error_exit(data, "Invalid color line");

    while (*ptr == ' ')
        ptr++;

    if (check_commas(ptr))
        error_exit(data, "Invalid RGB format");

    rgb = ft_split(ptr, ',');
    if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
    {
        ft_free_tab(rgb);
        error_exit(data, "Invalid RGB format");
    }

    i = 0;
    while (i < 3)
    {
        if (!rgb[i] || !*rgb[i])
        {
            ft_free_tab(rgb);
            error_exit(data, "Missing RGB value");
        }

        if(ft_strlen(rgb[i]) > 3)
        {
            if ((rgb[i][0] && !ft_isdigit(rgb[i][0]) )||
            (rgb[i][1] && !ft_isdigit((char)rgb[i][1])) ||
            (rgb[i][2] && !ft_isdigit((char)rgb[i][2])))
            {
                ft_free_tab(rgb);
                error_exit(data, "Invalid RGB value");
            }
        }
        else if(ft_strlen(rgb[i]) > 2)
        {
            if ((rgb[i][0] && !ft_isdigit(rgb[i][0]) )||
            (rgb[i][1] && !ft_isdigit((char)rgb[i][1])))
            {
                ft_free_tab(rgb);
                error_exit(data, "Invalid RGB value");
            }
        }
        else if(ft_strlen(rgb[i]) > 1)
        {
            if ((rgb[i][0] && !ft_isdigit(rgb[i][0])))
            {
                ft_free_tab(rgb);
                error_exit(data, "Invalid RGB value");
            }
        }

        int value = ft_atoi(rgb[i]);
        if (value < 0 || value > 255)
        {
            ft_free_tab(rgb);
            error_exit(data, "RGB value out of range");
        }
        color[i] = value;
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

    trimmed_line = ft_strtrim(line, "\n");
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
    int player_count = 0;
    int i = 0;
    int j;

    size_t max_len = 0;
    while (i < data->map_height)
    {
        size_t len = ft_strlen(data->map[i]);
        if (len > max_len)
            max_len = len;
        i++;
    }

    i = 0;
    while (i < data->map_height)
    {
        size_t len = ft_strlen(data->map[i]);
        if (len < max_len)
        {
            char *padded_line = malloc((max_len + 1) * sizeof(char));
            if (!padded_line)
                error_exit(data, "Failed to allocate memory for padded line");

            ft_strlcpy(padded_line, data->map[i], len + 1);
            while (len < max_len)
            {
                padded_line[len] = ' ';
                len++;
            }
            padded_line[len] = '\0';

            free(data->map[i]);
            data->map[i] = padded_line;
        }
        i++;
    }

    i = 0;
    while (i < data->map_height)
    {
        j = 0;
        while (j < (int)ft_strlen(data->map[i]))
        {
            char c = data->map[i][j];
            if (ft_strchr("NSEW", c))
            {
                player_count++;
                data->player_x = j;
                data->player_y = i;
                data->player_dir = c;
            }
            else if (!ft_strchr("01 NSEW", c))
                error_exit(data, "Invalid character in map");
            j++;
        }
        i++;
    }

    if (player_count != 1)
        error_exit(data, "Map must contain exactly one player spawn point");

    i = 0;
    while (i < (int)ft_strlen(data->map[0]))
    {
        if (data->map[0][i] != '1' && data->map[0][i] != ' ')
            error_exit(data, "First line must contain only walls and spaces");
        i++;
    }
    i = 0;
    while (i < (int)ft_strlen(data->map[data->map_height - 1]))
    {
        if (data->map[data->map_height - 1][i] != '1' && data->map[data->map_height - 1][i] != ' ')
            error_exit(data, "Last line must contain only walls and spaces");
        i++;
    }

    i = 1;
    while (i < data->map_height - 1)
    {
        int start = 0;
        int end = ft_strlen(data->map[i]) - 1;

        while (data->map[i][start] == ' ')
            start++;

        while (data->map[i][end] == ' ')
            end--;

        if (data->map[i][start] != '1' || data->map[i][end] != '1')
            error_exit(data, "Map must be surrounded by walls");

        j = start;
        while (j <= end)
        {
            if (data->map[i][j] == '0')
            {
                if ((j > start && data->map[i][j - 1] == ' ') || data->map[i][j - 1] == '\0')
                    error_exit(data, "Empty space adjacent to space in the same line");

                if ((j < end && data->map[i][j + 1] == ' ') || data->map[i][j + 1] == '\0')
                    error_exit(data, "Empty space adjacent to space in the same line");

                if ((i > 0 && data->map[i - 1][j] == ' ' )|| data->map[i - 1][j] == '\0')
                    error_exit(data, "Empty space adjacent to space in the line above");

                if ((i < data->map_height - 1 && data->map[i + 1][j] == ' ' )|| data->map[i + 1][j] == '\0')
                    error_exit(data, "Empty space adjacent to space in the line below");
            }
            j++;
        }
        i++;
    }
}
int has_cub_extension(const char *filename)
{
    char *dot;

    dot = ft_strrchr(filename, '.');
    if (!dot || dot == filename)
        return 0;
    if (filename[ft_strlen(filename) - 5] == '/')
        return 0;
    return (ft_strcmp(dot, ".cub") == 0);
}

void check_textures(t_data *data)
{
    if (!data->has_north_texture)
        error_exit(data, "Missing north texture");
    if (!data->has_south_texture)
        error_exit(data, "Missing south texture");
    if (!data->has_west_texture)
        error_exit(data, "Missing west texture");
    if (!data->has_east_texture)
        error_exit(data, "Missing east texture");
}

void check_colors(t_data *data)
{
    if (!data->has_floor_color)
        error_exit(data, "Missing floor color");
    if (!data->has_ceiling_color)
        error_exit(data, "Missing ceiling color");
}
void parse_cub_file(t_data *data, const char *filename)
{
    if (!has_cub_extension(filename))
    {
        write(2, "Error: File ", 12);
        write(2, filename, ft_strlen(filename));
        write(2, " does not have a .cub extension\n", 32);
        exit(1);
    }

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        error_exit(data, "Failed to open file");

    char *line = NULL;
    int map_started = 0;

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

    check_textures(data);
    check_colors(data);
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
    write(2, "Error: ", 7);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);
    free_data(data);
    exit(1);
}