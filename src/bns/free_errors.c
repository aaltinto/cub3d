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
#include <signal.h>
#include <unistd.h>

int	err(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

int	free_doubles2(void **str, int size)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (++i < size)
		null_free(str[i]);
	null_free(str);
	return (0);
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

void	abort_mission(t_vars *vars, int close, int child)
{
	null_free(vars->textures.ceiling);
	null_free(vars->textures.floor);
	null_free(vars->textures.walls[0]);
	null_free(vars->textures.walls[1]);
	null_free(vars->textures.walls[2]);
	null_free(vars->textures.walls[3]);
	null_free(vars->sprites);
	if (close)
		free_doubles(vars->map);
	free_doubles2((void **)vars->gun_name, 3);
	free_doubles2((void **)vars->ui.sound, 3);
	free_doubles2((void **)vars->enemy.filename, 5);
	null_free(vars->ui.ammo);
	(void)child;
	// if (!child)
	// 	system("leaks cub3d");
}

int	close_windows(t_vars *vars, int close, int child)
{
	int	i;
	int	j;
	int	det;

	i = 0;
	det = 0;
	if (!child)
	{
		kill(vars->game.pid + 1, SIGKILL);
		sleep(1);
	}
	i = -1;
	while (vars->gun && ++i < 3 && !det)
	{
		j = -1;
		while (++j < 15 && !det)
		{
			if (vars->gun[i][j].img == NULL || !vars->gun[i])
			{
				det = 1;
				break ;
			}
			if (i != 1 && j > 9)
				break ;
			mlx_destroy_image(vars->mlx.mlx, vars->gun[i][j].img);
		}
	}
	i = -1;
	while (++i < 4)
	{
		mlx_destroy_image(vars->mlx.mlx, vars->ui.healt_bar[i].img);
		mlx_destroy_image(vars->mlx.mlx, vars->xpm[i].img);
	}
	free_doubles2((void **)vars->gun, 3);
	if (vars->ui.map_arrow.img)
		mlx_destroy_image(vars->mlx.mlx, vars->ui.map_arrow.img);
	i = -1;
	while (++i < vars->game.spr_count)
		mlx_destroy_image(vars->mlx.mlx, vars->sprites[i].sprite.img);
	i = -1;
	while (vars->enemy.sprites && ++i < 5)
	{
		j = -1;
		while (++j < vars->enemy.index[i])
			mlx_destroy_image(vars->mlx.mlx, vars->enemy.sprites[i][j].img);
		null_free(vars->enemy.sprites[i]);
	}
	null_free(vars->enemy.sprites);
	abort_mission(vars, close, child);
	mlx_destroy_window(vars->mlx.mlx, vars->mlx.win);
	if (!close)
		return (0);
	exit(0);
	return (0);
}
