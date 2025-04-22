#include "../includes/cub3d.h"

int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <map.cub>\n", argv[0]);
        return 1;
    }

    t_data data = {0};
    parse_cub_file(&data, argv[1]);

    printf("North Texture: %s\n", data.north_texture);
    printf("South Texture: %s\n", data.south_texture);
    printf("West Texture: %s\n", data.west_texture);
    printf("East Texture: %s\n", data.east_texture);
    printf("Floor Color: %d,%d,%d\n", data.floor_color[0], data.floor_color[1], data.floor_color[2]);
    printf("Ceiling Color: %d,%d,%d\n", data.ceiling_color[0], data.ceiling_color[1], data.ceiling_color[2]);
    printf("Map:\n");
    for (int i = 0; i < data.map_height; i++)
        printf("%s\n", data.map[i]);

    free_data(&data);
    return 0;
}