/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:58:39 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/13 01:58:29 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../cub3d.h"
#include "../libft/libft.h"
#include "../minilibx/mlx.h"
#include <unistd.h>
#include <math.h>

static int	get_images(t_vars *vars)
{
	vars->xpm[NO].addr = mlx_get_data_addr(vars->xpm[NO].img, \
		&(vars->xpm[NO].bits_per_pixel), &(vars->xpm[NO].line_length), \
		&(vars->xpm[NO].endian));
	if (!vars->xpm[NO].addr)
		return (err("Get data addr error: 'NO'"));
	vars->xpm[SO].addr = mlx_get_data_addr(vars->xpm[SO].img, \
		&(vars->xpm[SO].bits_per_pixel), &(vars->xpm[SO].line_length), \
		&(vars->xpm[SO].endian));
	if (!vars->xpm[SO].addr)
		return (err("Get data addr error: 'SO'"));
	vars->xpm[WE].addr = mlx_get_data_addr(vars->xpm[WE].img, \
		&vars->xpm[WE].bits_per_pixel, &vars->xpm[WE].line_length, \
		&vars->xpm[WE].endian);
	if (!vars->xpm[WE].addr)
		return (err("Get data addr error: 'WE'"));
	vars->xpm[EA].addr = mlx_get_data_addr(vars->xpm[EA].img, \
		&vars->xpm[EA].bits_per_pixel, &vars->xpm[EA].line_length, \
		&vars->xpm[EA].endian);
	if (!vars->xpm[EA].addr)
		return (err("Get data addr error: 'EA'"));
	return (0);
}

int	get_textures(t_vars *vars)
{
	int	x;
	int	y;

	x = 64;
	y = 64;
	vars->xpm = (t_data *)malloc(sizeof(t_data) * 4);
	if (!vars->xpm)
		return (err("Malloc error"), 1);
	vars->xpm[NO].img = mlx_xpm_file_to_image(vars->mlx.mlx, \
		strip(vars->textures.north), &x, &y);
	if (!vars->xpm[NO].img)
		return (err("can't get texture 'NO'"));
	vars->xpm[SO].img = mlx_xpm_file_to_image(vars->mlx.mlx, \
		strip(vars->textures.south), &x, &y);
	if (!vars->xpm[SO].img)
		return (err("can't get texture 'SO'"));
	vars->xpm[WE].img = mlx_xpm_file_to_image(vars->mlx.mlx, \
		strip(vars->textures.west), &x, &y);
	if (!vars->xpm[WE].img)
		return (err("can't get texture 'WE'"));
	vars->xpm[EA].img = mlx_xpm_file_to_image(vars->mlx.mlx, \
		strip(vars->textures.east), &x, &y);
	if (!vars->xpm[EA].img)
		return (err("can't get texture 'EA'"));
	return (get_images(vars));
}

int	marche(t_vars *vars)
{
	vars->map = NULL;
	vars->textures.ceiling = NULL;
	vars->textures.floor = NULL;
	vars->textures.north = NULL;
	vars->textures.south = NULL;
	vars->textures.east = NULL;
	vars->textures.west = NULL;
	vars->mlx.mlx = NULL;
	vars->mlx.win = NULL;
	vars->render.sc_height = 600;
	vars->render.sc_width = 840;
	vars->render.flag = 0;
	vars->keys.key_a = 0;
	vars->keys.key_s = 0;
	vars->keys.key_d = 0;
	vars->keys.key_w = 0;
	vars->keys.key_la = 0;
	vars->keys.key_ra = 0;
	vars->player.running = 1;
	vars->fov_angle = 66;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	vars->textures.ceiling_rgb = (int *)malloc(sizeof(int) * 3);
	vars->textures.floor_rgb = (int *)malloc(sizeof(int) * 3);
	if (!vars->textures.ceiling_rgb || !vars->textures.floor_rgb)
		return (null_free(vars->textures.ceiling_rgb),
			null_free(vars->textures.floor_rgb), err("Malloc error"));
	return (0);
}

int	detect_player(t_vars *vars)
{
	int	x;
	int	y;

	y = -1;
	while (vars->map[++y] != NULL)
	{
		x = -1;
		while (vars->map[y][++x])
		{
			vars->player.pos_x = (x * TILE_SIZE) + TILE_SIZE / 2;
			vars->player.pos_y = (y * TILE_SIZE) + TILE_SIZE / 2;
			if (vars->map[y][x] == 'N')
				return (vars->player.p_angle = (3 * M_PI) / 2, 0);
			else if (vars->map[y][x] == 'E')
				return (vars->player.p_angle = 0, 0);
			else if (vars->map[y][x] == 'S')
				return (vars->player.p_angle = M_PI / 2, 0);
			else if (vars->map[y][x] == 'W')
				return (vars->player.p_angle = M_PI, 0);
		}
	}
	return (1);
}

int	main(int ac, char **argv)
{
	t_vars	vars;

	if (ac != 2)
		return (err(ARG));
	if (marche(&vars))
		return (abort_mission(&vars), 1);
	if (read_map(argv, &vars))
		return (abort_mission(&vars), 1);
	vars.mlx.mlx = mlx_init();
	if (!vars.mlx.mlx)
		return (err("Mlx init error"));
	vars.mlx.win = mlx_new_window(vars.mlx.mlx, vars.render.sc_width,
			vars.render.sc_height, "cub3d");
	if (!vars.mlx.win)
		return (err("Mlx window error"), close_windows(&vars), 1);
	if (get_textures(&vars))
		return (close_windows(&vars), 1);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_hook(vars.mlx.win, 03, 0, key_release, &vars);
	mlx_loop_hook(vars.mlx.mlx, render, (void *)(&vars));
	mlx_loop(vars.mlx.mlx);
	return (close_windows(&vars), 0);
}
