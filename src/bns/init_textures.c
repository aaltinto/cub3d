/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:50:09 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/12 16:49:39 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/bonus.h"
#include "../../libft/libft.h"
#include "../../minilibx/mlx.h"

int	get_alp_sprites(t_vars *vars, int x, int y)
{
	char	*filename;
	char	*tmp;
	char	c[2];
	int		i;

	c[1] = '\0';
	vars->ui.alp = malloc(sizeof(t_data) * 26);
	if (!vars->ui.alp)
		return (err("Malloc error"));
	i = -1;
	while (++i < 26)
	{
		c[0] = i + 97;
		tmp = ft_strjoin("./alphabet/", c);
		filename = ft_strjoin(tmp, ".xpm");
		if (null_free(tmp), !filename)
			return (err("Strjoin error"));
		vars->ui.alp[i].img = mlx_xpm_file_to_image(vars->mlx.mlx, filename,
				&x, &y);
		if (null_free(filename), !vars->ui.alp[i].img)
			return (err("Can't find animation sprites"));
		vars->ui.alp[i].addr = mlx_get_data_addr(vars->ui.alp[i].img, \
		&vars->ui.alp[i].bits_per_pixel, &vars->ui.alp[i].line_length, \
		&vars->ui.alp[i].endian);
		if (!vars->ui.alp[i].addr)
			return (err("Get data addr error"));
	}
	return (0);
}

int	get_num_sprites(t_vars *vars, int x, int y)
{
	int		i;
	char	*filename;

	if (get_alp_sprites(vars, 8, 8))
		return (1);
	vars->ui.num = malloc(sizeof(t_data) * 10);
	if (!vars->ui.num)
		return (err("Malloc error"));
	i = -1;
	while (++i < 10)
	{
		filename = get_xpm_filename("./textures/num", i);
		if (!filename)
			return (1);
		vars->ui.num[i].img = mlx_xpm_file_to_image(vars->mlx.mlx, filename,
				&x, &y);
		if (null_free(filename), !vars->ui.num[i].img)
			return (err("Can't find animation sprites"));
		vars->ui.num[i].addr = mlx_get_data_addr(vars->ui.num[i].img, \
		&vars->ui.num[i].bits_per_pixel, &vars->ui.num[i].line_length, \
		&vars->ui.num[i].endian);
		if (!vars->ui.num[i].addr)
			return (err("Get data addr error"));
	}
	return (0);
}

static int	set_gun_data(t_vars *vars, int i, int j, char *filename)
{
	int	x;
	int	y;

	x = 64;
	y = 64;
	vars->gun[j][i].img = mlx_xpm_file_to_image(vars->mlx.mlx, filename, &x, &y);
	if (!vars->gun[j][i].img)
		return (err("Can't find animation sprites"));
	vars->gun[j][i].addr = mlx_get_data_addr(vars->gun[j][i].img, \
	&vars->gun[j][i].bits_per_pixel, &vars->gun[j][i].line_length, \
	&vars->gun[j][i].endian);
	if (!vars->gun[j][i].addr)
		return (err("Get data addr error"));
	return (0);
}

void	set_null(void ***thing, int count)
{
	int	i;

	i = -1;
	while (++i < count)
		(*thing)[i] = NULL;
}

int	get_magnum_sprites(t_vars *vars)
{
	int		i;
	int		j;
	int		ani_count;
	char	*filename;

	vars->gun = (t_data **)malloc(sizeof(t_data *) * 3);
	if (!vars->gun)
		return (err("Malloc error"));
	set_null((void ***)(&vars->gun), 3);
	j = -1;
	while (++j < 3)
	{
		ani_count = 0;
		if (j == 1)
			ani_count = 5;
		vars->gun[j] = (t_data *)malloc(sizeof(t_data) * (10 + ani_count));
		if (!vars->gun[j])
			return (err("Malloc error"));
		i = -1;
		while (++i < 10 + ani_count)
		{
			vars->gun[j][i].img = NULL;
			filename = get_xpm_filename(vars->gun_name[j], i + 1);
			if (!filename || set_gun_data(vars, i, j, filename))
				return (null_free(filename));
			null_free(filename);
		}
	}
	return (0);
}

int	get_walls(t_textures *textures, char *tmp, int dir)
{
	if (dir == NO && !textures->walls[NO])
		textures->walls[NO] = ft_substr(tmp, 2, ft_strlen(tmp));
	else if (dir == SO && !textures->walls[SO])
		textures->walls[SO] = ft_substr(tmp, 2, ft_strlen(tmp));
	else if (dir == WE && !textures->walls[WE])
		textures->walls[WE] = ft_substr(tmp, 2, ft_strlen(tmp));
	else if (dir == EA && !textures->walls[EA])
		textures->walls[EA] = ft_substr(tmp, 2, ft_strlen(tmp));
	else
		return (err("Error!\nDuplicating wall textures"));
	if (!textures->walls[dir])
		return (err("Substr error!"));
	return (0);
}

int	init_textures(t_textures *textures, char *tmp)
{
	int	ret;

	ret = -1;
	if (ft_strnstr(tmp, "NO ", 3) != NULL)
		ret = get_walls(textures, tmp, NO);
	else if (ft_strnstr(tmp, "SO ", 3) != NULL)
		ret = get_walls(textures, tmp, SO);
	else if (ft_strnstr(tmp, "WE ", 3) != NULL)
		ret = get_walls(textures, tmp, WE);
	else if (ft_strnstr(tmp, "EA ", 3) != NULL)
		ret = get_walls(textures, tmp, EA);
	if (ret == 0 || (*tmp == 'F' && (*(tmp + 1)) == ' ') || (*tmp == 'C'
			&& (*(tmp + 1)) == ' ') || *tmp == '0' || *tmp == '1')
		return (2);
	if (ret == 1)
		return (1);
	return (err("Invalid character: "), 1);
}
