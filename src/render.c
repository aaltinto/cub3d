/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:15:38 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/13 02:06:19 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include "../minilibx/mlx.h"
#include "../libft/libft.h"
#include <math.h>
#define TILE_GUN 8

int	get_canvas(t_vars *vars)
{
	vars->mini_map.img = mlx_new_image(vars->mlx.mlx, vars->render.sc_width
			* 0.20f, vars->render.sc_height * 0.20f);
	if (!vars->mini_map.img)
		return (err("İmage init error!"), 1);
	vars->mini_map.addr = mlx_get_data_addr(vars->mini_map.img,
			&vars->mini_map.bits_per_pixel, &vars->mini_map.line_length,
			&vars->mini_map.endian);
	if (!vars->mini_map.addr)
		return (err("İmage addr error!"), 1);
	vars->img.img = mlx_new_image(vars->mlx.mlx, vars->render.sc_width,
			vars->render.sc_height);
	if (!vars->img.img)
		return (err("İmage init error!"), 1);
	vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bits_per_pixel,
			&vars->img.line_length, &vars->img.endian);
	if (!vars->img.addr)
		return (err("İmage addr error!"), 1);
	vars->gun_canvas.img = mlx_new_image(vars->mlx.mlx, 64 * TILE_GUN,
			64 * TILE_GUN);
	if (!vars->gun_canvas.img)
		return (err("İmage init error!"), 1);
	vars->gun_canvas.addr = mlx_get_data_addr(vars->gun_canvas.img, &vars->gun_canvas.bits_per_pixel,
			&vars->gun_canvas.line_length, &vars->gun_canvas.endian);
	if (!vars->gun_canvas.addr)
		return (err("İmage addr error!"), 1);
	return (0);
}

int	fill_background(t_vars *vars)
{
	int		i;
	int		color_floor;
	int		color_celing;

	color_floor = rgb_to_hex(vars->textures.floor_rgb[0], \
		vars->textures.floor_rgb[1], vars->textures.floor_rgb[2]);
	color_celing = rgb_to_hex(vars->textures.ceiling_rgb[0], \
		vars->textures.ceiling_rgb[1], vars->textures.ceiling_rgb[2]);
	i = -1;
	while (++i < (vars->render.sc_height * vars->render.sc_width))
	{
		if (i < vars->render.sc_width * vars->render.sc_height / 2)
			pixel_put(&vars->img, i % vars->render.sc_width, i
				/ vars->render.sc_width, color_celing);
		else
			pixel_put(&vars->img, i % vars->render.sc_width, i
				/ vars->render.sc_width, color_floor);
	}
	return (0);
}

void scale_up_image(t_data *data, t_data canvas, int original_width, int original_height, int tile_size)
{
	// Loop through each pixel in the original image
	for (int y = 0; y < original_height; y++)
	{
		for (int x = 0; x < original_width; x++)
		{
			for (int ty = 0; ty < tile_size; ty++)
			{
				for (int tx = 0; tx < tile_size; tx++)
				{
					pixel_put(&canvas, (x * tile_size) + tx, (y * tile_size) + ty, texture_color(data, x, y));
				}
			}
		}
	}
}

int	render(void *ptr)
{
	static char	*addr = NULL;
	static int	i = 0;
	t_vars	*vars;
	int		x;
	int		y;

	if (i == 10)
		i = 0;
	vars = (t_vars *)ptr;
	if (vars->player.running != 1 && vars->fov_angle >= 64)
		vars->fov_angle--;
	else if (vars->player.running == 1 && vars->fov_angle < 66)
		vars->fov_angle++;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	get_canvas(vars);
	if (fill_background(vars))
		return (close_windows(vars), 1);
	cast_rays(vars);
	render_mini_map(vars);
	addr = vars->img.addr;
	//mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->mini_map.img,
		0, 0);
	scale_up_image(&vars->gun[1], vars->gun_canvas, 64, 64, TILE_GUN);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->gun_canvas.img,
		(vars->render.sc_width / 2) - ((64 * TILE_GUN) / 2),
		vars->render.sc_height - (64 * TILE_GUN));
	move_player(vars, 0.0, 0.0);
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	mlx_destroy_image(vars->mlx.mlx, vars->mini_map.img);
	mlx_destroy_image(vars->mlx.mlx, vars->gun_canvas.img);
	i++;
	return (0);
}
