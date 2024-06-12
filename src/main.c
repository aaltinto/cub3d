/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:58:39 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/12 17:19:19 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../cub3d.h"
#include "../libft/libft.h"
#include "../minilibx/mlx.h"
#include <unistd.h>

int	parse_init(t_vars *vars, char *map)
{
	char	**map_tmp;
	char	*tmp;
	int		i;

	map_tmp = ft_split(map, '\n');
	if (!map_tmp)
		return (err("Split error"));
	i = -1;
	while (map_tmp[++i])
	{
		tmp = ft_strdup(map_tmp[i]);
		if (!tmp)
			return (free_doubles(map_tmp), err("Strdup error"));
		while (*tmp != '\0')
		{
			if ((*tmp != ' ' && *tmp != '\t') && (init_textures(vars, tmp)
					|| color_init(vars, tmp)))
				return (free_doubles(map_tmp), free(tmp), 1);
			tmp++;
		}
	}
	if (!vars->textures.north || !vars->textures.south || !vars->textures.west \
	|| !vars->textures.east || !vars->textures.floor || !vars->textures.ceiling)
		return (free_doubles(map_tmp), err("Error! Missing indicators"));
	return (free_doubles(map_tmp), 0);
}

int	marche(t_vars *vars)
{
	vars->map = NULL;
	vars->textures.ceiling = NULL;
	vars->textures.floor = NULL;
	vars->textures.north = NULL;
	vars->textures.south = NULL;
	vars->textures.east = NULL;
	vars->textures.west = NULL;
	return (0);
}

int	close_windows(t_vars *vars)
{
	mlx_destroy_window(vars->mlx.mlx, vars->mlx.win);
	free_doubles(vars->map);
	free(vars->textures.ceiling);
	free(vars->mlx.mlx);
	free(vars->textures.floor);
	free(vars->textures.north);
	free(vars->textures.south);
	free(vars->textures.east);
	free(vars->textures.west);
	free(vars->textures.ceiling_rgb);
	free(vars->textures.floor_rgb);
	system("leaks cub3d");
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
	if (marche(&vars) || read_map(argv, &vars))
		return (1);
	vars.mlx.mlx = mlx_init();
	if (!vars.mlx.mlx)
		return (err("Mlx init error"));
	vars.mlx.win = mlx_new_window(vars.mlx.mlx, 1920, 1080, "cub3d");
	if (!vars.mlx.win)
		return (err("Mlx window error"), close_windows(&vars), 1);
	printf("salam\n");
	if (fill_background(&vars))
		return (close_windows(&vars), 1);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_loop(vars.mlx.mlx);
	return (0);
}
