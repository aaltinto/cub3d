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

int	get_textures(t_vars *vars)
{
	int	x;
	int	y;

	x = 512;
	y = 512;
	vars->xpm = (t_data *)malloc(sizeof(t_data) * 4);
	if (!vars->xpm)
		return (err("Malloc error"), 1);
	vars->xpm[NO].img = mlx_xpm_file_to_image(vars->mlx.mlx,
		strip(vars->textures.north), &x, &y);
	if (!vars->xpm[NO].img)
		return (err("can't get texture 'NO'"));
	vars->xpm[NO].addr = mlx_get_data_addr(vars->xpm[NO].img,
		&(vars->xpm[NO].bits_per_pixel), &(vars->xpm[NO].line_length),
		&(vars->xpm[NO].endian));
	if (!vars->xpm[NO].addr)
		return (err("Get data addr error: 'NO'"));
	vars->xpm[SO].img = mlx_xpm_file_to_image(vars->mlx.mlx,
		strip(vars->textures.south), &x, &y);
	vars->xpm[SO].addr = mlx_get_data_addr(vars->xpm[SO].img,
		&(vars->xpm[SO].bits_per_pixel), &(vars->xpm[SO].line_length),
		&(vars->xpm[SO].endian));
	if (!vars->xpm[SO].addr)
		return (err("Get data addr error: 'SO'"));
	vars->xpm[WE].img = mlx_xpm_file_to_image(vars->mlx.mlx,
		strip(vars->textures.west), &x, &y);
	vars->xpm[WE].addr = mlx_get_data_addr(vars->xpm[WE].img,
		&vars->xpm[WE].bits_per_pixel, &vars->xpm[WE].line_length,
		&vars->xpm[WE].endian);
	if (!vars->xpm[WE].addr)
		return (err("Get data addr error: 'WE'"));
	vars->xpm[EA].img = mlx_xpm_file_to_image(vars->mlx.mlx,
		strip(vars->textures.east), &x, &y);
	vars->xpm[EA].addr = mlx_get_data_addr(vars->xpm[EA].img,
		&vars->xpm[EA].bits_per_pixel, &vars->xpm[EA].line_length,
		&vars->xpm[EA].endian);
	if (!vars->xpm[EA].addr)
		return (err("Get data addr error: 'EA'"));
	printf("data: %p\n", vars->xpm[NO].addr);
	return (0);
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
	vars->render.sc_height = 600;
	vars->render.sc_width = 840;
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
			pixel_put(&vars->img, i % vars->render.sc_width, i / vars->render.sc_width, color_celing);
		else
			pixel_put(&vars->img, i % vars->render.sc_width, i / vars->render.sc_width, color_floor);
	}
	return (0);
}

int	is_space(int c)
{
	return (c == 32);
}

