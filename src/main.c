/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:58:39 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/11 21:58:40 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../cub3d.h"
#include "../libft/libft.h"

int	parse_init(t_vars *vars, char *map)
{
	char	**map_tmp;
	int		i;
	int		j;
	//int		len;

	map_tmp = ft_split(map, '\n');
	if (!map_tmp)
		return (err("Split error"));
	//len = double_counter(map_tmp);
	i = -1;
	while (map_tmp[++i])
	{
		j = -1;
		while (map_tmp[i][++j])
		{
			if (map_tmp[i][j] == ' ' || map_tmp[i][j] == '\t')
				continue ;
			if (map_tmp[i][j] == 'N' && map_tmp[i][j + 1] == 'O' && map_tmp[i][j + 2] == ' ')
			{
				if (vars->textures.north)
					return (free_doubles(map_tmp), err("Duplicating indicators: 'NO'"));
				vars->textures.north = ft_substr(map_tmp[i], j + 2, ft_strlen(map_tmp[i]));
				if (!vars->textures.north)
					return (free_doubles(map_tmp), err("Substr error"));
			}
			if (map_tmp[i][j] == 'S' && map_tmp[i][j + 1] == 'O' && map_tmp[i][j + 2] == ' ')
			{
				if (vars->textures.south)
					return (free_doubles(map_tmp), err("Duplicating indicators: 'SO'"));
				vars->textures.south = ft_substr(map_tmp[i], j + 2, ft_strlen(map_tmp[i]));
				if (!vars->textures.south)
					return (free_doubles(map_tmp), err("Substr error"));
			}
			if (map_tmp[i][j] == 'W' && map_tmp[i][j + 1] == 'E' && map_tmp[i][j + 2] == ' ')
			{
				if (vars->textures.west)
					return (free_doubles(map_tmp), err("Duplicating indicators: 'WE'"));
				vars->textures.west = ft_substr(map_tmp[i], j + 2, ft_strlen(map_tmp[i]));
				if (!vars->textures.west)
					return (free_doubles(map_tmp), err("Substr error"));
			}
			if (map_tmp[i][j] == 'E' && map_tmp[i][j + 1] == 'A' && map_tmp[i][j + 2] == ' ')
			{
				if (vars->textures.east)
					return (free_doubles(map_tmp), err("Duplicating indicators: 'WE'"));
				vars->textures.east = ft_substr(map_tmp[i], j + 2, ft_strlen(map_tmp[i]));
				if (!vars->textures.east)
					return (free_doubles(map_tmp), err("Substr error"));
			}
			if (map_tmp[i][j] == 'F' && map_tmp[i][j + 1] == ' ')
			{
				if (vars->textures.floor)
					return (free_doubles(map_tmp), err("Duplicating indicators: 'F'"));
				vars->textures.floor = ft_substr(map_tmp[i], j + 1, ft_strlen(map_tmp[i]));
				if (!vars->textures.floor)
					return (free_doubles(map_tmp), err("Substr error"));
			}
			if (map_tmp[i][j] == 'C' && map_tmp[i][j + 1] == ' ')
			{
				if (vars->textures.ceiling)
					return (free_doubles(map_tmp), err("Duplicating indicators: 'C'"));
				vars->textures.ceiling = ft_substr(map_tmp[i], j + 1, ft_strlen(map_tmp[i]));
				if (!vars->textures.ceiling)
					return (free_doubles(map_tmp), err("Substr error"));
			}

		}
	}
	free_doubles(map_tmp);
	// printf("%s\n", vars->textures.ceiling);
	// printf("%s\n", vars->textures.north);
	// printf("%s\n", vars->textures.floor);
	// printf("%s\n", vars->textures.south);
	// printf("%s\n", vars->textures.east);
	// printf("%s\n", vars->textures.west);
	return (0);
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

int	main(int ac, char **argv)
{
	t_vars	vars;

	if (ac != 2)
		return (err(ARG));
	if (marche(&vars) || read_map(argv, &vars))
		return (1);
	if (vars.map)
		for (int i = 0; vars.map[i]; i++)
			printf("%s\n", vars.map[i]);
	return (0);
}
