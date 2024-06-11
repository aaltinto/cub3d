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
#include "../gnl/get_next_line.h"
#include "../libft/libft.h"

int	err(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

int	double_counter(char **str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[i])
		++i;
	return (i);
}

int	free_doubles(char **str)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);	
	return (0);
}

int	parse_init(t_vars *vars, char *map)
{
	char	**map_tmp;
	int		i;
	int		j;
	//int		len;

	map_tmp = ft_split(map, '\n');
	if (!map_tmp)
		return (err("Split error"));
	for (int i = 0; map_tmp[i]; i++)
		printf("%s\n", map_tmp[i]);
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
	printf("%s\n", vars->textures.ceiling);
	printf("%s\n", vars->textures.north);
	printf("%s\n", vars->textures.floor);
	printf("%s\n", vars->textures.south);
	printf("%s\n", vars->textures.east);
	printf("%s\n", vars->textures.west);
	return (0);
}

int	check_raw_map(t_vars *vars, char *map)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (map[++i])
	{
		if (map[i] == ' ' || map[i] == '\t')
			continue ;
		if (map[i + 1] != '\0' && map[i] == '\n'
			&& map[i + 1] == '\n')
			return (err("Error!\nConsecutive newlines in the map"));
		if (map[i] != '0' && map[i] != '1' && map[i] != '\n' && map[i] != 'N'
			&& map[i] != 'S' && map[i] != 'W' && map[i] != 'E')
			return (printf("%c\n", map[i]), err("Error! Invalid map"));
		if (map[i] == 'N' || map[i] == 'S' || map[i] == 'W' || map[i] == 'E')
			j++;
	}
	//check spaces and nulls to search borders. 0's cant touch spaces and nulls.
	if (j > 1 || j == 0)
		return (err("Error!\nInvalid player"));
	vars->map = ft_split(map, '\n');
	return (0);
}

int	split_map(char *map, t_vars *vars)
{
	int	i;
	int	j;
	int	spc;

	i = -1;
	j = 0;
	spc = 0;
	while (map[++i])
	{
		spc++;
		if (map[i] == ' ' || map[i] == '\t')
			continue ;
		if (j == 0 && map[i] == '1')
			break;
		j++;
		if (map[i] == '\n')
			j = 0;
		spc = 0;
	}
	vars->raw_map = ft_substr(map, i - spc + 1, ft_strlen(map));
	if (!vars->raw_map)
		return (err("Substr error"));
	if (check_raw_map(vars, vars->raw_map))
		return (1);
	return (0);
}

int	read_map(char **argv, t_vars *vars)
{
	size_t	len;
	int		fd;
	char	*map;

	(void)vars;
	len = ft_strlen(argv[1]) - 1;
	if (argv[1][len] != 'b' || argv[1][len -1] != 'u' || argv[1][len -2] != 'c'
		|| argv[1][len -3] != '.')
		return (err(ARG));
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Can't open the file"), 1);
	map = get_next_line(fd);
	if (!map)
		return (err("Error while reading the file"));
	split_map(map, vars);
	// if (parse_init(vars, map))
	// 	return (1);
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
