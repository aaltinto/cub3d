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
	if (fill_t_data(&vars->mini_map, vars, (int)(vars->render.sc_width * 0.2),
			(int)(vars->render.sc_height * 0.2)) == NULL)
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->img, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->gun_canvas, vars, 64 * TILE_GUN,
			64 * TILE_GUN))
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->ui_canvas, vars, vars->render.sc_width,
			vars->render.sc_height))
		return (err("Image init error!"), 1);
	if (!fill_t_data(&vars->sprites_canvas, vars, vars->render.sc_width,
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

#include <math.h>

double calculate_angle(double *cam, double *pos)
{
    double dx = cam[X] - pos[X];
    double dy = cam[Y] - pos[Y];
    double angle = atan2(dy, dx);
    double angle_degrees = angle * (180.0 / M_PI);
    return (angle_degrees);
}

int	print_text(t_vars *vars, char *text, int *pos, double size)
{
	t_img_args	args;
	int			i;
	int			c;

	if (!text)
		return (err("Error\ntext can't be null"));
	args.original_height = 8;
	args.original_width = 8;
	args.pos_x = pos[X] - ((ft_strlen(text) * 8 * size) / 2);
	if (args.pos_x < 0)
		args.pos_x = 0;
	if (vars->render.sc_width < args.pos_x + (ft_strlen(text) * size))
	args.pos_x += vars->render.sc_width - (args.pos_x + (ft_strlen(text) * size));
	if (args.pos_x < 0)
		return (err("Error\nText too big to fit"));
	args.pos_y = pos[Y];
	args.tile_size = size;
	i = -1;
	while (text[++i])
	{
		if (text[i] == ' ')
		{
			args.pos_x += 8 * size;
			continue ;
		}
		if (!ft_isalpha(text[i]))
			return (err("Error\nUnrecognized char in the text. All must \
be alphabetical"));
		c = ft_tolower(text[i]) - 97;
		menu_printer(vars, &vars->alp[c], vars->ui_canvas, args);
		args.pos_x += 8 * size;
	}
	return (0);
}

int	new_game(t_vars *vars)
{
	int	i;
	int	j;
	int	count;

	i = -1;
	count = vars->spr_count;
	while (vars->map[++i])
	{
		j = -1;
		while (vars->map[i][++j])
		{
			if (vars->map[i][j] == 'X' || vars->map[i][j] == 'B' || vars->map[i][j] == 'D')
			{
				count--;
				if (vars->map[i][j] != 'X')
					continue ;
				vars->sprites[count].spr_pos[X] = (double)j;
				vars->sprites[count].spr_pos[Y] = (double)i;
				vars->sprites[count].hit = 0;
				vars->sprites[count].life = 500;
				vars->sprites[count].emy_ani = 0;
				vars->sprites[count].spr_ani = 0;
				vars->sprites[count].time = 0;
			}
		}
	}
	detect_player(vars);
	vars->player.life = 100;
	vars->menu = 0;
	return (0);
}

int	opening_menu(t_vars *vars)
{
	int	pos[2];

	if (vars->player.life <= 0)
	{
		pos[X] = vars->render.sc_width / 2;
		pos[Y] = vars->render.sc_height * 0.2;
		if (print_text(vars, "game over", pos, 8))
			return (1);
			pos[X] = vars->render.sc_width / 2;
		pos[Y] = vars->render.sc_height / 2;
		if (print_text(vars, "new game", pos, 5))
			return (1);
		pos[X] = vars->render.sc_width / 2;
		pos[Y] = vars->render.sc_height / 2 + 80;
		if (print_text(vars, "Quit", pos, 5))
			return (1);
		mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui_canvas.img, 0, 0);
		return (0);
	}
	pos[X] = vars->render.sc_width / 2;
	pos[Y] = vars->render.sc_height * 0.2;
	if (print_text(vars, "cube game", pos, 8))
		return (1);
	pos[X] = vars->render.sc_width / 2;
	pos[Y] = vars->render.sc_height / 2;
	if (print_text(vars, "start", pos, 5))
		return (1);
	pos[X] = vars->render.sc_width / 2;
	pos[Y] = vars->render.sc_height / 2 + 80;
	if (print_text(vars, "quit", pos, 5))
		return (1);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->ui_canvas.img, 0, 0);
	return (0);
}

int	render(void *ptr)
{
	t_vars	*vars;
	int		x;
	int		y;

	vars = (t_vars *)ptr;
	if (vars->player.running != 1 && vars->fov_angle >= 64)
		vars->fov_angle--;
	else if (vars->player.running == 1 && vars->fov_angle < 66)
		vars->fov_angle++;
	vars->player.fov = vars->fov_angle * (M_PI / 180);
	get_canvas(vars);
	if (vars->menu)
	{
		mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
		if (opening_menu(vars))
			return (close_windows(vars, 1));
		return (0);
	}
	if (fill_background(vars))
		return (close_windows(vars, 1), 1);
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
	if (vars->spr_count)
	{
		int i = -1;
		double x;
		double y;
	
		while (++i < vars->spr_count)
		{
			x = 0;
			y = 0;
			if (vars->sprites[i].life <= 0)
				continue ;
			double dist = euclid_dist(vars->player.pos, vars->sprites[i].spr_pos);
			if (dist < 7 && dist > 0.75f && vars->sprites[i].is_enemy)
			{
				if (vars->sprites[i].spr_pos[X] > (vars->player.pos[X] / TILE_SIZE - 0.5))
					x -= 0.025;
				else
					x += 0.025;
				if (vars->sprites[i].spr_pos[Y] > (vars->player.pos[Y] / TILE_SIZE - 0.5))
					y -= 0.025;
				else
					y += 0.025;
				if (vars->map[(int)(round(vars->sprites[i].spr_pos[Y] + y +((y / fabs(y)) * 0.25f)))][(int)(round(vars->sprites[i].spr_pos[X]))] != '1' && vars->map[(int)(round(vars->sprites[i].spr_pos[Y] + y +((y / fabs(y)) * 0.25f)))][(int)(round(vars->sprites[i].spr_pos[X]))] != 'B')
					if (fabs((vars->sprites[i].spr_pos[Y] + y) - (vars->player.pos[Y] / TILE_SIZE - 0.5)) >= 0.1)
						vars->sprites[i].spr_pos[Y] += y;
				if (vars->map[(int)(round(vars->sprites[i].spr_pos[Y]))][(int)(round(vars->sprites[i].spr_pos[X] + x + ((x / fabs(x)) * 0.25f)))] != '1' && vars->map[(int)(round(vars->sprites[i].spr_pos[Y]))][(int)(round(vars->sprites[i].spr_pos[X] + x + ((x / fabs(x)) * 0.25f)))] != 'B')
					if (fabs((vars->sprites[i].spr_pos[X] + x) - (vars->player.pos[X] / TILE_SIZE - 0.5)) >= 0.1)
						vars->sprites[i].spr_pos[X] += x;

			}

		}
	}
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	mlx_destroy_image(vars->mlx.mlx, vars->mini_map.img);
	mlx_destroy_image(vars->mlx.mlx, vars->gun_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->sprites_canvas.img);
	mlx_destroy_image(vars->mlx.mlx, vars->ui_canvas.img);
	return (0);
}
