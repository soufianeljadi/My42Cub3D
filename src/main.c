#include "../includes/cub3d.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include <string.h>

char **get_map()
{
    char **map = calloc(sizeof(char*) ,11);
    map[0] = strdup("11111111111111111");
    map[1] = strdup("10000000000000001");
    map[2] = strdup("10010001000100001");
    map[3] = strdup("10010000000001001");
    map[4] = strdup("10010001000001001");
    map[5] = strdup("10010000000001001");
    map[6] = strdup("10010000000000001");
    map[7] = strdup("10000001000010001");
    map[8] = strdup("100100000000P1001");
    map[9] = strdup("11111111111111111");
    map[10] = NULL;
    return map;
}
void set_postion(t_player *player, char **map)
{
    int x,y;

    y = 0;
    while(map[y])
    {
        x = 0;
        while(map[y][x])
        {
            if(map[y][x] == 'P')
            {
                player->x = (x* TILE) + TILE / 2;
                player->y = (y * TILE) + TILE / 2;
                player->dir = 3 * (M_PI/ 2);
                return;
            }
            x++;
        }
        y++;
    }
}

void draw_map(char **map,mlx_image_t *img,mlx_t *mlx)
{
    int x,y;
    int width =TILE * strlen(*map);
    int height = TILE * 10;
    y = 0;
    while(y < height)
    { 
        x = 0;
        while(x < width)
        {
            if(map[y/TILE][x /TILE] == '1')
                mlx_put_pixel(img,x,y,0xff0000);
            else
                mlx_put_pixel(img,x,y,0xffffffff);
            x++;
        }
        y++;
    }
    mlx_image_to_window(mlx,img,0,0);
}
// void dda_line(t_params *params)
// {
//     int dx, dy, steps, x, y;
//     float x_inc, y_inc;

//     dx = data->dest_x - data->player_x;
//     dy = data->dest_y - data->player_y;

//     // Determine the number of steps needed to draw the line
//     steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

//     // Calculate the increment for each step
//     x_inc = (float)dx / steps;
//     y_inc = (float)dy / steps;

//     // Start from the player's position
//     x = data->player_x;
//     y = data->player_y;

//     // Draw the line
//     for (int i = 0; i <= steps; i++) {
//         mlx_put_pixel(data, round(x), round(y));
//         x += x_inc;
//         y += y_inc;
//     }
// }
void draw_circle(mlx_image_t *img, mlx_t *mlx,int center_x, int center_y, int radius) {
    int x = radius;
    int y = 0;
    int decision_over2 = 1 - x;  // Midpoint decision parameter

    while (x >= y) {
        // Draw the 8 symmetric points of the circle
        mlx_put_pixel(img, center_x + x, center_y + y, 0x00ff00ff);  // White pixel
        mlx_put_pixel(img, center_x - x, center_y + y, 0x00ff00ff);
        mlx_put_pixel(img, center_x + x, center_y - y, 0x00ff00ff);
        mlx_put_pixel(img, center_x - x, center_y - y, 0x00ff00ff);
        mlx_put_pixel(img, center_x + y, center_y + x, 0x00ff00ff);
        mlx_put_pixel(img, center_x - y, center_y + x, 0x00ff00ff);
        mlx_put_pixel(img, center_x + y, center_y - x, 0x00ff00ff);
        mlx_put_pixel(img, center_x - y, center_y - x, 0x00ff00ff);

        y++;

        if (decision_over2 <= 0) {
            decision_over2 += 2 * y + 1;
        } else {
            x--;
            decision_over2 += 2 * (y - x) + 1;
        }
    }
}
void draw_player(t_params * params)
{
    t_player player = params->player;
    static mlx_image_t *img;
    if(img)
        mlx_delete_image(params->mlx,img);
    img = mlx_new_image(params->mlx,30,30);
    draw_circle(img,params->mlx,15,15,10);
    mlx_image_to_window(params->mlx,img,player.x - 30 /2 ,player.y - 30 / 2);
}

void set_to_pos(t_params *params, double x, double y)
{
    printf("%f %f %f\n",params->player.dir,y,x);
    if(params->map[(int)(y/TILE)][(int)(x/TILE)] != '1')
    {
        params->player.x = x;
        params->player.y = y;
    }
}

// x sin
//y cos


void player_movement(t_params *params)
{
    if(mlx_is_key_down(params->mlx,MLX_KEY_RIGHT))
        params->player.dir +=0.05;
    if(mlx_is_key_down(params->mlx,MLX_KEY_LEFT))
        params->player.dir -=0.05;
    if(params->player.dir < 0)
        params->player.dir+= M_PI * 2;
    if(params->player.dir > 2 * M_PI)
        params->player.dir -=2 * M_PI;
    if(mlx_is_key_down(params->mlx,MLX_KEY_W))
    {
        double t_x = params->player.x + (params->player.dir) * 3.0;
        double t_y = params->player.y + (params->player.dir) * 3.0;
        set_to_pos(params,t_x,t_y);
    }
    if(mlx_is_key_down(params->mlx,MLX_KEY_S))
        set_to_pos(params,params->player.x,params->player.y+10);
    if(mlx_is_key_down(params->mlx,MLX_KEY_A))
        set_to_pos(params,params->player.x-10,params->player.y);
    if(mlx_is_key_down(params->mlx,MLX_KEY_D))
        set_to_pos(params,params->player.x+10,params->player.y);
}
void key_hook(void *data)
{
     t_params *params = data;
    if(mlx_is_key_down(params->mlx,MLX_KEY_ESCAPE))
        exit(0);
    player_movement(params);
    draw_player(params);
}

void set_value(t_data data,t_params *params)
{
    params->data = data;
    
}
int main() 
{
    t_params params;
    params.map = get_map();
    // t_player player;
    set_postion(&params.player,params.map);
    params.mlx = mlx_init(TILE*strlen(*params.map),TILE * 10,"2d map",false);
    params.img = mlx_new_image(params.mlx,TILE*strlen(*params.map),TILE * 10);
    draw_map(params.map,params.img,params.mlx);
    mlx_loop_hook(params.mlx,key_hook,&params);
    mlx_loop(params.mlx);
    return 0;
}