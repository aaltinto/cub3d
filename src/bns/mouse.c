/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 18:05:15 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/23 18:05:16 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/bonus.h"
#include "../../minilibx/mlx.h"
#include <math.h>

int	mouse_func(int button, int x, int y, t_vars *vars)
{
	if (button == 1)
	{
		if (vars->menu)
		{
			if (y > vars->render.sc_height / 2 - 40
				&& x < vars->render.sc_width / 2 + 100
				&& x > vars->render.sc_width / 2 - 100)
			{
				vars->menu = 0;
				mlx_mouse_hide();
			}
			return (0);
		}
		if (vars->ammo == 0)
			return (0);
		vars->s_time = get_time();
		if (vars->ammo > 0 && vars->player.shoot == 0)
			vars->ammo--;
		if (vars->player.shoot == 0)
			vars->player.shoot = 1;
	}
	return (0);
}

int	mouse_move(t_vars *vars)
{
	int	mx;
	int	my;
	static int	x = 0;

	mlx_mouse_get_pos(vars->mlx.win, &mx, &my);
	if (mx != 0 && x == 0)
		x = vars->render.sc_width / 2;
	if (mx > vars->render.sc_width || mx < 5)
	{
		x = vars->render.sc_width / 2;
		mlx_mouse_move(vars->mlx.win, vars->render.sc_width / 2,
			vars->render.sc_height / 2);
		mlx_mouse_get_pos(vars->mlx.win, &mx, &my);
	}
	vars->player.p_angle += nor_angle((mx - x) * 0.005);
	vars->player.dir[X] = cos(vars->player.p_angle);
	vars->player.dir[Y] = sin(vars->player.p_angle);
	vars->player.plane[X] = (-1 * sin(vars->player.p_angle)) * tan((vars->player.fov / 2));
	vars->player.plane[Y] = cos(vars->player.p_angle) * tan(vars->player.fov / 2);
	x = mx;
	return (0);
}
