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
	set_null((void ***)&vars->gun_name, 4);
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
	vars->ui.ammo = malloc(sizeof(int) * 3);
	if (!vars->ui.ammo)
		return (err("Malloc error"));
	vars->ui.ammo[0] = 30;
	vars->ui.ammo[1] = 30;
	vars->ui.ammo[2] = 3;
	vars->player.ani_i = 0;
	return (0);
}

int	claim_resources(t_vars *vars)
{
	vars->d_time = get_time();
	if (vars->d_time == 0)
		return (err("Get_time error"));
	vars->ui.sound = malloc(sizeof(char *) * 3);
	if (!vars->ui.sound)
		return (err("Malloc error"));
	vars->ui.sound[0] = ft_strdup("afplay sounds/menu_track.mp3");
	vars->ui.sound[1] = ft_strdup("afplay sounds/morbid_descent.mp3");
	vars->ui.sound[2] = NULL;
	if (!vars->ui.sound[0] || !vars->ui.sound[1])
		return (err("Strdup error"));
	vars->enemy.filename = malloc(sizeof(char *) * 6);
	if (!vars->enemy.filename)
		return (err("Malloc error"));
	vars->enemy.filename[0] = ft_strdup("./enemy/idle/idle");
	vars->enemy.filename[1] = ft_strdup("./enemy/attack/attack");
	vars->enemy.filename[2] = ft_strdup("./enemy/walk/walk");
	vars->enemy.filename[3] = ft_strdup("./enemy/damage/damaged");
	vars->enemy.filename[4] = ft_strdup("./enemy/death/death");
	vars->enemy.filename[5] = NULL;
	if (!vars->enemy.filename[0] || !vars->enemy.filename[1]
		|| !vars->enemy.filename[2] || !vars->enemy.filename[3]
		|| !vars->enemy.filename[4])
		return (err("Strdup error"));
	return (0);
}

void	set_default(t_vars *vars)
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
	vars->ui.sound = NULL;
	vars->gun = NULL;
	vars->ui.map_arrow.img = NULL;
	vars->enemy.sprites = NULL;
	vars->ui.ammo = NULL;
	vars->gun_name = NULL;
	vars->sprites = NULL;
	vars->render.flag = 0;
	vars->keys.key_a = 0;
	vars->keys.key_s = 0;
	vars->keys.key_d = 0;
	vars->keys.key_w = 0;
	vars->keys.key_la = 0;
	vars->keys.key_ra = 0;
}

int	marche(t_vars *vars)
{
	vars->render.sc_height = 900;
	vars->render.sc_width = 1090;
	vars->ui.music_on = 1;
	vars->game.start = 0;
	vars->new_game = 1;
	vars->player.running = 1;
	vars->fov_angle = 60;
	vars->game.spr_count = 0;
	vars->ui.time = get_time();
	vars->ui.menu = 1;
	vars->player.life = 100;
	vars->player.plane[X] = 0.0;
	vars->player.plane[Y] = 0.60;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	vars->enemy.index[0] = 4;
	vars->enemy.index[1] = 6;
	vars->enemy.index[2] = 6;
	vars->enemy.index[3] = 4;
	vars->enemy.index[4] = 9;
	set_default(vars);
	if (set_guns(vars) || claim_resources(vars))
		return (1);
	return (0);
}

int	main(int ac, char **argv)
{
	t_vars	vars;

	if (ac != 2)
		return (err(ARG));
	if (marche(&vars) || read_map(argv, &vars))
		return (abort_mission(&vars, 1, 0), 1);
	vars.mlx.mlx = mlx_init();
	if (!vars.mlx.mlx)
		return (abort_mission(&vars, 1, 0), err("Mlx init error"));
	vars.mlx.win = mlx_new_window(vars.mlx.mlx, vars.render.sc_width,
			vars.render.sc_height, "cub3d");
	if (!vars.mlx.win)
		return (abort_mission(&vars, 1, 0), err("Mlx window error"));
	if (get_textures(&vars) || get_magnum_sprites(&vars))
		return (close_windows(&vars, 1, 0), 1);
	if (get_num_sprites(&vars, 7, 10))
		return (close_windows(&vars, 1, 0), 1);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_hook(vars.mlx.win, 03, 0, key_release, &vars);
	mlx_hook(vars.mlx.win, 04, 0, mouse_func, &vars);
	mlx_loop_hook(vars.mlx.mlx, render, (void *)(&vars));
	mlx_loop(vars.mlx.mlx);
	return (close_windows(&vars, 1, 0), 0);
}
