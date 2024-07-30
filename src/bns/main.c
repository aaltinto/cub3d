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

int	set_guns(t_vars *vars)
{
	vars->player.shoot = 0;
	vars->gun_name = malloc(sizeof(char *) * 4);
	if (!vars->gun_name)
		return (err("Malloc error"));
	vars->gun_name[0] = ft_strdup("./xpm/Hunter");
	if (!vars->gun_name[0])
		return (err("Malloc error"));
	vars->gun_name[1] = ft_strdup("./xpm/Talon");
	if (!vars->gun_name[1])
		return (err("Malloc error"));
	vars->gun_name[2] = ft_strdup("./xpm/Therm");
	if (!vars->gun_name[2])
		return (err("Malloc error"));
	vars->gun_name[3] = NULL;
	vars->player.gun_type = 0;
	vars->ammo = 10;
	vars->player.ani_i = 0;
	return (0);
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
	vars->fov_angle = 0.66;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	if (set_guns(vars))
		return (1);
	return (0);
}

int	main(int ac, char **argv)
{
	t_vars	vars;
	int x = 64;
	int y = 64;

	if (ac != 2)
		return (err(ARG));
	if (marche(&vars) || read_map(argv, &vars))
		return (abort_mission(&vars), 1);
	vars.mlx.mlx = mlx_init();
	if (!vars.mlx.mlx)
		return (err("Mlx init error"));
	vars.mlx.win = mlx_new_window(vars.mlx.mlx, vars.render.sc_width,
			vars.render.sc_height, "cub3d");
	if (!vars.mlx.win)
		return (err("Mlx window error"), close_windows(&vars), 1);
	if (get_textures(&vars) || get_magnum_sprites(&vars)
		|| get_num_sprites(&vars, 7, 10))
		return (close_windows(&vars), 1);
	vars.sprite.img = mlx_xpm_file_to_image(vars.mlx.mlx, "./textures/barrel.xpm", &x, &y);
	if (!vars.sprite.img)
		return (err("something bad"));
	vars.sprite.addr = mlx_get_data_addr(vars.sprite.img, &vars.sprite.bits_per_pixel, &vars.sprite.line_length, &vars.sprite.endian);
	if (!vars.sprite.addr)
		return (err("something bad"));
	mlx_mouse_hide();
	mlx_mouse_move(vars.mlx.win, vars.render.sc_width / 2, vars.render.sc_height / 2);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_hook(vars.mlx.win, 03, 0, key_release, &vars);
	mlx_hook(vars.mlx.win, 04, 0, mouse_func, &vars);
	mlx_loop_hook(vars.mlx.mlx, render, (void *)(&vars));
	mlx_loop(vars.mlx.mlx);
	return (close_windows(&vars), 0);
}
