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

int	render(void *ptr)
{
	t_vars	*vars;
	int		x;
	int		y;

	vars = (t_vars *)ptr;
	mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
	get_canvas(vars);
	if (fill_background(vars))
		return (close_windows(vars), 1);
	cast_rays(vars);
	render_mini_map(vars);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->mini_map.img,
		0, 0);
	move_player(vars, 0.0, 0.0);
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	mlx_destroy_image(vars->mlx.mlx, vars->mini_map.img);
	return (0);
}
