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
#include "../../includes/bonus.h"
#include "../../libft/libft.h"
#include "../../minilibx/mlx.h"
#include <unistd.h>
#include <math.h>

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

int	wall_err(int i)
{
	if (i == NO)
		return (err("can't get texture 'NO'"));
	if (i == SO)
		return (err("can't get texture 'SO'"));
	if (i == WE)
		return (err("can't get texture 'WE'"));
	if (i == EA)
		return (err("can't get texture 'EA'"));
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

int	marche(t_vars *vars)
{
	int	i;

	vars->map = NULL;
	vars->textures.ceiling = NULL;
	vars->textures.floor = NULL;
	i = -1;
	while (++i < 5)
		vars->textures.walls[i] = NULL;
	vars->player.shoot = 0;
	vars->gun_name = malloc(sizeof(char *) * 4);
	if (!vars->gun_name)
		return(err("Malloc error"));
	vars->gun_name[0] = ft_strdup("./xpm/Hunter");
	vars->gun_name[1] = ft_strdup("./xpm/Talon");
	vars->gun_name[2] = ft_strdup("./xpm/Therm");
	vars->gun_name[3] = NULL;
	vars->player.gun_type = 0;
	vars->ammo = 10;
	vars->player.ani_i = 0;
	vars->mlx.mlx = NULL;
	vars->mlx.win = NULL;
	vars->render.sc_height = 900;
	vars->render.sc_width = 1090;
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
			vars->player.pos[X] = (x * TILE_SIZE) + TILE_SIZE / 2;
			vars->player.pos[Y] = (y * TILE_SIZE) + TILE_SIZE / 2;
			if (vars->map[y][x] == 'N')
				return (vars->player.p_angle = (3 * M_PI) / 2, \
	vars->player.camera[X] = vars->player.pos[X], vars->player.camera[Y] \
	= vars->player.pos[Y] - 1, 0);
			else if (vars->map[y][x] == 'E')
				return (vars->player.p_angle = 0, vars->player.camera[X] = \
	vars->player.pos[X] - 1, vars->player.camera[Y] = vars->player.pos[Y], 0);
			else if (vars->map[y][x] == 'S')
				return (vars->player.p_angle = M_PI / 2, vars->player.camera[X] \
	= vars->player.pos[X], vars->player.camera[Y] = vars->player.pos[Y] + 1, 0);
			else if (vars->map[y][x] == 'W')
				return (vars->player.p_angle = M_PI, vars->player.camera[X] = \
	vars->player.pos[X] + 1, vars->player.camera[Y] = vars->player.pos[Y], 0);
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
	if (get_textures(&vars) || get_magnum_sprites(&vars, 64 * TILE_SIZE, 64 * TILE_SIZE)
		|| get_num_sprites(&vars, 7, 10))
		return (close_windows(&vars), 1);
	mlx_mouse_hide();
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_hook(vars.mlx.win, 03, 0, key_release, &vars);
	mlx_hook(vars.mlx.win, 04, 0, mouse_func, &vars);
	mlx_loop_hook(vars.mlx.mlx, render, (void *)(&vars));
	mlx_loop(vars.mlx.mlx);
	return (close_windows(&vars), 0);
}
