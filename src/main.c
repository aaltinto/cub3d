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
	vars->index = 0;
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
	if (vars->map)
		free_doubles(vars->map);
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
	exit(0);
}

int	rgb_to_hex(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

int	key_capture(int keycode, t_vars *vars)
{
	if (keycode == ESC)
		close_windows(vars);
	printf("Keycode: %d\n", keycode);
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
	while (++i < 1920 * 1080)
	{
		if (i < 1920 * 540)
			mlx_pixel_put(vars->mlx.mlx, vars->mlx.win, i % 1920, \
			i / 1920, color_celing);
		else
			mlx_pixel_put(vars->mlx.mlx, vars->mlx.win, i % 1920, \
			i / 1920, color_floor);
	}
	return (0);
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
	vars.mlx.mlx = mlx_init();
	if (!vars.mlx.mlx)
		return (err("Mlx init error"));
	vars.mlx.win = mlx_new_window(vars.mlx.mlx, 1920, 1080, "cub3d");
	if (!vars.mlx.win)
		return (err("Mlx window error"), close_windows(&vars), 1);
	if (fill_background(&vars))
		return (close_windows(&vars), 1);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_loop(vars.mlx.mlx);
	return (close_windows(&vars), 0);
}
