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

void draw_circle(t_vars *vars, int color, int center_x, int center_y)
{
	int x;
	int y;
	int err;

	y = 0;
	x = TILE_SIZE * 0.20f;
	err = 0;
	while (x >= y)
	{
		pixel_put(&vars->mini_map, center_x + x, center_y + y, color);
		pixel_put(&vars->mini_map, center_x + y, center_y + x, color);
		pixel_put(&vars->mini_map, center_x - y, center_y + x, color);
		pixel_put(&vars->mini_map, center_x - x, center_y + y, color);
		pixel_put(&vars->mini_map, center_x - x, center_y - y, color);
		pixel_put(&vars->mini_map, center_x - y, center_y - x, color);
		pixel_put(&vars->mini_map, center_x + y, center_y - x, color);
		pixel_put(&vars->mini_map, center_x + x, center_y - y, color);
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
	draw_arrow(vars, center_x, center_y, TILE_SIZE * 0.40f, vars->player.p_angle, color);
}


int	color(t_vars *vars, int tile_x, int tile_y)
{
	if (vars->map[tile_y][tile_x] == '1' || vars->map[tile_y][tile_x] == ' ')
		return (rgb_to_hex(0, 0, 0));
	else
		return (rgb_to_hex(255, 255, 255));
}

void	put_tiles(t_vars *vars, int x, int y, int color)
{
	int	i;
	int	j;
	int	scale_width;
	int	scale_height;

	scale_width = (int)(vars->render.sc_width * 0.20f) / MAP_TILE;
	scale_height = (int)(vars->render.sc_height * 0.20f) / MAP_TILE;
	j = 0;
	while (j < scale_height)
	{
		i = 0;
		while (i < scale_width)
		{
			pixel_put(&(vars->mini_map), x * scale_width + i, y * scale_height
				+ j, color);
			i++;
		}
		j++;
	}
}

int render_mini_map(t_vars *vars)
{
	int	pos_x;
	int	pos_y;
	int	x;
	int	y;
	int	tile_x;
	int	tile_y;
	int	scale_width;
	int	scale_height;

	scale_width = (int)(vars->render.sc_width * 0.20f) / MAP_TILE;
	scale_height = (int)(vars->render.sc_height * 0.20f) / MAP_TILE;

	pos_x = (int)round(vars->player.pos_x / TILE_SIZE);
	pos_y = (int)round(vars->player.pos_y / TILE_SIZE);
	vars->map_h = double_counter(vars->map);
	y = 0;
	tile_y = pos_y - 5 - 1;
	while (++tile_y <= pos_y + 5)
	{
		if ((tile_y < 0 || tile_y >= vars->map_h) && ++y)
			continue ;
		x = 0;
		tile_x = pos_x - 5 - 1;
		while (++tile_x <= pos_x + 5)
		{
			if ((tile_x < 0 || tile_x >= ft_strlen(vars->map[tile_y])) && ++x)
				continue ;
			put_tiles(vars, x, y, color(vars, tile_x, tile_y));
			x++;
		}
		y++;
	}
	double	add_x = vars->player.pos_x/TILE_SIZE - pos_x - 8;
	double	add_y = vars->player.pos_y/TILE_SIZE - pos_y - 8;
	draw_circle(vars, rgb_to_hex(255, 0, 0), vars->render.sc_width*0.10 + add_x, vars->render.sc_height*0.10 + add_y);
	return (0);
}
