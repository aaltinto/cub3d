/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:50:09 by aaltinto          #+#    #+#             */
/*   Updated: 2024/08/12 15:08:58 by aaltinto         ###   ########.fr       */
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
	int		len[2];

	c[1] = '\0';
	i = -1;
	while (++i < 26)
	{
		c[0] = i + 97;
		tmp = ft_strjoin("./alphabet/", c);
		filename = ft_strjoin(tmp, ".xpm");
		if (null_free(tmp), !filename)
			return (err("Strjoin error"));
		set_len(len, x, y);
		if (xpm_file_init(vars, &vars->ui.alp[i], filename, len))
			return (null_free(filename), 1);
		null_free(filename);
	}
	return (0);
}

int	get_num_sprites(t_vars *vars, int x, int y)
{
	int		i;
	int		len[2];
	char	*filename;

	if (get_alp_sprites(vars, 8, 8))
		return (1);
	i = -1;
	while (++i < 10)
	{
		filename = get_xpm_filename("./textures/num", i);
		if (!filename)
			return (1);
		set_len(len, x, y);
		if (xpm_file_init(vars, &vars->ui.num[i], filename, len))
			return (null_free(filename), 1);
		null_free(filename);
	}
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
	int		len[2];
	int		ani_count;
	char	*filename;

	vars->gun = (t_data **)ft_calloc(sizeof(t_data *), 3);
	if (!vars->gun)
		return (err("Malloc error"));
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
			if (set_len(len, 64, 64), !filename)
				return (1);
			if (xpm_file_init(vars, &vars->gun[j][i], filename, len))
				return (null_free(filename), 1);
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
