#include "../includes/cub3d.h"

void parse_texture(t_data *data, char *line) 
{
	if (strncmp(line, "NO ", 3) == 0)
		data->north_texture = strdup(line + 3);
	else if (strncmp(line, "SO ", 3) == 0)
		data->south_texture = strdup(line + 3);
	else if (strncmp(line, "WE ", 3) == 0)
		data->west_texture = strdup(line + 3);
	else if (strncmp(line, "EA ", 3) == 0)
		data->east_texture = strdup(line + 3);
	else
		error_exit(data, "Invalid texture line");
}

void parse_color(t_data *data, char *line) 
{
	int *color;
	int i = 0;

	if (strncmp(line, "F ", 2) == 0)
		color = data->floor_color;
	else if (strncmp(line, "C ", 2) == 0)
		color = data->ceiling_color;
	else
		error_exit(data, "Invalid color line");

	char **rgb = ft_split(line + 2, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
		error_exit(data, "Invalid RGB format");

	while(i < 3)
	{
		color[i] = atoi(rgb[i]);
		if (color[i] < 0 || color[i] > 255)
			error_exit(data, "RGB value out of range");
		i++;
	}
	ft_free_tab(rgb);
}

void parse_map(t_data *data, char *line) 
{
	static int map_started = 0;

	if (!map_started) 
	{
		data->map = malloc(sizeof(char *) * 100); 
		data->map_height = 0;
		map_started = 1;
	}
	data->map[data->map_height++] = strdup(line);
}

void parse_cub_file(t_data *data, const char *filename) 
{
	char line[MAX_LINE_LENGTH];

	FILE *file = fopen(filename, "r");
	if (!file)
		error_exit(data, "Failed to open file");

	while (fgets(line, MAX_LINE_LENGTH, file)) 
	{
		if (strncmp(line, "NO ", 3) == 0 || strncmp(line, "SO ", 3) == 0 ||
			strncmp(line, "WE ", 3) == 0 || strncmp(line, "EA ", 3) == 0)
			parse_texture(data, line);
		else if (strncmp(line, "F ", 2) == 0 || strncmp(line, "C ", 2) == 0)
			parse_color(data, line);
		else if (strchr("01NSEW ", line[0]))
			parse_map(data, line);
		else if (line[0] != '\n')
			error_exit(data, "Invalid line in .cub file");
	}
	fclose(file);
}

void free_data(t_data *data) 
{
	int i;

	i = 0;
	free(data->north_texture);
	free(data->south_texture);
	free(data->west_texture);
	free(data->east_texture);
	while(i < data->map_height)
	{
		free(data->map[i]);
		i++;
	}
	free(data->map);
}

void error_exit(t_data *data, const char *msg) 
{
	fprintf(stderr, "Error: %s\n", msg);
	free_data(data);
	exit(1);
}