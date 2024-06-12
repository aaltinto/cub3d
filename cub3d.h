/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:29:45 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/11 22:29:46 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define ARG "Error! Invalid arguments.\nUsage: './cub3d *.cub'"

typedef struct s_textures
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	char	*floor;
	char	*ceiling;
}	t_textures;

typedef struct s_vars
{
	t_textures	textures;
	char		**map;
	char		*raw_map;
	int			*index;
}	t_vars;

//error
int	err(char *str);
int	read_map(char **argv, t_vars *vars);
int	double_counter(char **str);
char	**reallocate_double(char	***str);
int	find_longest_line(char **str);
int	free_doubles(char **str);
int	parse_init(t_vars *vars, char *map);

#endif