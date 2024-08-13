/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 15:06:05 by aaltinto          #+#    #+#             */
/*   Updated: 2024/08/12 15:06:08 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/bonus.h"
#include "../../minilibx/mlx.h"
#include <stdlib.h>
#include <unistd.h>

int	enemy_hit(t_vars *vars, int	*map_grid, int *hit)
{
	int			i;

	i = -1;
	while (vars->game.spr_count && vars->render.ray_angle + 0.09
		>= vars->player.p_angle
		&& vars->render.ray_angle - 0.09 <= vars->player.p_angle
		&& vars->player.shoot
		&& *hit == 0 && ++i < vars->game.spr_count)
	{
		if (vars->sprites[i].spr_pos[X] <= map_grid[X] + 0.5 \
		&& vars->sprites[i].spr_pos[X] >= map_grid[X] - 0.5 \
		&& vars->sprites[i].spr_pos[Y] <= map_grid[Y] + 0.5 \
		&& vars->sprites[i].spr_pos[Y] >= map_grid[Y] - 0.5)
		{
			if (vars->player.gun_type != 2)
				*hit = 1;
			if (vars->sprites[i].is_enemy)
				vars->sprites[i].hit = 1;
		}
	}
	return (0);
}

void	calculate_ammo_count(t_vars *vars, double pos_tile)
{
	int			ammo;
	int			pos;
	t_img_args	args;

	ammo = vars->ui.ammo[vars->player.gun_type];
	pos = 0;
	args.original_height = 10;
	args.original_width = 7;
	args.tile_size = pos_tile;
	args.pos_x = vars->render.sc_width * 0.8 - pos * 7;
	args.pos_y = vars->render.sc_height * 0.8;
	while (ammo != 0)
	{
		args.pos_x = vars->render.sc_width * 0.8 - pos * 7;
		args.pos_y = vars->render.sc_height * 0.8;
		scale_up_image(&vars->ui.num[ammo % 10], vars->ui.ui_canvas, args);
		ammo /= 10;
		pos += pos_tile;
	}
	if (vars->ui.ammo[vars->player.gun_type] == 0)
		scale_up_image(&vars->ui.num[ammo % 10], vars->ui.ui_canvas, args);
}

static int	make_sound(t_vars *vars)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		return (err("Fork error"));
	if (vars->player.shoot == 1 && vars->player.ani_i == 0)
	{
		if (pid == 0)
		{
			if (!vars->player.gun_type)
				system("afplay xpm/Huntershoot.wav");
			else if (vars->player.gun_type == 1)
				system("afplay xpm/Talonshoot.wav");
			else
				system("afplay xpm/Thermshoot.wav");
		}
	}
	if (pid == 0)
		close_windows(vars, 1, 1);
	return (0);
}

static int	shoot(t_vars *vars, int *gun)
{
	if (make_sound(vars))
		return (1);
	if ((int)(get_time() - vars->s_time) > 7 + vars->player.ani_i)
		vars->player.ani_i++;
	if ((*gun != 1 && vars->player.ani_i >= 10)
		|| (*gun == 1 && vars->player.ani_i >= 15))
	{
		vars->player.ani_i = 0;
		vars->player.shoot = 0;
	}
	return (0);
}

int	render_gun(t_vars *vars)
{
	t_img_args	args;
	static int	gun = 0;

	args.original_height = 64;
	args.original_width = 64;
	args.tile_size = TILE_GUN;
	args.pos_x = 0;
	args.pos_y = 0;
	if (vars->player.shoot == 1)
		shoot(vars, &gun);
	if (vars->player.ani_i == 0)
		gun = vars->player.gun_type;
	scale_up_image(&vars->gun[gun][vars->player.ani_i], vars->ui.gun_canvas,
		args);
	return (0);
}
