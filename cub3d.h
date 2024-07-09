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

# define FOV 66 * (M_PI / 180)
# define TILE_SIZE 30
# define PLAYER_SPEED 0.05f

#include "minilibx/mlx.h"

typedef struct s_render
{
	int		sc_width;
	int		sc_height;
	double	ray_angle;
	double	ray_dist;
	int		flag;
}	t_render;

typedef struct s_player
{
	double	posX;
	double	posY;
	double	fov;
	double	p_angle;
}	t_player;

typedef struct s_textures
{
	void	*north_img;
	void	*west_img;
	void	*south_img;
	void	*east_img;
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

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_vars
{
	t_textures	textures;
	t_mlx		mlx;
	t_data		img;
	t_data		mini_map;
	t_player	player;
	t_render	render;
	char		**map;
	char		*raw_map;
}	t_vars;

//error
int		err(char *str);
int		null_free(void *ptr);
void	abort_mission(t_vars *vars);
int		read_map(char **argv, t_vars *vars);
int		double_counter(char **str);
char	**reallocate_double(char **str);
int		find_longest_line(char **str);
int		free_doubles(char **str);
int		free_doubles2(void **str, int len);
int		parse_init(t_vars *vars, char *map);
int		init_textures(t_vars *vars, char *tmp);
int		extract_rgb(t_vars *vars);
int		color_init(t_vars *vars, char *tmp);
void	cast_rays(t_vars *vars);
int	rgb_to_hex(int r, int g, int b);
void	pixel_put(t_data *data, int x, int y, int color);
double nor_angle(double angle);

#endif