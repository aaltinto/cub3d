/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:58:39 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/25 16:31:38 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../cub3d.h"
#include "../libft/libft.h"
#include "../minilibx/mlx.h"
#include <unistd.h>
#include <math.h>

static int	detect_missing_ind(t_vars *vars)
{
	if (!vars->textures.north)
		err("North");
	if (!vars->textures.south)
		err("South");
	if (!vars->textures.west)
		err("West");
	if (!vars->textures.east)
		err("East");
	if (!vars->textures.floor)
		err("Floor");
	if (!vars->textures.ceiling)
		err("Ceiling");
	return (1);
}

int	parse_init(t_vars *vars, char *map)
{
	char	**map_tmp;
	char	*tmp;
	char	*tmp2;
	int		i;
	int		ret;

	map_tmp = ft_split(map, '\n');
	if (!map_tmp)
		return (err("Split error"));
	i = -1;
	while (map_tmp[++i])
	{
		tmp = ft_strdup(map_tmp[i]);
		if (!tmp)
			return (free_doubles(map_tmp), err("Strdup error"));
		tmp2 = tmp;
		while (*tmp != '\0')
		{
			if (*tmp != ' ' && *tmp != '\t')
			{
				if (color_init(vars, tmp))
					return (free_doubles(map_tmp), null_free(tmp2), 1);
				ret = init_textures(vars, tmp);
				if (ret == 2)
					break ;
				if (ret == 1)
					return (free_doubles(map_tmp), null_free(tmp2), 1);
			}
			tmp++;
		}
		null_free(tmp2);
	}
	if (!vars->textures.north || !vars->textures.south || !vars->textures.west \
	|| !vars->textures.east || !vars->textures.floor || !vars->textures.ceiling)
		return (free_doubles(map_tmp), err("Error! Missing indicators:"),
			detect_missing_ind(vars));
	return (free_doubles(map_tmp), 0);
}

int	marche(t_vars *vars)
{
	vars->map = NULL;
	vars->textures.ceiling_rgb = NULL;
	vars->textures.floor_rgb = NULL;
	vars->textures.ceiling = NULL;
	vars->textures.floor = NULL;
	vars->textures.north = NULL;
	vars->textures.south = NULL;
	vars->textures.east = NULL;
	vars->textures.west = NULL;
	vars->mlx.mlx = NULL;
	vars->mlx.win = NULL;
	vars->render.sc_height = 768;
	vars->render.sc_width = 1024;
	vars->render.flag = 0;
	vars->textures.ceiling_rgb = (int *)malloc(sizeof(int) * 3);
	if (!vars->textures.ceiling_rgb)
		return (err("Malloc error"));
	vars->textures.floor_rgb = (int *)malloc(sizeof(int) * 3);
	if (!vars->textures.floor_rgb)
		return (err("Malloc error"));
	return (0);
}

int	null_free(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
	return (1);
}

void	abort_mission(t_vars *vars)
{
	//free_doubles(vars->map);
	null_free(vars->textures.ceiling);
	null_free(vars->textures.floor);
	null_free(vars->textures.north);
	null_free(vars->textures.south);
	null_free(vars->textures.east);
	null_free(vars->textures.west);
	null_free(vars->textures.ceiling_rgb);
	null_free(vars->textures.floor_rgb);
}

int	close_windows(t_vars *vars)
{
	mlx_destroy_window(vars->mlx.mlx, vars->mlx.win);
	null_free(vars->mlx.mlx);
	abort_mission(vars);
	//system("leaks cub3d");
	exit(0);
}

