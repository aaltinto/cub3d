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

void	abort_mission(t_vars *vars)
{
	int	i;

	null_free(vars->textures.ceiling);
	null_free(vars->textures.floor);
	null_free(vars->textures.walls[0]);
	null_free(vars->textures.walls[1]);
	null_free(vars->textures.walls[2]);
	null_free(vars->textures.walls[3]);
	null_free(vars->sprites);
	free_doubles((char **)vars->enemy.sprites);
	free_doubles(vars->map);
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
	}
	i = -1;
	while (++i < 15)
		mlx_destroy_image(vars->mlx.mlx, vars->gun[1][i].img);
	null_free(vars->gun[0]);
	null_free(vars->gun[1]);
	null_free(vars->gun[2]);
}

int	close_windows(t_vars *vars)
{
	int	i;

	i = -1;
	while (++i < 26)
		mlx_destroy_image(vars->mlx.mlx, vars->alp[i].img);
	i = -1;
	while (++i < 10)
		mlx_destroy_image(vars->mlx.mlx, vars->num[i].img);
	abort_mission(vars);
	mlx_destroy_window(vars->mlx.mlx, vars->mlx.win);
	null_free(vars->mlx.mlx);
	exit(0);
	return (0);
}
