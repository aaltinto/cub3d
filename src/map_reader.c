/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 04:42:12 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/25 15:21:01 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include "../gnl/get_next_line.h"
#include "../libft/libft.h"
#include <fcntl.h>
#include <stdio.h>

int	fill_map(char **map, t_vars *vars)
{
	int	i;

	vars->map = (char **)malloc((double_counter(map) + 1) * sizeof(char *));
	i = -1;
	while (map[++i])
	{
		vars->map[i] = ft_strdup(map[i]);
		if (!vars->map[i])
			return (err("Strdup error"));
	}
	vars->map[i] = NULL;
	return (0);
}

static int	check_borders(char	**map, t_vars *vars)
{
	int	i;
	int	j;
	int	len;
	char	**tmp_map;

	tmp_map = reallocate_double(map);
	if (!map)
		return (err("Allocate error"), 1);
	len = find_longest_line(map);
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == '0' && (i == 0 || j == len -1 || (j < len
				&& (map[i][j + 1] == '\0' || map[i][j + 1] == ' '
				|| map[i][j + 1] == '\n')) || j == 0 || (j > 0
				&& map[i][j - 1] == ' ') || map[i + 1] == NULL
				|| (map[i + 1] != NULL && (map[i + 1][j] == '\0'
				|| map[i + 1][j] == ' ')) || (i != 0
				&& (map[i -1][j] == ' ' || map[i -1][j] == '\0'))))
				return (err("Error!\nMap should be closed by 1's"));
		}
	}
	return (fill_map(tmp_map, vars), free_doubles(tmp_map));
}

//check spaces and nulls to search borders. 0's cant touch spaces and nulls.
static int	check_raw_map(t_vars *vars, char *map)
{
	int	i;
	int	j;
	char	**splitted_map;

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
			return (err("Error! Invalid map"));
		if (map[i] == 'N' || map[i] == 'S' || map[i] == 'W' || map[i] == 'E')
			j++;
	}
	if (j > 1 || j == 0)
		return (err("Error!\nInvalid player"));
	splitted_map = ft_split(map, '\n');
	if (!splitted_map)
		return (err("Split error"));
	if (check_borders(splitted_map, vars))
		return ( 1);
	return (free_doubles(splitted_map), 0);
}

static int	split_map(char *map, t_vars *vars)
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
		if (j == 0 && (map[i] == '1' || map[i] == '0'))
			break ;
		j++;
		if (map[i] == '\n')
			j = 0;
		spc = 0;
	}
	vars->raw_map = ft_substr(map, i - spc + 1, ft_strlen(map));
	if (!vars->raw_map)
		return (err("Substr error"));
	if (check_raw_map(vars, vars->raw_map))
		return (null_free(vars->raw_map), 1);
	return (null_free(vars->raw_map), 0);
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
	if (split_map(map, vars))
		return (null_free(map), 1);
	if(parse_init(vars, map))
		return (null_free(map), 1);
	return (null_free(map), 0);
}
