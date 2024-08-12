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
	if (fill_t_data(&vars->ui.mini_map, vars, (int)(vars->render.sc_width * \
		0.2), (int)(vars->render.sc_height * 0.2)) == NULL)
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->img, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->ui.gun_canvas, vars, 64 * TILE_GUN,
			64 * TILE_GUN))
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->ui.ui_canvas, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->ui.sprites_canvas, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("Image init error!"), 1);
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

int	new_game(t_vars *vars)
{
	int	count;

	count = vars->game.spr_count;
	vars->new_game = 1;
	while (--count >= 0)
	{
		if (vars->sprites[count].is_enemy)
		{
			vars->sprites[count].spr_pos[X] = vars->sprites[count].org_pos[X];
			vars->sprites[count].spr_pos[Y] = vars->sprites[count].org_pos[Y];
			vars->sprites[count].life = 500;
			vars->sprites[count].spr_ani = 0;
			vars->sprites[count].time = 0;
		}
		if (vars->sprites[count].is_diamond)
			vars->sprites[count].life = 100;
	}
	vars->game.diamond = vars->game.diamond_org;
	vars->game.enemy_count = vars->game.enemy_org;
	vars->ui.ammo[0] = 30;
	vars->ui.ammo[1] = 30;
	vars->ui.ammo[2] = 3;
	detect_player(vars);
	vars->d_time = get_time();
	mlx_mouse_hide();
	mlx_mouse_move(vars->mlx.win, vars->render.sc_width / 2,
		vars->render.sc_height / 2);
	vars->player.life = 100;
	vars->ui.menu = 0;
	return (0);
}

void	make_transparent(t_vars *vars, t_data canvas)
{
	int		x;
	int		y;

	y = -1;
	while (vars->render.sc_height > ++y)
	{
		x = -1;
		while (vars->render.sc_width > ++x)
			pixel_put(&canvas, x, y, -16777216);
	}
}

void	collect_diamond(t_vars *vars, t_player player, t_sprite *sprites)
{
	int	pos[2];

	if (vars->game.diamond == 0)
	{
		mlx_mouse_show();
		vars->player.life = 0;
		vars->ui.menu = 1;
	}
	if (!sprites->is_diamond)
		return ;
	if (vars->game.enemy_count != 0)
	{
		if ((sprites->spr_pos[X] <= (player.camera[X] / TILE_SIZE) + 0.5 \
			&& sprites->spr_pos[X] >= (player.camera[X] / TILE_SIZE) - 0.5)
			&& (sprites->spr_pos[Y] <= (player.camera[Y] / TILE_SIZE) + 0.5 \
			&& sprites->spr_pos[Y] >= (player.camera[Y] / TILE_SIZE) - 0.5))
			vars->d_time = get_time();
		pos[X] = vars->render.sc_width / 2;
		pos[Y] = vars->render.sc_height * 0.22;
		if (get_time() - vars->d_time < 3000)
		{
			print_text(vars, "kill all the martians", pos, 3);
			mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui.ui_canvas.img, 0, 0);
		}
		return ;
	}
	if ((sprites->spr_pos[X] <= (player.camera[X] / TILE_SIZE) + 0.5 \
		&& sprites->spr_pos[X] >= (player.camera[X] / TILE_SIZE) - 0.5)
		&& (sprites->spr_pos[Y] <= (player.camera[Y] / TILE_SIZE) + 0.5 \
		&& sprites->spr_pos[Y] >= (player.camera[Y] / TILE_SIZE) - 0.5))
	{
		vars->game.diamond--;
		sprites->life = 0;
	}
	if ( get_time() - vars->d_time < 3000)
	{
		pos[X] = vars->render.sc_width / 2;
		pos[Y] = vars->render.sc_height * 0.22;
		print_text(vars, "collect the diamonds", pos, 3);
		mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui.ui_canvas.img, 0, 0);
	}
}

int	find(double num)
{
	return (num +((num / fabs(num)) * 0.25f));
}

void	spr_move(t_vars *vars, double *pos, int i)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	if (pos[X] > (vars->player.pos[X] / TILE_SIZE - 0.5))
		x -= 0.025;
	else
		x += 0.025;
	if (pos[Y] > (vars->player.pos[Y] / TILE_SIZE - 0.5))
		y -= 0.025;
	else
		y += 0.025;
	if (vars->map[(int)(round(pos[Y] + find(y)))][(int)(round(pos[X]))] != '1'
		&& vars->map[(int)(round(pos[Y] + find(y)))][(int)(round(pos[X]))]
		!= 'B')
		if (fabs((pos[Y] + y) - (vars->player.pos[Y] / TILE_SIZE - 0.5)) >= 0.1)
			vars->sprites[i].spr_pos[Y] += y;
	if (vars->map[(int)(round(pos[Y]))][(int)(round(pos[X] + find(x)))] != '1'
		&& vars->map[(int)(round(pos[Y]))][(int)(round(pos[X] + find(x)))]
		!= 'B')
		if (fabs((pos[X] + x) - (vars->player.pos[X] / TILE_SIZE - 0.5)) >= 0.1)
			vars->sprites[i].spr_pos[X] += x;
}

void	sprite_func(t_vars *vars)
{
	int		i;
	double	*pos;
	double	x;
	double	y;
	double	dist;

	i = -1;
	while (++i < vars->game.spr_count)
	{
		if (vars->sprites[i].life <= 0)
			continue ;
		collect_diamond(vars, vars->player, &(vars->sprites[i]));
		dist = euclid_dist(vars->player.pos, vars->sprites[i].spr_pos);
		pos = vars->sprites[i].spr_pos;
		if (dist < 7 && dist > 0.75f && vars->sprites[i].is_enemy)
			spr_move(vars, pos, i);
	}
}

void	destroy_canvases(t_vars *vars)
{
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui.mini_map.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui.gun_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui.sprites_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui.ui_canvas.img);
}

int	render(void *ptr)
{
	t_vars	*vars;

	vars = (t_vars *)ptr;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	if (get_canvas(vars) )//|| music(vars))
		return (close_windows(vars, 1, 0));
	if (vars->ui.menu)
	{
		mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
		if (menu(vars))
			return (destroy_canvases(vars), close_windows(vars, 1, 0));
		destroy_canvases(vars);
		return (0);
	}
	mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
	fill_background(vars);
	make_transparent(vars, vars->ui.sprites_canvas);
	cast_rays(vars);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win,
		vars->ui.sprites_canvas.img, 0, 0);
	render_mini_map(vars);
	if (render_gun(vars))
		return (close_windows(vars, 1, 0));
	render_ui(vars);
	move_player(vars, 0.0, 0.0);
	if (vars->game.spr_count)
		sprite_func(vars);
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	destroy_canvases(vars);
	return (0);
}
