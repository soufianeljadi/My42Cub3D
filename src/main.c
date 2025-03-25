#include "../includes/cub3d.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include <string.h>

char **get_map()
{
    char **map = calloc(sizeof(char*) ,11);
    map[0] = strdup("11111111111111111");
    map[1] = strdup("10000000000000001");
    map[2] = strdup("10010001010100001");
    map[3] = strdup("10100000000001001");
    map[4] = strdup("100100100P001001");
    map[5] = strdup("10001000000001001");
    map[6] = strdup("10010100000000001");
    map[7] = strdup("10000101000010001");
    map[8] = strdup("10010000000001001");
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
                player->dir = 0;
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
void dda_line(t_params *params,mlx_image_t  *img,double dest_x, double dest_y)
{
    double dx, dy, steps, x, y;
    float x_inc, y_inc;

    dx = dest_x - params->player.x;
    dy = dest_y - params->player.y;

    steps = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);

    x_inc = (float)dx / steps;
    y_inc = (float)dy / steps;

    x = params->player.x;
    y = params->player.y;
    for (int i = 0; i <= steps; i++){
        if((x > 0 && x < params->width) && (y > 0 && y < params->height) )
            mlx_put_pixel(img, x, y, 0x00ffffff);
        x += x_inc;
        y += y_inc;
    }
}
void dda_line2(t_params *params,mlx_image_t  *img,double dest_x, double dest_y)
{
    double dx, dy, steps, x, y;
    float x_inc, y_inc;

    dx = dest_x - params->player.x;
    dy = dest_y - params->player.y;

    steps = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);

    x_inc = (float)dx / steps;
    y_inc = (float)dy / steps;

    x = params->player.x;
    y = params->player.y;
    for (int i = 0; i <= steps; i++){
        if((x > 0 && x < params->width) && (y > 0 && y < params->height))
        {
            if(params->map[(int)(y / TILE)][(int)(x/TILE)] == '1')
                return;
            mlx_put_pixel(img, x, y, 0x00ffffff);
        }
        x += x_inc;
        y += y_inc;
    }
}
void draw_circle(mlx_image_t *img, mlx_t *mlx,double center_x, double center_y, int radius) {
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
void get_hers_inter(t_params *params, t_ray *ray)
{
    double a_y,a_x;
    double y_step,x_step;
    a_y = floor(params->player.y / TILE) * TILE;
    if(!ray->facing_up)
        a_y+=TILE;
    else
        a_y-=0.01;
    //ax = player_x + (ay - player_y) / tan(ray_angle);
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
  //ray->hor_dis = INT_MAX;
    
}

void get_verts_inter(t_params *params, t_ray *ray)
{
    double a_y,a_x;
    double y_step,x_step;
    a_x = floor(params->player.x / TILE) * TILE;
    if(ray->facing_right)
        a_x+=TILE;
    else
        a_x-=0.01;
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
    //ray->ver_dis = INT_MAX;
    
}
void draw_wall(t_params *params, t_ray ray, mlx_image_t *img, int x)
{
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
            color = 0xff0000;
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
         //if(y > 0 && y < SCREEN_HEIGHT )
        mlx_put_pixel(img, x,y,0x0000ffff);
        y++;
    }
    while(y < wall_end)
    {
         //if(y > 0 && y < SCREEN_HEIGHT )
        mlx_put_pixel(img,x,y,color);
        y++;
    }
    while(y < SCREEN_HEIGHT)
    {
        //if(y > 0 && y < SCREEN_HEIGHT )
            mlx_put_pixel(img,x,y,0x0000ffff);
        y++;
    }

}
void cast_rays(t_params *params, mlx_image_t *img)
{
    int x = 0;
    t_ray ray;

    // ray_angle = player_angle - (FOV / 2) + (x * (FOV / SCREEN_WIDTH));
    
    while(x < SCREEN_WIDTH )
    {
        // if (x == screen_width/2)
        //     printf("%f\n",params->player.dir);
        ray.angle = params->player.dir - (FOV / 2.0) + ((FOV / SCREEN_WIDTH ) * x);
        if(ray.angle<0)
            ray.angle+=2 * M_PI;
        if(ray.angle>2 * M_PI)
            ray.angle-=2 * M_PI;
        ray.facing_up = ray.angle > M_PI && ray.angle < M_PI * 2;
        ray.facing_right = !(ray.angle < 3 * M_PI/2 && ray.angle > M_PI / 2);
        // wall_height = (TILE * SCREEN_HEIGHT) / ray.distance;
        //ray.distance = 1500.0;
        ray.hor_dis = INT_MAX;
        ray.ver_dis = INT_MAX;
        if(ray.angle > 2 * M_PI || ray.angle<0)
            printf("%f\n",ray.angle);
        get_hers_inter(params,&ray);
        get_verts_inter(params,&ray);
    
        ray.distance = fmin(ray.hor_dis,ray.ver_dis);
        ray.distance*=cos(params->player.dir-ray.angle);
         //dda_line(params,img,cos(ray.angle) * fabs(ray.distance) + params->player.x,sin(ray.angle) *fabs(ray.distance) + params->player.y);
        draw_wall(params,ray,img,x);
//        dda_line2(params,img,sin(ray.angle) * 1000.2 + params->player.x,cos(ray.angle) * 1000.2 + params->player.y);
        x++;
    }
}
void cast_player(t_params * params)
{
    t_player player = params->player;
    static mlx_image_t *img;
    if(img)
        mlx_delete_image(params->mlx,img);
    img = mlx_new_image(params->mlx,SCREEN_WIDTH,SCREEN_HEIGHT);
   draw_circle(img,params->mlx,player.x,player.y,10);
   // dda_line2(params,img,cos(params->player.dir) * 1000.2 + params->player.x,sin(params->player.dir) * 1000.2 + params->player.y);
    //printf("%f\n",params->player.dir);
    cast_rays(params,img);
    mlx_image_to_window(params->mlx,img,0,0);
}

