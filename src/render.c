#include "../cub3d.h"
#include "../minilibx/mlx.h"
#include "../libft/libft.h"
#include <math.h>

int find_side_dist(t_vars *vars, t_ray *ray)
{
	double	mapX;
	double	mapY;

	mapX = vars->player.posX/TILE_SIZE;
	mapY = vars->player.posY/TILE_SIZE;
    ray->side_dist = (double *)malloc(sizeof(double) * 2);
    if (ray->side_dist == NULL)
        return (1);
    if (ray->raydir[X] < 0)
    {
        ray->step[X] = -1;
        ray->side_dist[X] = (mapX - (int)mapX) * ray->delta_dist[X];
    }
    else
        ray->side_dist[X] = ((int)mapX + 1.0 - mapX) * ray->delta_dist[X];
    if (ray->raydir[Y] < 0)
    {
        ray->step[Y] = -1;
        ray->side_dist[Y] = (mapY - (int)mapY) * ray->delta_dist[Y];
    }
    else
        ray->side_dist[Y] = ((int)mapY + 1.0 - mapY) * ray->delta_dist[Y];
	return (0);
}

static int	fill_variable(t_vars *vars, t_ray *ray)
{
    ray->raydir[X] = cos(vars->render.ray_angle);
    ray->raydir[Y] = sin(vars->render.ray_angle);
    ray->delta_dist[X] = fabs(1 / ray->raydir[X]);
    ray->delta_dist[Y] = fabs(1 / ray->raydir[Y]);
    ray->step[X] = 1;
    ray->step[Y] = 1;
    if (find_side_dist(vars, ray))
		return (1);
	return (0);
}

int *check_hit(t_vars *vars, t_ray *ray)
{
    int hit;
    int *map_grid;

	map_grid = (int *)malloc(sizeof(int) * 2);
	if (!map_grid)
		return (NULL);
    hit = 0;
    map_grid[X] = (int)(vars->player.posX/TILE_SIZE);
    map_grid[Y] = (int)(vars->player.posY/TILE_SIZE);
    while (hit == 0)
    {
        if (ray->side_dist[X] < ray->side_dist[Y])
        {
            ray->side_dist[X] += ray->delta_dist[X];
			map_grid[X] += ray->step[X];
        }
		else
        {
            ray->side_dist[Y] += ray->delta_dist[Y];
			map_grid[Y] += ray->step[Y];
        }
		if (vars->map[map_grid[Y]][map_grid[X]] == '1')
			hit = 1;
    }
	return (map_grid);
}

double euclidean_distance(t_vars *vars, t_ray *ray, int *map_grid)
{
	double	dist;
	double	posX;
	double	posY;

	posX = vars->player.posX/TILE_SIZE;
	posY = vars->player.posY/TILE_SIZE;
	if (ray->side_dist[X] < ray->side_dist[Y])
		dist = (map_grid[X] - posX + (1 - ray->step[X]) / 2) / ray->raydir[X];
	else
		dist = (map_grid[Y] - posY + (1 - ray->step[Y]) / 2) / ray->raydir[Y];
	return (dist);
}

void	free_ray_map(t_ray *ray)
{
	null_free(ray->delta_dist);
	null_free(ray->raydir);
	null_free(ray->side_dist);
	null_free(ray->step);
}

double  find_wall_distance(t_vars *vars)
{
    t_ray   ray;
	int		*map_grid;
	double	dist;

    ray.raydir = (double *)malloc(sizeof(double) * 2);
    ray.delta_dist = (double *)malloc(sizeof(double) * 2);
    ray.step = (int *)malloc(sizeof(int) * 2);
    if (!ray.raydir || !ray.step || !ray.delta_dist)
        return (null_free(ray.raydir), null_free(ray.step),
            null_free(ray.delta_dist), -1);
    fill_variable(vars, &ray);
	map_grid = check_hit(vars, &ray);
	if (!map_grid)
		return (-1);
	dist = euclidean_distance(vars, &ray, map_grid);
	free_ray_map(&ray);
	null_free(map_grid);
	return (dist);
}

int	wall_height(t_vars *vars, double dist)
{
	int	wall_height;
	int	t_pix;
	int	b_pix;

	wall_height = (int)(vars->render.sc_height / dist);
	t_pix = -wall_height / 2 + vars->render.sc_height / 2;
	if (t_pix < 0)
		t_pix = 0;
	b_pix = wall_height / 2 + vars->render.sc_height / 2;
	if (b_pix > vars->render.sc_height)
		b_pix = vars->render.sc_height;
	

}

void cast_rays(t_vars *vars)
{
    int ray;
	double	wall_dist;
    int *side_dist;

    vars->render.ray_angle = vars->player.p_angle - (FOV / 2);
    ray = 0;
    while (ray <= vars->render.sc_width)
    {
		wall_dist = find_wall_distance(vars);
		printf("wall distance: %lf\n", wall_dist);
		ray++;
		vars->render.ray_angle += FOV/vars->render.sc_width;
    }
}