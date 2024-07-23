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

#include <unistd.h>
int	color(t_vars *vars, int tile_x, int tile_y)
{
	int	max_x;
	int	max_y;

	max_y = double_counter(vars->map) * MAP_TILE;
	if (max_y <= tile_y || tile_y < 0)
		return (rgb_to_hex(0, 0, 0));
	max_x = ft_strlen(vars->map[tile_y / MAP_TILE]) * MAP_TILE;
	if (max_x < tile_x || tile_x < 0 || max_x == 0)
		return (rgb_to_hex(0, 0, 0));
	else if (vars->map[tile_y / MAP_TILE][tile_x / MAP_TILE] == '0'
	|| vars->map[tile_y / MAP_TILE][tile_x / MAP_TILE] == 'N'
	|| vars->map[tile_y / MAP_TILE][tile_x / MAP_TILE] == 'S'
	|| vars->map[tile_y / MAP_TILE][tile_x / MAP_TILE] == 'W'
	|| vars->map[tile_y / MAP_TILE][tile_x / MAP_TILE] == 'E')
		return (rgb_to_hex(255, 255, 255));
	else
		return (rgb_to_hex(0, 0, 0));
	// printf("tile_x: %d\ntile_y: %d\nmax_x: %d\nmax_y: %d\n",tile_x, tile_y, max_x, max_y );
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
	int	x;
	int	y;
	int	pos_x;
	int	pos_y;
	int	map_size_w;
	int	map_size_h;

	vars->map_h = double_counter(vars->map) * MAP_TILE;
	vars->map_w = find_longest_line(vars->map) * MAP_TILE;
	map_size_h = vars->render.sc_height * 0.20;
	map_size_w = vars->render.sc_width * 0.20;
	pos_x = (vars->player.camera[X] / TILE_SIZE) * MAP_TILE - map_size_w / 2;
	pos_y = (vars->player.camera[Y] / TILE_SIZE) * MAP_TILE - map_size_h / 2;
	y = -1;
	while (++y <= map_size_h)
	{
		x = -1;
		while (++x <= map_size_w)
			pixel_put(&vars->mini_map, x, y, color(vars, pos_x + x, pos_y + y));
	}
	return (draw_circle(vars, rgb_to_hex(255, 0, 0), map_size_w / 2,
			map_size_h / 2), 0);
}

	// for (int i = 0; vars->map[i]; i++)
	// {
	// 	for (int y = 0; y <= find_longest_line(vars->map); y++)
	// 	{
	// 		if (vars->map[i][y] == ' ' || vars->map[i][y] == 0)
	// 			printf("*");
	// 		else
	// 			printf("%c", vars->map[i][y]);
	// 	}
	// 	printf("\n");
	// }