int	rgb_to_hex(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

void	move_player(t_vars *vars, double new_x, double new_y)
{
	int	grid_x;
	int	grid_y;

	grid_x = (int)round(vars->player.posX + new_x)/TILE_SIZE;
	grid_y = (int)round(vars->player.posY + new_y)/TILE_SIZE;
	ft_putnbr_fd(grid_x, 1);
	write(1, "\n", 1);
	ft_putnbr_fd(grid_y, 1);
	write(1, "\n", 1);
	if (vars->map[grid_y][grid_x] == '1')
		return ;
	vars->player.posX = (vars->player.posX/TILE_SIZE + new_x) * TILE_SIZE;
	vars->player.posY = (vars->player.posY/TILE_SIZE + new_y) * TILE_SIZE;
}

int	key_capture(int keycode, t_vars *vars)
{
	double	new_x;
	double	new_y;

	if (keycode == ESC)
		close_windows(vars);
	if (keycode == ARROW_R)
		vars->player.p_angle += 0.2f;
	if (keycode == ARROW_L)
		vars->player.p_angle -= 0.2f;
	if (keycode == W && vars->map[((int)vars->player.posY/30) + 1][((int)vars->player.posX/30)] != '1')
	{
		new_x = cos(vars->player.p_angle) * PLAYER_SPEED;
		new_y = sin(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (keycode == S && vars->map[((int)vars->player.posY/30) - 1][((int)vars->player.posX/30)] != '1')
	{
		new_x = -cos(vars->player.p_angle) * PLAYER_SPEED;
		new_y = -sin(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (keycode == A && vars->map[((int)vars->player.posY/30)][((int)vars->player.posX/30) - 1] != '1')
	{
		new_x = sin(vars->player.p_angle) * PLAYER_SPEED;
		new_y = -cos(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (keycode == D && vars->map[((int)vars->player.posY/30)][((int)vars->player.posX/30) + 1] != '1')
	{
		new_x = -sin(vars->player.p_angle) * PLAYER_SPEED;
		new_y = cos(vars->player.p_angle) * PLAYER_SPEED;
	}
	move_player(vars, new_x, new_y);
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
		if (i < vars->render.sc_width * vars->render.sc_height/2)
			pixel_put(&vars->img, i % vars->render.sc_width, i / vars->render.sc_width, color_celing);
		else
			pixel_put(&vars->img, i % vars->render.sc_width, i / vars->render.sc_width, color_floor);
	}
	return (0);
}

void strip(char* source)
{
    char* i = source;
    char* j = source;

    while (*j != '\0') {
        *i = *j++;
        if (*i != ' ') {
            i++;
        }
    }
    *i = '\0';
}

int	render(void *ptr)
{
	t_vars *vars;
	int	x;
	int	y;

	vars = (t_vars *)ptr; 

	vars->img.img = mlx_new_image(vars->mlx.mlx, vars->render.sc_width, vars->render.sc_height);
	vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bits_per_pixel, &vars->img.line_length,
								&vars->img.endian);
	mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
	if (fill_background(vars))
		return (close_windows(vars), 1);
	cast_rays(vars);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	vars->player.p_angle = nor_angle(vars->player.p_angle);
	return (0);
}

int	detect_player(t_vars *vars)
{
	int	x;
	int	y;

	y = -1;
	while (vars->map[++y] != NULL)
	{
		x = -1;
		while (vars->map[y][++x])
		{
			vars->player.posX = x * TILE_SIZE;
			vars->player.posY = y * TILE_SIZE;
			if (vars->map[y][x] == 'N')
				return (vars->player.p_angle = (3*M_PI)/2, 0);
			else if (vars->map[y][x] == 'E')
				return (vars->player.p_angle = 0, 0);
			else if (vars->map[y][x] == 'S')
				return (vars->player.p_angle = M_PI/2, 0);
			else if (vars->map[y][x] == 'W')
				return (vars->player.p_angle = M_PI, 0);
		}
	}
	return (1);
}

int	main(int ac, char **argv)
{
	t_vars	vars;

	if (ac != 2)
		return (err(ARG));
	if (marche(&vars))
		return (abort_mission(&vars), 1);
	if (read_map(argv, &vars))
		return (abort_mission(&vars), 1);
	if (detect_player(&vars))
		return (abort_mission(&vars), 1);
	vars.mlx.mlx = mlx_init();
	if (!vars.mlx.mlx)
		return (err("Mlx init error"));
	vars.mlx.win = mlx_new_window(vars.mlx.mlx, vars.render.sc_width, vars.render.sc_height, "cub3d");
	if (!vars.mlx.win)
		return (err("Mlx window error"), close_windows(&vars), 1);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_loop_hook(vars.mlx.mlx, render, (void *)(&vars));
	mlx_loop(vars.mlx.mlx);
	return (close_windows(&vars), 0);
}
