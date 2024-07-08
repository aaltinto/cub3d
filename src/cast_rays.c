#include "../cub3d.h"
#include "../minilibx/mlx.h"
#include "../libft/libft.h"
#include <math.h>
#define TILE_SIZE 30

double	nor_angle(double angle)
{
	if (angle < 0)
		angle += (2 * M_PI);
	if (angle > (2 * M_PI))
		angle -= (2 * M_PI);
	return (angle);
}

int unit_circle(float angle, char c)
{
	if (c == 'x')
	{
		if (angle > 0 && angle < M_PI)
		return (1);
	}
	else if (c == 'y')
	{
		if (angle > (M_PI / 2) && angle < (3 * M_PI) / 2)
		return (1);
	}
	return (0);
}

int inter_check(double angle, double *inter, double *step, int is_horizon) // check the intersection
{
	if (is_horizon)
	{
		if (angle > 0 && angle < M_PI)
		{
			*inter += TILE_SIZE;
			return (-1);
		}
		*step *= -1;
	}
	else
	{
		if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2)) 
		{
			*inter += TILE_SIZE;
			return (-1);
		}
		*step *= -1;
	}
	return (1);
}

int wall_hit(float x, float y, t_vars *vars)
{
	int  x_m;
	int  y_m;

	if (x < 0 || y < 0)
		return (0);
	x_m = floor (x / TILE_SIZE);
	y_m = floor (y / TILE_SIZE);
	if ((y_m >= double_counter(vars->map) || x_m >= ft_strlen(vars->map[0])))
		return (0);
	if (vars->map[y_m] && x_m <= (int)ft_strlen(vars->map[y_m]))
		if (vars->map[y_m][x_m] == '1')
			return (0);
	return (1);
}

double	get_h_inter(t_vars *vars, double angle)
{
	double	h_x;
	double	h_y;
	double	x_step;
	double	y_step;
	int		pixel;

	x_step = TILE_SIZE / tan(angle);
	y_step = TILE_SIZE;
	h_y = floor(vars->player.posY / TILE_SIZE) * TILE_SIZE;
	h_x = vars->player.posX + (h_y - vars->player.posY) / tan(angle);
	pixel = inter_check(angle, &h_y, &y_step, 1);
	if ((unit_circle(angle, 'y') && x_step > 0) || (!unit_circle(angle, 'y') && x_step < 0))
		x_step *= -1;
	while (wall_hit(h_x, h_y - pixel, vars))
	{
		h_x += x_step;
		h_y += y_step;
	}
	return (sqrt(pow(h_x - vars->player.posX, 2) + pow(h_y - vars->player.posY, 2)));
}

double	get_v_inter(t_vars *vars, double angle)
{
	double	v_x;
	double	v_y;
	double	x_step;
	double	y_step;
	int		pixel;

	x_step = TILE_SIZE / tan(angle);
	y_step = TILE_SIZE;
	v_x = floor(vars->player.posX / TILE_SIZE) * TILE_SIZE;
	v_y = vars->player.posY + (v_x - vars->player.posX) / tan(angle);
	pixel = inter_check(angle, &v_x, &x_step, 0);
	if ((unit_circle(angle, 'x') && y_step < 0) || (!unit_circle(angle, 'x') && y_step > 0))
		y_step *= -1;
	while (wall_hit(v_x, v_y - pixel, vars))
	{
		v_x += x_step;
		v_y += y_step;
	}
	return (sqrt(pow(v_x - vars->player.posX, 2) + pow(v_y - vars->player.posY, 2)));
}

int	get_color(t_vars *vars, int flag)	// get the color of the wall
{
	vars->render.ray_angle = nor_angle(vars->render.ray_angle); // normalize the angle
	if (flag == 0)
	{
		if (vars->render.ray_angle > M_PI / 2 && vars->render.ray_angle < 3 * (M_PI / 2))
			return (0xB5B5B5FF); // west wall
		else
			return (0xB5B5B5FF); // east wall
	}
	else
	{
		if (vars->render.ray_angle > 0 && vars->render.ray_angle < M_PI)
			return (0xF5F5F5FF); // south wall
		else
			return (0xF5F5F5FF); // north wall
	}
}
#include <stdio.h>
#include <unistd.h>
void	pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	my_mlx_pixel_put(t_vars *vars, int x, int y, int color)	// put the pixel
{
	if (x < 0) // check the x position
		return ;
	else if (x >= vars->render.sc_width)
		return ;
	if (y < 0) // check the y position
		return ;
	else if (y >= vars->render.sc_height)
		return ;
	pixel_put(&vars->img, x, y, color); // put the pixel
}

void	draw_wall(t_vars *vars, int ray, int t_pix, int b_pix)	// draw the wall
{
	int color;

	color = get_color(vars, vars->render.flag);
	while (t_pix < b_pix)
		my_mlx_pixel_put(vars, ray, t_pix++, color);
}

void	render_wall(t_vars *vars, int ray)	// render the wall
{
	double	wall_h;
	double	b_pix;
	double	t_pix;

	vars->render.ray_dist *= cos(nor_angle(vars->render.ray_angle - vars->player.p_angle)); // fix the fisheye
	wall_h = (TILE_SIZE / vars->render.ray_dist) * ((vars->render.sc_width / 2) / tan(((60 * M_PI) / 180) / 2)); // get the wall height
	b_pix = (vars->render.sc_height / 2) + (wall_h / 2); // get the bottom pixel
	t_pix = (vars->render.sc_height / 2) - (wall_h / 2); // get the top pixel
	if (b_pix > vars->render.sc_height) // check the bottom pixel
		b_pix = vars->render.sc_height;
	if (t_pix < 0) // check the top pixel
		t_pix = 0;
	draw_wall(vars, ray, t_pix, b_pix); // draw the wall
	//draw_floor_ceiling(mlx, ray, t_pix, b_pix); // draw the floor and the ceiling
}

void	cast_rays(t_vars *vars)
{
	double	h_inter;
	double	v_inter;
	int		ray;

	ray = 0;
	vars->render.ray_angle = vars->player.p_angle - ( ((60 * M_PI) / 180) / 2);
	while (ray < vars->render.sc_width)
	{
		vars->render.flag = 0;
		h_inter = get_h_inter(vars, nor_angle(vars->render.ray_angle));
		v_inter = get_v_inter(vars, nor_angle(vars->render.ray_angle));
		if (v_inter <= h_inter)
			vars->render.ray_dist = v_inter;
		else
		{
			vars->render.ray_dist = h_inter;
			vars->render.flag = 1;
		}
		mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
		render_wall(vars, ray);
		ray++;
		vars->render.ray_angle += (((60 * M_PI) / 180) / vars->render.sc_width);
	}
}