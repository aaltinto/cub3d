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

	map_x = vars->player.pos_x / TILE_SIZE;
	map_y = vars->player.pos_y / TILE_SIZE;
	ray->side_dist = (double *)malloc(sizeof(double) * 2);
	if (ray->side_dist == NULL)
		return (1);
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

int	*check_hit(t_vars *vars, t_ray *ray)
{
	int	*map_grid;

	map_grid = (int *)malloc(sizeof(int) * 2);
	if (!map_grid)
		return (NULL);
	map_grid[X] = (int)(vars->player.pos_x / TILE_SIZE);
	map_grid[Y] = (int)(vars->player.pos_y / TILE_SIZE);
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
	return (map_grid);
}

double	find_wall_distance(t_vars *vars)
{
	t_ray	ray;
	int		*map_grid;
	double	dist;

	ray.raydir = (double *)malloc(sizeof(double) * 2);
	ray.delta_dist = (double *)malloc(sizeof(double) * 2);
	ray.step = (int *)malloc(sizeof(int) * 2);
	if (!ray.raydir || !ray.step || !ray.delta_dist)
		return (null_free(ray.raydir), null_free(ray.step),
			null_free(ray.delta_dist), -1);
	if (fill_variable(vars, &ray) || find_side_dist(vars, &ray))
		return (-1);
	map_grid = check_hit(vars, &ray);
	if (!map_grid)
		return (-1);
	if (vars->render.flag == 0)
		dist = ray.side_dist[X] - ray.delta_dist[X];
	else
		dist = ray.side_dist[Y] - ray.delta_dist[Y];
	free_ray_map(&ray);
	null_free(map_grid);
	return (dist * cos(vars->render.ray_angle - vars->player.p_angle));
}

void	wall_height(t_vars *vars, double dist, int ray)
{
	int	wall_height;
	int	t_pix;
	int	b_pix;

	if (dist == 0.0f)
		dist += 0.000001f;
	wall_height = (int)(vars->render.sc_height / dist);
	t_pix = -wall_height / 2 + vars->render.sc_height / 2;
	if (t_pix < 0)
		t_pix = 0;
	b_pix = wall_height / 2 + vars->render.sc_height / 2;
	if (b_pix > vars->render.sc_height)
		b_pix = vars->render.sc_height;
	while (t_pix <= b_pix)
		pixel_put(&vars->img, ray, t_pix++, get_color(vars, vars->render.flag));
}

int	cast_rays(t_vars *vars)
{
	int		ray;
	double	wall_dist;
	int		*side_dist;

	vars->render.ray_angle = vars->player.p_angle - (FOV / 2);
	ray = 0;
	while (ray < vars->render.sc_width)
	{
		wall_dist = find_wall_distance(vars);
		if (wall_dist < 0)
			return (1);
		if (vars->player.running != 1)
			wall_dist += (vars->player.running/8);
		wall_height(vars, wall_dist, ray);
		ray++;
		vars->render.ray_angle += FOV / vars->render.sc_width;
	}
	return (0);
}
