#include "../includes/cub3d.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include <string.h>

void set_position(t_player *player, char **map, t_data *data)
{
    int x, y;

    y = 0;
    while (y < data->map_height)
    {
        x = 0;
        while (map[y][x])
        {
            if (ft_strchr("NSEW", map[y][x]))
            {
                player->x = (x * TILE) + TILE / 2;
                player->y = (y * TILE) + TILE / 2;
                player->dir = 0;
                if (map[y][x] == 'N')
                    player->dir = 3 * M_PI / 2;
                else if (map[y][x] == 'S')
                    player->dir = M_PI / 2;
                else if (map[y][x] == 'W')
                    player->dir = M_PI;
                return;
            }
            x++;
        }
        y++;
    }
}

void get_hers_inter(t_params *params, t_ray *ray)
{
    double a_y,a_x;
    double y_step,x_step;
    a_y = floor(params->player.y / TILE) * TILE;
    if(!ray->facing_up)
        a_y+=TILE;
    else
        a_y-= 10e-5;
    a_x = (( a_y - params->player.y) / tan(ray->angle)) + params->player.x;
    y_step  = TILE;
    if(ray->facing_up)
        y_step=-y_step;
    x_step = y_step / tan(ray->angle);
    while(a_y > 0 && a_y < params->height && a_x > 0 && a_x < params->width)
    {
        int map_y = a_y / TILE;
        int map_x = a_x / TILE;
        if(params->map[map_y][map_x] == '1')
        {
            ray->hor_dis= sqrt(pow(a_y-params->player.y,2) + pow(a_x-params->player.x,2));
            return;
        }
        a_y+=y_step;
        a_x+=x_step;
    }   
}

void get_verts_inter(t_params *params, t_ray *ray)
{
    double a_y,a_x;
    double y_step,x_step;
    a_x = floor(params->player.x / TILE) * TILE;
    if(ray->facing_right)
        a_x+=TILE;
    else
        a_x-=10e-5;
    a_y = (( a_x - params->player.x) * tan(ray->angle)) + params->player.y;
    x_step  = TILE;
    if(!ray->facing_right)
    x_step=-x_step;
    y_step = x_step * tan(ray->angle);
    while(a_y > 0 && a_y < params->height && a_x > 0 && a_x < params->width)
    {
        int map_y = a_y / TILE;
        int map_x = a_x / TILE;
        if(params->map[map_y][map_x] == '1')
        {
            ray->ver_dis = sqrt(pow(a_y-params->player.y,2) + pow(a_x-params->player.x,2));
            return;
        }
        a_y+=y_step;
        a_x+=x_step;
    }
}
void draw_wall(t_params *params, t_ray ray, mlx_image_t *img, int x)
{
    (void)params;
    int wall_hieght =  TILE *SCREEN_HEIGHT / ray.distance;

    if(wall_hieght < 0)
        wall_hieght = 0;
    wall_hieght = fmin(wall_hieght,SCREEN_HEIGHT);
    int wall_start = SCREEN_HEIGHT / 2 - wall_hieght / 2;
    int wall_end = SCREEN_HEIGHT / 2 + wall_hieght / 2;
    int y = 0;
    int color;
    if(ray.hor_dis < ray.ver_dis)
    {
        if(ray.facing_up)
            color = 0x550070;
        else
            color = 0xff0000/2;
    }
    else
    {
        if(ray.facing_right)
            color = 0x00ffff;
        else
            color = 0x00ffff/2;
    }
    color = color << 8 | 255;

    while(y < wall_start)
    {
        mlx_put_pixel(img, x,y,0x0000ffff);
        y++;
    }
    while(y < wall_end)
    {
        mlx_put_pixel(img,x,y,color);
        y++;
    }
    while(y < SCREEN_HEIGHT)
    {
            mlx_put_pixel(img,x,y,0x0000ffff);
        y++;
    }

}
void cast_rays(t_params *params, mlx_image_t *img)
{
    int x = 0;
    t_ray ray;
 
    while(x < SCREEN_WIDTH )
    {
        ray.angle = params->player.dir - (FOV / 2.0) + ((FOV / SCREEN_WIDTH ) * x);
        if(ray.angle<0)
            ray.angle+=2 * M_PI;
        if(ray.angle>2 * M_PI)
            ray.angle-=2 * M_PI;
        ray.facing_up = ray.angle >= M_PI && ray.angle <= M_PI * 2;
        ray.facing_right = !(ray.angle <= 3 * M_PI/2 && ray.angle >= M_PI / 2);
        ray.hor_dis = INT_MAX;
        ray.ver_dis = INT_MAX;
        if(ray.angle > 2 * M_PI || ray.angle<0)
            printf("%f\n",ray.angle);
        get_hers_inter(params,&ray);
        get_verts_inter(params,&ray);
        ray.distance = fmin(ray.hor_dis,ray.ver_dis);
        ray.distance*=cos(params->player.dir-ray.angle);
        draw_wall(params,ray,img,x);
        x++;
    }
}
void cast_player(t_params *params)
{
    static mlx_image_t *img;
    if(img)
        mlx_delete_image(params->mlx,img);
    img = mlx_new_image(params->mlx,SCREEN_WIDTH,SCREEN_HEIGHT);
    cast_rays(params,img);
    mlx_image_to_window(params->mlx,img,0,0);
}

