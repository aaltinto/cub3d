#include "../cub3d.h"
#include "../minilibx/mlx.h"
#include "../libft/libft.h"
#include <math.h>

double nor_angle(double angle) {
    if (angle < 0)
        angle += (2 * M_PI);
    if (angle > (2 * M_PI))
        angle -= (2 * M_PI);
    return angle;
}

int unit_circle(double angle, char c) {
    if (c == 'x') {
        if (angle > 0 && angle < M_PI)
            return 1;
    } else if (c == 'y') {
        if (angle > (M_PI / 2) && angle < (3 * M_PI) / 2)
            return 1;
    }
    return 0;
}

int inter_check(double angle, double *inter, double *step, int is_horizon) {
    if (is_horizon) {
        if (angle > 0 && angle < M_PI) {
            *inter += TILE_SIZE;
            return -1;
        }
        *step *= -1;
    } else {
        if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2)) {
            *inter += TILE_SIZE;
            return -1;
        }
        *step *= -1;
    }
    return 1;
}

int wall_hit(double x, double y, t_vars *vars) {
    int x_m, y_m;

    if (x < 0 || y < 0)
        return 0;
    x_m = floor(x / TILE_SIZE);
    y_m = floor(y / TILE_SIZE);
    if ((y_m >= double_counter(vars->map) || x_m >= find_longest_line(vars->map)))
        return 0;
    if (vars->map[y_m] && x_m <= (int)ft_strlen(vars->map[y_m]))
        if (vars->map[y_m][x_m] == '1')
            return 0;
    return 1;
}

double get_h_inter(t_vars *vars, double angle) {
    double h_x, h_y, x_step, y_step;
    int pixel;

    x_step = TILE_SIZE / tan(angle);
    y_step = TILE_SIZE;
    h_y = floor(vars->player.posY / TILE_SIZE) * TILE_SIZE;
    h_x = vars->player.posX + (h_y - vars->player.posY) / tan(angle);
    pixel = inter_check(angle, &h_y, &y_step, 1);
    if ((unit_circle(angle, 'y') && x_step > 0) || (!unit_circle(angle, 'y') && x_step < 0))
        x_step *= -1;
    while (wall_hit(h_x, h_y - pixel, vars)) {
        h_x += x_step;
        h_y += y_step;
    }
    return sqrt(pow(h_x - vars->player.posX, 2) + pow(h_y - vars->player.posY, 2));
}

double get_v_inter(t_vars *vars, double angle) {
    double v_x, v_y, x_step, y_step;
    int pixel;

    x_step = TILE_SIZE;
    y_step = TILE_SIZE * tan(angle);
    v_x = floor(vars->player.posX / TILE_SIZE) * TILE_SIZE;
    pixel = inter_check(angle, &v_x, &x_step, 0);
    v_y = vars->player.posY + (v_x - vars->player.posX) * tan(angle);
    if ((unit_circle(angle, 'x') && y_step < 0) || (!unit_circle(angle, 'x') && y_step > 0))
        y_step *= -1;
    while (wall_hit(v_x - pixel, v_y, vars)) {
        v_x += x_step;
        v_y += y_step;
    }
    return sqrt(pow(v_x - vars->player.posX, 2) + pow(v_y - vars->player.posY, 2));
}

void pixel_put(t_data *data, int x, int y, int color) {
    char *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void my_mlx_pixel_put(t_vars *vars, int x, int y, int color) {
    if (x < 0 || x >= vars->render.sc_width || y < 0 || y >= vars->render.sc_height)
        return;
    pixel_put(&vars->img, x, y, color);
}

int get_color(t_vars *vars, int flag) {
    vars->render.ray_angle = nor_angle(vars->render.ray_angle);
    if (flag == 0) {
        if (vars->render.ray_angle > M_PI / 2 && vars->render.ray_angle < 3 * M_PI / 2)
            return rgb_to_hex(255, 0, 0); // west wall
        else
            return rgb_to_hex(0, 255, 0); // east wall
    } else {
        if (vars->render.ray_angle > 0 && vars->render.ray_angle < M_PI)
            return rgb_to_hex(0, 0, 255); // south wall
        else
            return rgb_to_hex(100, 100, 100); // north wall
    }
}

void draw_wall(t_vars *vars, int ray, int t_pix, int b_pix) {
    int color;

    color = get_color(vars, vars->render.flag);
    while (t_pix < b_pix)
        my_mlx_pixel_put(vars, ray, t_pix++, color);
}

void render_wall(t_vars *vars, int ray) {
    double wall_h, b_pix, t_pix;

    //vars->render.ray_dist *= cos(nor_angle(vars->render.ray_angle - vars->player.p_angle)); // fix fisheye effect
    wall_h = (TILE_SIZE / vars->render.ray_dist) * ((vars->render.sc_width / 2) / tan(FOV / 2));
    b_pix = (vars->render.sc_height / 2) + (wall_h / 2);
    t_pix = (vars->render.sc_height / 2) - (wall_h / 2);
    if (b_pix > vars->render.sc_height)
        b_pix = vars->render.sc_height;
    if (t_pix < 0)
        t_pix = 0;
    draw_wall(vars, ray, t_pix, b_pix);
}

void cast_rays(t_vars *vars) {
    double h_inter, v_inter;
    int ray;

    ray = 0;
    vars->render.ray_angle = vars->player.p_angle - (FOV / 2);
    mlx_clear_window(vars->mlx.mlx, vars->mlx.win); // Clear the window once before the loop
    while (ray < vars->render.sc_width) {
        vars->render.flag = 0;
        h_inter = get_h_inter(vars, nor_angle(vars->render.ray_angle));
        v_inter = get_v_inter(vars, nor_angle(vars->render.ray_angle));
        if (v_inter <= h_inter)
            vars->render.ray_dist = v_inter;
        else {
            vars->render.ray_dist = h_inter;
            vars->render.flag = 1;
        }
        render_wall(vars, ray);
        ray++;
        vars->render.ray_angle += (FOV / vars->render.sc_width);
    }
}