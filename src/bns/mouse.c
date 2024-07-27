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

int	mouse_func(int button, int x, int y, t_vars *vars)
{
	(void)x;
	(void)y;
	if (button == 1)
	{
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
	int	x;
	int	y;

	x = vars->render.sc_width / 2;
	y = vars->render.sc_height / 2;
	mlx_mouse_get_pos(vars->mlx.win, &mx, &my);
	if (mx > x + 721 || mx < x - 721)
		mlx_mouse_move(vars->mlx.win, x, y);
	mx = mx / 2;
	my = mx / 2;
	vars->player.p_angle = nor_angle(mx * 0.05);
	return (0);
}
