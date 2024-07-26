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

#include "../../includes/bonus.h"
#include "../../minilibx/mlx.h"
#include "../../libft/libft.h"
#include <math.h>
#define TILE_GUN 9

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
	vars->ui_canvas.img = mlx_new_image(vars->mlx.mlx, vars->render.sc_width,
			vars->render.sc_height);
	if (!vars->ui_canvas.img)
		return (err("İmage init error!"), 1);
	vars->ui_canvas.addr = mlx_get_data_addr(vars->ui_canvas.img, &vars->ui_canvas.bits_per_pixel,
			&vars->ui_canvas.line_length, &vars->ui_canvas.endian);
	if (!vars->ui_canvas.addr)
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

void scale_up_image(t_data *data, t_data canvas, int original_width, int original_height, double tile_size, int s_x, int s_y)
{
	for (int y = 0; y < original_height; y++)
	{
		for (int x = 0; x < original_width; x++)
		{
			for (int ty = 0; ty < tile_size; ty++)
			{
				for (int tx = 0; tx < tile_size; tx++)
				{
					pixel_put(&canvas, (x * tile_size) + tx + s_x, (y * tile_size) + ty + s_y, texture_color(data, x, y));
				}
			}
		}
	}
}
#include <unistd.h>
#include <sys/time.h>

int	calculate_ammo_count(t_vars *vars, double pos_tile)
{
	int	ammo;
	int	pos;

	ammo = vars->ammo;
	pos = 0;
	while (ammo != 0)
	{
		scale_up_image(&vars->num[ammo % 10], vars->ui_canvas, 7, 10, pos_tile, vars->render.sc_width * 0.8 - pos * 7, vars->render.sc_height * 0.8);
		ammo /= 10;
		pos += pos_tile;
	}
	if (vars->ammo == 0)
		scale_up_image(&vars->num[ammo % 10], vars->ui_canvas, 7, 10, pos_tile, vars->render.sc_width * 0.8 + pos, vars->render.sc_height * 0.8);
	return (0);
}

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		return (err("gettimeofday() error"), -1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	render(void *ptr)
{
	t_vars	*vars;

	vars = (t_vars *)ptr;
	if (vars->player.running != 1 && vars->fov_angle >= 64)
		vars->fov_angle--;
	else if (vars->player.running == 1 && vars->fov_angle < 66)
		vars->fov_angle++;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	get_canvas(vars);
	if (fill_background(vars))
		return (close_windows(vars), 1);
	mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
	cast_rays(vars);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	render_mini_map(vars);
	if (!vars->player.gun_type)
		scale_up_image(&vars->gun[vars->player.gun_type][vars->player.ani_i], vars->gun_canvas, 64, 64, TILE_GUN, 0, 0);
	else
		scale_up_image(&vars->gun[vars->player.gun_type][vars->player.ani_i], vars->gun_canvas, 64, 64, TILE_GUN, 0, 0);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->gun_canvas.img,
		(vars->render.sc_width / 2) - ((64 * TILE_GUN) / 2),
		vars->render.sc_height - (64 * TILE_GUN));
	for (int y = 0; vars->render.sc_height > y; y++)
	{
		for (int x = 0; vars->render.sc_width > x; x++)
			pixel_put(&vars->ui_canvas, x, y, texture_color(&vars->num[0], 0, 0));
	}
	scale_up_image(&vars->map_arrow, vars->ui_canvas, 48, 48, 0.5, vars->render.sc_width * 0.2 / 2 - 12, vars->render.sc_height * 0.2 / 2 - 12);
	calculate_ammo_count(vars, 3.5);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui_canvas.img, 0, 0);
	if (vars->player.shoot == 1)
	{
		if ((int)(get_time() - vars->s_time) > 7 + vars->player.ani_i)
			vars->player.ani_i++;
		if (vars->player.gun_type != 1 && vars->player.ani_i >= 10)
		{
			vars->player.ani_i = 0;
			vars->player.shoot = 0;
		}
		else if (vars->player.gun_type == 1 && vars->player.ani_i >= 15)
		{
			vars->player.ani_i = 0;
			vars->player.shoot = 0;
		}
	}
	move_player(vars, 0.0, 0.0);
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	mlx_destroy_image(vars->mlx.mlx, vars->mini_map.img);
	mlx_destroy_image(vars->mlx.mlx, vars->gun_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui_canvas.img);
	return (0);
}