void set_to_pos(t_params *params, double x, double y)
{
    float   radius = 3.0;
    int     i;
    int     j;

    i = -radius;
    j = i;
    while (i <= radius)
    {
        j = -radius;
        while (j <= radius)
        {
            int map_y = (y + j) / TILE;
            int map_x = (x + i) / TILE;
            if (params->map[map_y][map_x] == '1')
                return ;
            j+=radius;
        }
        i+=radius;
    }
        params->player.x = x;
        params->player.y = y;
}

void player_rotation(t_params *params)
{
    if (mlx_is_key_down(params->mlx, MLX_KEY_RIGHT))
        params->player.dir += 0.05;
    if (mlx_is_key_down(params->mlx, MLX_KEY_LEFT))
        params->player.dir -= 0.05;
    if (params->player.dir < 0)
        params->player.dir += M_PI * 2;
    if (params->player.dir > 2 * M_PI)
        params->player.dir -= 2 * M_PI;
}

void calculate_new_position(t_params *params, double angle_offset, double *t_x, double *t_y)
{
    double move_angle = params->player.dir + angle_offset;
    double move_speed = 3.0;
    
    *t_x = params->player.x + cos(move_angle) * move_speed;
    *t_y = params->player.y + sin(move_angle) * move_speed;
}

void forward_backward(t_params *params)
{
    double t_x, t_y;
    
    if (mlx_is_key_down(params->mlx, MLX_KEY_W))
    {
        calculate_new_position(params, 0, &t_x, &t_y);
        set_to_pos(params, t_x, t_y);
    }
    
    if (mlx_is_key_down(params->mlx, MLX_KEY_S))
    {
        calculate_new_position(params, M_PI, &t_x, &t_y);
        set_to_pos(params, t_x, t_y);
    }
}

void move_sideways(t_params *params)
{
    double t_x, t_y;
    
    if (mlx_is_key_down(params->mlx, MLX_KEY_D))
    {
        calculate_new_position(params, M_PI / 2, &t_x, &t_y);
        set_to_pos(params, t_x, t_y);
    }
    
    if (mlx_is_key_down(params->mlx, MLX_KEY_A))
    {
        calculate_new_position(params, -M_PI / 2, &t_x, &t_y);
        set_to_pos(params, t_x, t_y);
    }
}

void player_movement(t_params *params)
{
    player_rotation(params);
    forward_backward(params);
    move_sideways(params); 
    if (mlx_is_key_down(params->mlx, MLX_KEY_ESCAPE))
        exit(0);
}
void key_hook(void *data)
{
    t_params *params;

    params = data;
    if(mlx_is_key_down(params->mlx,MLX_KEY_ESCAPE))
        exit(0);
    player_movement(params);
    cast_player(params);
}

int main(int ac, char **av) 
{
    (void)ac;
    t_data data = {0};
    t_params params;
    parse_cub_file(&data, av[1]);
    params.map = data.map;
    set_position(&params.player,params.map, &data);
    params.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT,"2d map",false);
    params.width = TILE * strlen(*params.map);
    params.height = TILE * data.map_height;
    params.img = mlx_new_image(params.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    mlx_loop_hook(params.mlx,key_hook,&params);
    mlx_loop(params.mlx);
    return 0;
}