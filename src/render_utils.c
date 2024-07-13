/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:15:43 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/12 23:15:44 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	get_color(t_vars *vars, int flag)
{
	vars->render.ray_angle = nor_angle(vars->render.ray_angle);
	if (flag == 0)
	{
		if (vars->render.ray_angle > M_PI / 2
			&& vars->render.ray_angle < 3 * M_PI / 2)
			return (rgb_to_hex(145, 49, 43));
		else
			return (rgb_to_hex(150, 54, 48));
	}
	else
	{
		if (vars->render.ray_angle > 0 && vars->render.ray_angle < M_PI)
			return (rgb_to_hex(150, 54, 48));
		else
			return (rgb_to_hex(170, 74, 68));
	}
}
