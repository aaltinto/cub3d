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

static char	*get_xpm_filename(char *filename, int i)
{
	char	*tmp;
	char	*tmp2;
	char	*num;

	num = ft_itoa(i);
	if (!num)
		return (err("Itoa error"), NULL);
	ft_putendl_fd(num, 1);
	tmp = ft_strjoin(filename, num);
	if (null_free(num), !tmp)
		return (err("Strjoin err"), NULL);
	ft_putendl_fd(tmp, 1);
	tmp2 = ft_strjoin(tmp, ".xpm");
	if (null_free(tmp), !tmp2)
		return (err("Strjoin err"), NULL);
	return (tmp2);
}

int	get_num_sprites(t_vars *vars, int x, int y)
{

	int		i;
	char	*filename;

	i = -1;
	while (++i < 10)
	{
		filename = get_xpm_filename("./textures/num", i);
		if (!filename)
			return (1);
		vars->num[i].img = mlx_xpm_file_to_image(vars->mlx.mlx, filename,
				&x, &y);
		if (!vars->num[i].img)
			return (err("Can't find animation sprites"));
		vars->num[i].addr = mlx_get_data_addr(vars->num[i].img, \
		&vars->num[i].bits_per_pixel, &vars->num[i].line_length, \
		&vars->num[i].endian);
		if (!vars->num[i].addr)
			return (err("Get data addr error"));
		null_free(filename);
	}
	return (0);
}

int	get_magnum_sprites(t_vars *vars, int x, int y)
{
	int		i;
	int		j;
	int		ani_count;
	char	*filename;

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
			filename = get_xpm_filename(vars->gun_name[j], i + 1);
			if (!filename)
				return (1);
			vars->gun[j][i].img = mlx_xpm_file_to_image(vars->mlx.mlx, filename,
					&x, &y);
			if (!vars->gun[j][i].img)
				return (err("Can't find animation sprites"));
			vars->gun[j][i].addr = mlx_get_data_addr(vars->gun[j][i].img, \
			&vars->gun[j][i].bits_per_pixel, &vars->gun[j][i].line_length, \
			&vars->gun[j][i].endian);
			if (!vars->gun[j][i].addr)
				return (err("Get data addr error"));
			null_free(filename);
		}
	}
	return (0);
}

int	get_walls(t_textures *textures, char *tmp, int dir)
{
	if (dir == 0 && !textures->walls[NO])
		textures->walls[NO] = ft_substr(tmp, 2, ft_strlen(tmp));
	else if (dir == 1 && !textures->walls[SO])
		textures->walls[SO] = ft_substr(tmp, 2, ft_strlen(tmp));
	else if (dir == 2 && !textures->walls[WE])
		textures->walls[WE] = ft_substr(tmp, 2, ft_strlen(tmp));
	else if (dir == 3 && !textures->walls[EA])
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
