/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_things.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 18:59:15 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/26 18:59:22 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/bonus.h"
#include "../../minilibx/mlx.h"
#include <stdlib.h>
#include <unistd.h>

void	scale_up_image(t_data *data, t_data canvas, t_img_args args)
{
	int	x;
	int	y;
	int	tile[2];

	y = -1;
	while (++y < args.original_height)
	{
		x = -1;
		while (++x < args.original_width)
		{
			tile[Y] = -1;
			while (++tile[Y] < args.tile_size)
			{
				tile[X] = -1;
				while (++tile[X] < args.tile_size)
					pixel_put(&canvas, (x * args.tile_size) + tile[X]
						+ args.pos_x, (y * args.tile_size) + tile[Y]
						+ args.pos_y, texture_color(data, x, y));
			}
		}
	}
}

void	calculate_ammo_count(t_vars *vars, double pos_tile)
{
	int			ammo;
	int			pos;
	t_img_args	args;

	ammo = vars->ammo;
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
		scale_up_image(&vars->num[ammo % 10], vars->ui_canvas, args);
		ammo /= 10;
		pos += pos_tile;
	}
	if (vars->ammo == 0)
		scale_up_image(&vars->num[ammo % 10], vars->ui_canvas, args);
}

int	make_sound(t_vars *vars)
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
		exit(0);
	return (0);
}

int	render_gun(t_vars *vars)
{
	t_img_args	args;

	args.original_height = 64;
	args.original_width = 64;
	args.tile_size = TILE_GUN;
	args.pos_x = 0;
	args.pos_y = 0;
	scale_up_image(&vars->gun[vars->player.gun_type][vars->player.ani_i],
		vars->gun_canvas, args);
	if (vars->player.shoot == 1)
	{
		if (make_sound(vars))
			return (1);
		if ((int)(get_time() - vars->s_time) > 7 + vars->player.ani_i)
			vars->player.ani_i++;
		if ((vars->player.gun_type != 1 && vars->player.ani_i >= 10)
			|| (vars->player.gun_type == 1 && vars->player.ani_i >= 15))
		{
			vars->player.ani_i = 0;
			vars->player.shoot = 0;
		}
	}
	return (mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, \
		vars->gun_canvas.img, (vars->render.sc_width / 2) - ((64 * TILE_GUN) \
		/ 2), vars->render.sc_height - (64 * TILE_GUN)), 0);
}

void	render_ui(t_vars *vars)
{
	int			x;
	int			y;
	t_img_args	args;

	y = -1;
	while (vars->render.sc_height > ++y)
	{
		x = -1;
		while (vars->render.sc_width > ++x)
			pixel_put(&vars->ui_canvas, x, y, -16777216);
	}
	args.original_height = 48;
	args.original_width = 48;
	args.tile_size = 0.5;
	args.pos_x = vars->render.sc_width * 0.2 / 2 - 12;
	args.pos_y = vars->render.sc_height * 0.2 / 2 - 12;
	scale_up_image(&vars->map_arrow, vars->ui_canvas, args);
	calculate_ammo_count(vars, 3.5);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui_canvas.img,
		0, 0);
}
