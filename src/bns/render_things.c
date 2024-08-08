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

int	hover_effect(t_vars *vars, t_data *data, int *pos, t_img_args args)
{
	int	color;
	int	mx;
	int	my;

	mlx_mouse_get_pos(vars->mlx.win, &mx, &my);
	color = texture_color(data, pos[X], pos[Y]);
	if (!vars->ui.menu)
		return (color);
	if (color == 16777215 && my > vars->render.sc_height / 2
				&& my < vars->render.sc_height / 2 + 40
				&& mx < vars->render.sc_width / 2 + 95
				&& mx > vars->render.sc_width / 2 - 105
				&& args.pos_y == 450)
		color = rgb_to_hex(209, 95, 43);
	else if (color == 16777215 && my > vars->render.sc_height / 2 + 80
				&& my < vars->render.sc_height / 2 + 120
				&& mx < vars->render.sc_width / 2 + 75
				&& mx > vars->render.sc_width / 2 - 85
				&& args.pos_y == 530)
		color = rgb_to_hex(209, 95, 43);
	return (color);
}

void	menu_printer(t_vars *vars, t_data *data, t_data canvas, t_img_args args)
{
	int	pos[2];
	int	color;
	int	tile[2];

	pos[Y] = -1;
	while (++pos[Y] < args.original_height)
	{
		pos[X] = -1;
		while (++pos[X] < args.original_width)
		{
			tile[Y] = -1;
			while (++tile[Y] < args.tile_size)
			{
				tile[X] = -1;
				while (++tile[X] < args.tile_size)
				{
					color = hover_effect(vars, data, pos, args);
					pixel_put(&canvas, (pos[X] * args.tile_size) + tile[X]
						+ args.pos_x, (pos[Y] * args.tile_size) + tile[Y]
						+ args.pos_y, color);
				}
			}
		}
	}
}

void	scale_up_image(t_data *data, t_data canvas, t_img_args args)
{
	int	pos[2];
	int	tile[2];

	pos[Y] = -1;
	while (++pos[Y] < args.original_height)
	{
		pos[X] = -1;
		while (++pos[X] < args.original_width)
		{
			tile[Y] = -1;
			while (++tile[Y] < args.tile_size)
			{
				tile[X] = -1;
				while (++tile[X] < args.tile_size)
				{
					pixel_put(&canvas, (pos[X] * args.tile_size) + tile[X]
						+ args.pos_x, (pos[Y] * args.tile_size) + tile[Y]
						+ args.pos_y, texture_color(data, pos[X], pos[Y]));
				}
			}
		}
	}
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
		close_windows(vars, 1, 1);
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
		vars->ui.gun_canvas, args);
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
		vars->ui.gun_canvas.img, (vars->render.sc_width / 2) - ((64 * TILE_GUN) \
		/ 2), vars->render.sc_height - (64 * TILE_GUN)), 0);
}

int	select_index(t_vars *vars)
{
	if (vars->player.life == 100)
		return (0);
	if (vars->player.life == 75)
		return (1);
	if (vars->player.life == 50)
		return (2);
	if (vars->player.life == 25)
		return (3);
	return (3);
}

void	render_ui(t_vars *vars)
{
	int			x;
	int			y;
	int			i;
	t_img_args	args;

	y = -1;
	while (vars->render.sc_height > ++y)
	{
		x = -1;
		while (vars->render.sc_width > ++x)
			pixel_put(&vars->ui.ui_canvas, x, y, -16777216);
	}
	args.original_height = 48;
	args.original_width = 48;
	args.tile_size = 0.5;
	args.pos_x = vars->render.sc_width * 0.2 / 2 - 12;
	args.pos_y = vars->render.sc_height * 0.2 / 2 - 12;
	scale_up_image(&vars->ui.map_arrow, vars->ui.ui_canvas, args);
	calculate_ammo_count(vars, 3.5);
	args.original_height = 97;
	args.original_width = 380;
	args.pos_x = vars->render.sc_width * 0.9 - 100;
	args.pos_y = vars->render.sc_height * 0.9;
	args.tile_size = 0.5;
	i = select_index(vars);
	scale_up_image(&vars->ui.healt_bar[i], vars->ui.ui_canvas, args);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui.ui_canvas.img,
		0, 0);
}
