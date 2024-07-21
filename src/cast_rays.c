/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:05:03 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/12 23:05:04 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include <math.h>
#include <stdlib.h>

int	find_side_dist(t_vars *vars, t_ray *ray)
{
	double	map_x;
	double	map_y;

	map_x = vars->player.camera[X] / TILE_SIZE;
	map_y = vars->player.camera[Y] / TILE_SIZE;
	if (ray->raydir[X] < 0)
	{
		ray->step[X] = -1;
		ray->side_dist[X] = (map_x - (int)map_x) * ray->delta_dist[X];
	}
	else
		ray->side_dist[X] = ((int)map_x + 1.0 - map_x) * ray->delta_dist[X];
	if (ray->raydir[Y] < 0)
	{
		ray->step[Y] = -1;
		ray->side_dist[Y] = (map_y - (int)map_y) * ray->delta_dist[Y];
	}
	else
		ray->side_dist[Y] = ((int)map_y + 1.0 - map_y) * ray->delta_dist[Y];
	return (0);
}

int	check_hit(t_vars *vars, t_ray *ray)
{
	int	map_grid[2];

	map_grid[X] = (int)(vars->player.camera[X] / TILE_SIZE);
	map_grid[Y] = (int)(vars->player.camera[Y] / TILE_SIZE);
	while (vars->map[map_grid[Y]][map_grid[X]] != '1')
	{
		if (ray->side_dist[X] < ray->side_dist[Y])
		{
			ray->side_dist[X] += ray->delta_dist[X];
			map_grid[X] += ray->step[X];
			vars->render.flag = 0;
		}
		else
		{
			ray->side_dist[Y] += ray->delta_dist[Y];
			map_grid[Y] += ray->step[Y];
			vars->render.flag = 1;
		}
	}
	return (0);
}

double	find_wall_distance(t_vars *vars, t_ray *ray)
{
	double	dist;

	if (fill_variable(vars, ray) || find_side_dist(vars, ray))
		return (-1);
	if (check_hit(vars, ray))
		return (-1);
	if (vars->render.flag == 0)
		dist = ray->side_dist[X] - ray->delta_dist[X];
	else
		dist = ray->side_dist[Y] - ray->delta_dist[Y];
	return (((dist) * cos(vars->render.ray_angle - vars->player.p_angle)));
}

int calc_texture(t_vars *vars, t_ray *ray, double dist)
{
	double wall_x;
	int tex_x;
	int tex_width = 64;

	if (vars->render.flag == 0)
		wall_x = vars->player.camera[Y] / TILE_SIZE + dist * ray->raydir[Y];
	else
		wall_x = vars->player.camera[X] / TILE_SIZE + dist * ray->raydir[X];
	wall_x -= floor((wall_x));
	tex_x = (int)(wall_x * (double)tex_width);
	if (vars->render.flag == 0 && ray->raydir[X] > 0)
		tex_x = tex_width - tex_x - 1;
	if (vars->render.flag == 1 && ray->raydir[Y] < 0)
		tex_x = tex_width - tex_x - 1;
	return (tex_x);
}

void wall_height(t_vars *vars, double dist, int ray, t_ray *ray_data)
{
	int wall_height;
	int t_pix;
	int b_pix;
	int tex_x;
	int tex_y;
	double step;
	double tex_pos;
	int tex_height = 64;
	int tex_width = 64;

	if (dist == 0.0f)
		dist += 0.000001f;
	wall_height = (int)(vars->render.sc_height / dist);
	t_pix = -wall_height / 2 + vars->render.sc_height / 2;
	if (t_pix < 0)
		t_pix = 0;
	b_pix = wall_height / 2 + vars->render.sc_height / 2;
	if (b_pix > vars->render.sc_height)
		b_pix = vars->render.sc_height;
	tex_x = calc_texture(vars, ray_data, dist);
	//printf("%i\n", tex_x);
	step = (double)tex_height / wall_height;
	tex_pos = (t_pix - vars->render.sc_height / 2 + wall_height / 2) * step;
	while (t_pix < b_pix)
	{
		tex_y = (int)tex_pos % tex_height;
		tex_pos += step;
		pixel_put(&vars->img, ray, t_pix++, get_color(vars, vars->render.flag, tex_x, tex_y));
	}
}


int	cast_rays(t_vars *vars)
{
	int		ray;
	t_ray	ray_data;
	double	wall_dist;
	int		*side_dist;

	vars->render.ray_angle = vars->player.p_angle - (vars->player.fov / 2);
	ray = 0;
	while (ray < vars->render.sc_width)
	{
		wall_dist = find_wall_distance(vars, &ray_data);
		if (wall_dist < 0)
			return (1);
		wall_height(vars, wall_dist, ray, &ray_data);
		ray++;
		vars->render.ray_angle += vars->player.fov / vars->render.sc_width;
	}
	return (0);
}