char *strip(char *str) {
    char *end;

    // Trim leading space
	if (!str)
		return (err("string is null"), NULL);
    while (is_space((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + ft_strlen(str) - 1;
    while (end > str && is_space((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

void	draw_line(t_vars *vars, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
	int err = (dx > dy ? dx : -dy) / 2, e2;

    while (1)
    {
        pixel_put(&vars->mini_map, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void	draw_arrow(t_vars *vars, int centerX, int centerY, int length, double angle, int color)
{
	int	endX = centerX + (int)(length * cos(angle));
	int	endY = centerY + (int)(length * sin(angle));	

	draw_line(vars, centerX, centerY, endX, endY, color);
}

void	draw_circle(t_vars *vars, int color)
{
	int	x = TILE_SIZE*0.20f;
	int	y = 0;
	int	err = 0;

	while (x >= y)
	{
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f + x, vars->render.sc_height*0.10f + y, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f + y, vars->render.sc_height*0.10f + x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f - y, vars->render.sc_height*0.10f + x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f - x, vars->render.sc_height*0.10f + y, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f - x, vars->render.sc_height*0.10f - y, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f - y, vars->render.sc_height*0.10f - x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f + y, vars->render.sc_height*0.10f - x, color);
		pixel_put(&vars->mini_map, vars->render.sc_width*0.10f + x, vars->render.sc_height*0.10f - y, color);

		if (err <= 0)
		{
			y += 1;
			err += 2*y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2*x + 1;
		}
	}
	draw_arrow(vars, vars->render.sc_width*0.10f, vars->render.sc_height*0.10f, TILE_SIZE*0.20f*2, vars->player.p_angle, color);
}

int	render_mini_map(t_vars *vars)
{
	int	posX;
	int	posY;
	int	x;
	int	y;
	int	color;
	int	scale_width;
	int	scale_height;
	int	tile_x;
	int	tile_y;

	posX = (int)round(vars->player.posX / TILE_SIZE);
	posY = (int)round(vars->player.posY / TILE_SIZE);

	int	map_height = double_counter(vars->map);
	int	map_width = ft_strlen(vars->map[0]);

	scale_width = (int)(vars->render.sc_width * 0.20f) / MAP_TILE;
	scale_height = (int)(vars->render.sc_height * 0.20f) / MAP_TILE;

	y = 0;
	for (tile_y = posY - 5; tile_y <= posY + 5; tile_y++, y++)
	{
		if (tile_y < 0 || tile_y >= map_height) continue;

		x = 0;
		for (tile_x = posX - 5; tile_x <= posX + 5; tile_x++, x++)
		{
			if (tile_x < 0 || tile_x >= ft_strlen(vars->map[tile_y])) continue;

			if (vars->map[tile_y][tile_x] == '1' || vars->map[tile_y][tile_x] == ' ')
				color = rgb_to_hex(0, 0, 0);
			else
				color = rgb_to_hex(255, 255, 255);

			for (int j = 0; j < scale_height; j++)
			{
				for (int i = 0; i < scale_width; i++)
				{
					pixel_put(&(vars->mini_map), x * scale_width + i, y * scale_height + j, color);
				}
			}
		}
	}
	draw_circle(vars, rgb_to_hex(255, 0, 0));
	return (0);
}

int	render(void *ptr)
{
	t_vars *vars;
	int	x;
	int	y;

	vars = (t_vars *)ptr; 

	vars->mini_map.img = mlx_new_image(vars->mlx.mlx, vars->render.sc_width*0.20f, vars->render.sc_height*0.20f);
	vars->mini_map.addr = mlx_get_data_addr(vars->mini_map.img, &vars->mini_map.bits_per_pixel, &vars->mini_map.line_length,
								&vars->mini_map.endian);
	vars->img.img = mlx_new_image(vars->mlx.mlx, vars->render.sc_width, vars->render.sc_height);
	vars->img.addr = mlx_get_data_addr(vars->img.img, &vars->img.bits_per_pixel, &vars->img.line_length,
								&vars->img.endian);
	mlx_clear_window(vars->mlx.mlx, vars->mlx.win);
	if (fill_background(vars))
		return (close_windows(vars), 1);
	cast_rays(vars);
	render_mini_map(vars);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->img.img, 0, 0);
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.win, vars->mini_map.img, 0, 0);
	mlx_destroy_image(vars->mlx.mlx, vars->img.img);
	move_player(vars);
	//vars->player.p_angle = nor_angle(vars->player.p_angle);
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
			vars->player.posX = (x * TILE_SIZE) + TILE_SIZE/2;
			vars->player.posY = (y * TILE_SIZE) + TILE_SIZE/2;
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
	get_textures(&vars);
	mlx_hook(vars.mlx.win, 17, 0, close_windows, &vars);
	mlx_hook(vars.mlx.win, 02, 0, key_capture, &vars);
	mlx_hook(vars.mlx.win, 03, 0, key_release, &vars);
	mlx_loop_hook(vars.mlx.mlx, render, (void *)(&vars));
	//mlx_loop_hook(vars.mlx.mlx, move_player, &vars);
	mlx_loop(vars.mlx.mlx);
	// cast_rays(&vars);
	return (close_windows(&vars), 0);
}
