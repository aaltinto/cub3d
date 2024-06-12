/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:29:45 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/12 17:22:43 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define ARG "Error! Invalid arguments.\nUsage: './cub3d *.cub'"

# define ESC 53
# define W 13
# define A 0
# define S 1
# define D 2
# define ARROW_L 123
# define ARROW_R 124
# define ARROW_DOWN 125
# define ARROW_UP 126

typedef struct s_textures
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	char	*floor;
	int		*floor_rgb;
	char	*ceiling;
	int		*ceiling_rgb;
}	t_textures;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
}	t_mlx;

typedef struct s_vars
{
	t_textures	textures;
	t_mlx		mlx;
	char		**map;
	char		*raw_map;
}	t_vars;

//error
int		err(char *str);
int		read_map(char **argv, t_vars *vars);
int		double_counter(char **str);
char	**reallocate_double(char	***str);
int		find_longest_line(char **str);
int		free_doubles(char **str);
int		free_doubles2(void **str, int len);
int		parse_init(t_vars *vars, char *map);
int		init_textures(t_vars *vars, char *tmp);
int		extract_rgb(t_vars *vars);
int		color_init(t_vars *vars, char *tmp);

#endif