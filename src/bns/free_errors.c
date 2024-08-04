/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 04:41:58 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/13 02:16:53 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/bonus.h"
#include "../../minilibx/mlx.h"

int	err(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

int	free_doubles(char **str)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (str[++i])
		null_free(str[i]);
	null_free(str);
	return (0);
}

int	null_free(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
	return (1);
}

void	abort_mission(t_vars *vars, int close)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 5)
	{
		j = -1;
		while (++j < vars->enemy.index[i])
			mlx_destroy_image(vars->mlx.mlx, vars->enemy.sprites[i][j].img);
		null_free(vars->enemy.sprites[i]);
	}
	null_free(vars->textures.ceiling);
	null_free(vars->textures.floor);
	null_free(vars->textures.walls[0]);
	null_free(vars->textures.walls[1]);
	null_free(vars->textures.walls[2]);
	null_free(vars->textures.walls[3]);
	i = -1;
	while (++i < vars->spr_count)
		mlx_destroy_image(vars->mlx.mlx, vars->sprites[i].sprite.img);
	null_free(vars->sprites);
	null_free(vars->enemy.sprites);
	if (close)
		free_doubles(vars->map);
	free_doubles(vars->gun_name);
	null_free(vars->enemy.filename[0]);
	null_free(vars->enemy.filename[1]);
	null_free(vars->enemy.filename[2]);
	null_free(vars->enemy.filename[3]);
	null_free(vars->enemy.filename[4]);
	i = -1;
	while (++i < 10)
	{
		mlx_destroy_image(vars->mlx.mlx, vars->gun[0][i].img);
		mlx_destroy_image(vars->mlx.mlx, vars->gun[2][i].img);
		if (i < 4)
		{
			mlx_destroy_image(vars->mlx.mlx, vars->ui.healt_bar[i].img);
			mlx_destroy_image(vars->mlx.mlx, vars->xpm[i].img);
		}
	}
	i = -1;
	while (++i < 15)
		mlx_destroy_image(vars->mlx.mlx, vars->gun[1][i].img);
	mlx_destroy_image(vars->mlx.mlx, vars->map_arrow.img);
	null_free(vars->gun[0]);
	null_free(vars->gun[1]);
	null_free(vars->gun[2]);
}

int	close_windows(t_vars *vars, int close)
{
	int	i;

	i = -1;
	while (++i < 26)
		mlx_destroy_image(vars->mlx.mlx, vars->alp[i].img);
	i = -1;
	while (++i < 10)
		mlx_destroy_image(vars->mlx.mlx, vars->num[i].img);
	abort_mission(vars, close);
	mlx_destroy_window(vars->mlx.mlx, vars->mlx.win);
	null_free(vars->mlx.mlx);
	if (!close)
		return (0);
	exit(0);
	return (0);
}
