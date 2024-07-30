/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_up.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 06:54:06 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/23 06:54:07 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/bonus.h"
#include "../../minilibx/mlx.h"

static int	get_images(t_vars *vars)
{
	t_data	*xpm;

	xpm = vars->xpm;
	xpm[NO].addr = mlx_get_data_addr(xpm[NO].img, &(xpm[NO].bits_per_pixel),
			&(xpm[NO].line_length), &(xpm[NO].endian));
	if (!xpm[NO].addr)
		return (err("Get data addr error: 'NO'"));
	xpm[SO].addr = mlx_get_data_addr(xpm[SO].img, &(xpm[SO].bits_per_pixel),
			&(xpm[SO].line_length), &(xpm[SO].endian));
	if (!xpm[SO].addr)
		return (err("Get data addr error: 'SO'"));
	xpm[WE].addr = mlx_get_data_addr(xpm[WE].img, &xpm[WE].bits_per_pixel,
			&xpm[WE].line_length, &xpm[WE].endian);
	if (!xpm[WE].addr)
		return (err("Get data addr error: 'WE'"));
	xpm[EA].addr = mlx_get_data_addr(xpm[EA].img, &xpm[EA].bits_per_pixel,
			&xpm[EA].line_length, &xpm[EA].endian);
	if (!xpm[EA].addr)
		return (err("Get data addr error: 'EA'"));
	vars->map_arrow.addr = mlx_get_data_addr(vars->map_arrow.img, \
	&vars->map_arrow.bits_per_pixel, &vars->map_arrow.line_length, \
	&vars->map_arrow.endian);
	if (!vars->map_arrow.addr)
		return (err("Get data addr error: map arrow"));
	return (0);
}

static int	wall_err(int i)
{
	if (i == NO)
		return (err("can't get texture 'NO'"));
	if (i == SO)
		return (err("can't get texture 'SO'"));
	if (i == WE)
		return (err("can't get texture 'WE'"));
	if (i == EA)
		return (err("can't get texture 'EA'"));
	return (0);
}

int	get_textures(t_vars *vars)
{
	int	x;
	int	y;
	int	i;

	x = 64;
	y = 64;
	i = -1;
	while (++i < 4)
	{
		vars->xpm[i].img = mlx_xpm_file_to_image(vars->mlx.mlx, \
			strip(vars->textures.walls[i]), &x, &y);
		if (!vars->xpm[i].img)
			return (wall_err(i));
	}
	x = 48;
	y = 48;
	vars->map_arrow.img = mlx_xpm_file_to_image(vars->mlx.mlx,
			"./textures/map_arrow.xpm", &x, &y);
	if (!vars->map_arrow.img)
		return (err("can't get texture map arrow"));
	return (get_images(vars));
}

static void	assign_player(t_vars *vars, int dir)
{
	if (dir == 0)
	{
		vars->player.p_angle = (3 * M_PI) / 2;
		vars->player.camera[X] = vars->player.pos[X];
		vars->player.camera[Y] = vars->player.pos[Y] - 1;
	}
	else if (dir == 1)
	{
		vars->player.p_angle = 0;
		vars->player.camera[X] = vars->player.pos[X] - 1;
		vars->player.camera[Y] = vars->player.pos[Y];
	}
	else if (dir == 2)
	{
		vars->player.p_angle = M_PI / 2;
		vars->player.camera[X] = vars->player.pos[X];
		vars->player.camera[Y] = vars->player.pos[Y] + 1;
	}
	else if (dir == 3)
	{
		vars->player.p_angle = M_PI;
		vars->player.camera[X] = vars->player.pos[X] + 1;
		vars->player.camera[Y] = vars->player.pos[Y];
	}
}
#include <math.h>
int	detect_player(t_vars *vars)
{
	int	x;
	int	y;

	y = -1;
	while (vars->map[++y] != (void *)0)
	{
		x = -1;
		while (vars->map[y][++x])
		{
			vars->player.pos[X] = (x * TILE_SIZE) + TILE_SIZE / 2;
			vars->player.pos[Y] = (y * TILE_SIZE) + TILE_SIZE / 2;
			if (vars->map[y][x] == 'N')
				return (assign_player(vars, 0), 0);
			else if (vars->map[y][x] == 'E')
				return (assign_player(vars, 1), 0);
			else if (vars->map[y][x] == 'S')
				return (assign_player(vars, 2), 0);
			else if (vars->map[y][x] == 'W')
				return (assign_player(vars, 3), 0);
		}
	}
	vars->player.dir[X] = cos(vars->player.p_angle);
	vars->player.dir[Y] = sin(vars->player.p_angle);
	return (1);
}
	// double odx = vars->player.dir[X];
	// vars->player.dir[X] = vars->player.dir[X] * cos((mx - x) * 0.005) - vars->player.dir[Y] * sin(-((mx - x) * 0.005));
	// vars->player.dir[Y] = odx * sin(-(mx - x) * 0.005) + vars->player.dir[Y] * cos((mx - x) * 0.005);
	// double opx = vars->player.plane[X];
	// vars->player.plane[X] = vars->player.plane[X] * cos((mx - x) * 0.005) - vars->player.plane[Y] * sin((mx - x) * 0.005);
	// vars->player.plane[Y] = opx * sin((mx - x) * 0.005) + vars->player.plane[Y] * cos((mx - x) * 0.005);
	// printf("px:%lf, py:%lf\n", vars->player.plane[X], vars->player.plane[Y]);