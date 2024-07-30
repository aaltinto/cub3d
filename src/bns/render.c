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

t_data	*fill_t_data(t_data *data, t_vars *vars, int width, int height)
{
	data->img = mlx_new_image(vars->mlx.mlx, width, height);
	if (!data->img)
		return (err("Img can't initialized"), NULL);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	if (!data->addr)
		return (err("Img addr can't initialized"), NULL);
	return (data);
}

int	get_canvas(t_vars *vars)
{
	if (fill_t_data(&vars->mini_map, vars, vars->render.sc_width * 0.2,
			vars->render.sc_height * 0.2) == NULL)
		return (err("İmage init error!"), 1);
	if (!fill_t_data(&vars->img, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("İmage init error!"), 1);
	if (!fill_t_data(&vars->gun_canvas, vars, 64 * TILE_GUN,
			64 * TILE_GUN))
		return (err("İmage init error!"), 1);
	if (!fill_t_data(&vars->ui_canvas, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("İmage init error!"), 1);
	if (!fill_t_data(&vars->sprites_canvas, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("İmage init error!"), 1);
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
	t_vars		*vars;
	int	x;
	int	y;

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
	y = -1;
	while (vars->render.sc_height > ++y)
	{
		x = -1;
		while (vars->render.sc_width > ++x)
			pixel_put(&vars->sprites_canvas, x, y, -16777216);
	}
	cast_rays(vars);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->sprites_canvas.img, 0, 0);
	render_mini_map(vars);
	render_gun(vars);
	render_ui(vars);
	move_player(vars, 0.0, 0.0);
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	mlx_destroy_image(vars->mlx.mlx, vars->mini_map.img);
	mlx_destroy_image(vars->mlx.mlx, vars->gun_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->sprites_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui_canvas.img);
	return (0);
}
