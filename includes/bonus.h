/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 22:29:45 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/13 01:33:23 by aaltinto         ###   ########.fr       */
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
# define SHIFT 257
# define SPACE 49

# define M_PI 3.14159265358979323846

# define TILE_SIZE 40
# define MAP_TILE 20
# define TILE_GUN 9
# define PLAYER_SPEED 5.0f

# define X 0
# define Y 1
# define TOP 1
# define BOT 0

# define NO 0
# define SO 1
# define WE 2
# define EA 3

#include <stdio.h>

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
	double	pos[2];
	double	camera[2];
	double	fov;
	double	p_angle;
	double	plane[2];
	double	dir[2];
	double	running;
	int		shoot;
	int		ani_i;
	int		gun_type;
}	t_player;

typedef struct s_textures
{
	char	*walls[5];
	char	*floor;
	int		floor_rgb[3];
	char	*ceiling;
	int		ceiling_rgb[3];
}	t_textures;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
}	t_mlx;

typedef struct s_keys
{
	int		key_w;
	int		key_a;
	int		key_s;
	int		key_d;
	int		key_la;
	int		key_ra;
}	t_keys;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_sprite
{
	double	spr_pos[2];
	t_data	sprite;
}	t_sprite;

typedef struct s_vars
{
	t_textures	textures;
	t_mlx		mlx;
	t_data		img;
	t_data		mini_map;
	t_data		sprites_canvas;
	t_data		sprite;
	t_data		xpm[4];
	t_data		*gun[3];
	t_data		num[10];
	t_data		alp[26];
	t_data		gun_canvas;
	t_sprite	*sprites;
	t_data		ui_canvas;
	t_data		map_arrow;
	t_player	player;
	t_render	render;
	t_keys		keys;
	char		**map;
	char		**gun_name;
	int			map_w;
	int			map_h;
	int			ammo;
	int			spr_count;
	int			menu;
	size_t		s_time;
	char		*raw_map;
	double		fov_angle;
	int			*map_grid;
}	t_vars;

typedef struct s_ray
{
	double	side_dist[2];
	double	raydir[2];
	double	delta_dist[2];
	int		step[2];
	int		side;
}	t_ray;


typedef struct s_args
{
	int		original_width;
	int		original_height;
	double	tile_size;
	int		pos_x;
	int		pos_y;
}	t_img_args;


//error
int			err(char *str);
int			null_free(void *ptr);
void		abort_mission(t_vars *vars);
int			read_map(char **argv, t_vars *vars);
int			double_counter(char **str);
char		**reallocate_double(char **str);
int			find_longest_line(char **str);
int			free_doubles(char **str);
int			parse_init(t_vars *vars, char *map);
int			init_textures(t_textures *textures, char *tmp);
int			extract_rgb(t_vars *vars);
int			color_init(t_vars *vars, char *tmp);
int			cast_rays(t_vars *vars);
int			rgb_to_hex(int r, int g, int b);
void		pixel_put(t_data *data, int x, int y, int color);
double		nor_angle(double angle);
int			key_capture(int keycode, t_vars *vars);
int			key_release(int keycode, t_vars *vars);
int			move_player(t_vars *vars, double x, double y);
int			close_windows(t_vars *vars);
char		*strip(char *str);
int			render_mini_map(t_vars *vars);
int			get_color(t_vars *vars, int flag, int x, int y);
int			render(void *ptr);
int			fill_variable(t_vars *vars, t_ray *ray);
int			detect_player(t_vars *vars);
int			get_canvas(t_vars *vars);
int			texture_color(t_data *data, int x, int y);
int			get_magnum_sprites(t_vars *vars);
int			get_num_sprites(t_vars *vars, int x, int y);
size_t		get_time(void);
int			mouse_func(int button, int x, int y, t_vars *vars);
int			detect_player(t_vars *vars);
int			get_textures(t_vars *vars);
int			mouse_move(t_vars *vars);
int			render_gun(t_vars *vars);
void		calculate_ammo_count(t_vars *vars, double pos_tile);
void		render_ui(t_vars *vars);
t_data		*fill_t_data(t_data *data, t_vars *vars, int width, int height);
char		*get_xpm_filename(char *filename, int i);
int			cast_spr(t_vars *vars, double *ddist);
t_sprite	*detect_barrels(t_vars *vars);
double		euclid_dist(double *cam, double *pos2);
void		scale_up_image(t_data *data, t_data canvas, t_img_args args);

#endif