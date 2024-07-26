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

#include "../../includes/bonus.h"
#include "../../libft/libft.h"
#include "../../minilibx/mlx.h"
#include <math.h>

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
}

int render_mini_map(t_vars *vars)
{
    int x, y;
    int pos_x, pos_y;
    int map_size_w, map_size_h;
    double angle_rad = nor_angle(vars->player.p_angle + M_PI / 2);
    double cos_angle = cos(angle_rad);
    double sin_angle = sin(angle_rad);

    vars->map_h = double_counter(vars->map) * MAP_TILE;
    vars->map_w = find_longest_line(vars->map) * MAP_TILE;
    map_size_h = vars->render.sc_height * 0.20;
    map_size_w = vars->render.sc_width * 0.20;
    pos_x = (vars->player.camera[X] / TILE_SIZE) * MAP_TILE - map_size_w / 2;
    pos_y = (vars->player.camera[Y] / TILE_SIZE) * MAP_TILE - map_size_h / 2;

    int center_x = pos_x + map_size_w / 2;
    int center_y = pos_y + map_size_h / 2;

    for (y = 0; y <= map_size_h; ++y)
    {
        for (x = 0; x <= map_size_w; ++x)
        {
            // Translate point to origin
            int translated_x = x - map_size_w / 2;
            int translated_y = y - map_size_h / 2;

            // Rotate point
            int rotated_x = translated_x * cos_angle - translated_y * sin_angle;
            int rotated_y = translated_x * sin_angle + translated_y * cos_angle;

            // Translate point back
            int final_x = rotated_x + center_x;
            int final_y = rotated_y + center_y;

            pixel_put(&vars->mini_map, x, y, color(vars, final_x, final_y));
        }
    }
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->mini_map.img, 0, 0);
    return 0;
}
