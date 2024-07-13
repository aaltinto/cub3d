/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:17:42 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/12 17:18:01 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include "../libft/libft.h"
#include <math.h>

void	draw_line(t_vars *vars, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
	int err = (dx > dy ? dx : -dy) / 2, e2;

    while (1)
    {
        pixel_put(&vars->mini_map, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void	draw_arrow(t_vars *vars, int centerX, int centerY, int length, double angle, int color)
{
	int	endX = centerX + (int)(length * cos(angle));
	int	endY = centerY + (int)(length * sin(angle));	

	draw_line(vars, centerX, centerY, endX, endY, color);
}

void	draw_circle(t_vars *vars, int color)
{
	int	x;
	int	y;
	int	err;

	y = 0;
	x = TILE_SIZE * 0.20f;
	err = 0;
	while (x >= y)
	{
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f + x,
			vars->render.sc_height * 0.10f + y, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f + y,
			vars->render.sc_height * 0.10f + x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f - y,
			vars->render.sc_height * 0.10f + x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f - x,
			vars->render.sc_height * 0.10f + y, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f - x,
			vars->render.sc_height * 0.10f - y, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f - y,
			vars->render.sc_height * 0.10f - x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f + y,
			vars->render.sc_height * 0.10f - x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width * 0.10f + x,
			vars->render.sc_height * 0.10f - y, color);
		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
	draw_arrow(vars, vars->render.sc_width * 0.10f, vars->render.sc_height
		* 0.10f, TILE_SIZE * 0.40f, vars->player.p_angle, color);
}

int	render_mini_map(t_vars *vars)
{
	int	pos_x;
	int	pos_y;
	int	x;
	int	y;
	int	color;
	int	scale_width;
	int	scale_height;
	int	tile_x;
	int	tile_y;

	pos_x = (int)round(vars->player.pos_x / TILE_SIZE);
	pos_y = (int)round(vars->player.pos_y / TILE_SIZE);

	int	map_height = double_counter(vars->map);
	int	map_width = ft_strlen(vars->map[0]);

	scale_width = (int)(vars->render.sc_width * 0.20f) / MAP_TILE;
	scale_height = (int)(vars->render.sc_height * 0.20f) / MAP_TILE;

	y = 0;
	for (tile_y = pos_y - 5; tile_y <= pos_y + 5; tile_y++, y++)
	{
		if (tile_y < 0 || tile_y >= map_height) continue;

		x = 0;
		for (tile_x = pos_x - 5; tile_x <= pos_x + 5; tile_x++, x++)
		{
			if (tile_x < 0 || tile_x >= ft_strlen(vars->map[tile_y])) continue;

			if (vars->map[tile_y][tile_x] == '1' || vars->map[tile_y][tile_x] == ' ')
				color = rgb_to_hex(0, 0, 0);
			else
				color = rgb_to_hex(255, 255, 255);

			for (int j = 0; j < scale_height; j++)
			{
				for (int i = 0; i < scale_width; i++)
				{
					pixel_put(&(vars->mini_map), x * scale_width + i, y * scale_height + j, color);
				}
			}
		}
	}
	draw_circle(vars, rgb_to_hex(255, 0, 0));
	return (0);
}