void set_to_pos(t_params *params, double x, double y)
{
    // printf("%f %f %f\n",params->player.dir,y,x);
    if(params->map[(int)(y/TILE)][(int)(x/TILE)] != '1')
    {
        params->player.x = x;
        params->player.y = y;
    }
}

// x cos
//y sin


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
        double t_x = params->player.x + cos(params->player.dir) * 3.0;
        double t_y = params->player.y +  sin(params->player.dir) * 3.0;
        set_to_pos(params,t_x,t_y);
    }
    if(mlx_is_key_down(params->mlx,MLX_KEY_S))
    {
        double t_x = params->player.x - cos(params->player.dir) * 3.0;
        double t_y = params->player.y -  sin(params->player.dir) * 3.0;
        set_to_pos(params,t_x,t_y);
    }
    if(mlx_is_key_down(params->mlx,MLX_KEY_D))
    {
    double t_x = params->player.x + cos(params->player.dir + M_PI / 2) * 3.0;
    double t_y = params->player.y + sin(params->player.dir + M_PI / 2) * 3.0;
        set_to_pos(params,t_x,t_y);
    }
    if(mlx_is_key_down(params->mlx,MLX_KEY_A))
    {
        double t_x = params->player.x + cos(params->player.dir - M_PI / 2) * 3.0;
        double t_y = params->player.y +  sin(params->player.dir - M_PI / 2) * 3.0;
        set_to_pos(params,t_x,t_y);
    }
    if(mlx_is_key_down(params->mlx,MLX_KEY_ESCAPE))
        exit(0);
}
void key_hook(void *data)
{
     t_params *params = data;
    if(mlx_is_key_down(params->mlx,MLX_KEY_ESCAPE))
        exit(0);
    player_movement(params);
    cast_player(params);
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
    params.mlx = mlx_init(SCREEN_WIDTH,SCREEN_HEIGHT,"2d map",false);
    params.width = TILE * strlen(*params.map);
    params.height = TILE * 10;
    params.img = mlx_new_image(params.mlx,SCREEN_WIDTH,SCREEN_HEIGHT);
    draw_map(params.map,params.img,params.mlx);
    mlx_loop_hook(params.mlx,key_hook,&params);
    mlx_loop(params.mlx);
    return 0;
}