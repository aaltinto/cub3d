#include "../cub3d.h"
#include "../libft/libft.h"
#include <math.h>

int	move_player(t_vars *vars)
{
	double	new_x;
	double	new_y;

	new_x = 0;
	new_y = 0;
	if (vars->keys.keyRA)
		vars->player.p_angle += 0.02f;
	if (vars->keys.keyLA)
		vars->player.p_angle -= 0.02f;
	if (vars->keys.keyW)
	{
		new_x += cos(vars->player.p_angle) * PLAYER_SPEED;
		new_y += sin(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (vars->keys.keyS)
	{
		new_x += -cos(vars->player.p_angle) * PLAYER_SPEED;
		new_y += -sin(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (vars->keys.keyA)
	{
		new_x += sin(vars->player.p_angle) * PLAYER_SPEED;
		new_y += -cos(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (vars->keys.keyD)
	{
		new_x += -sin(vars->player.p_angle) * PLAYER_SPEED;
		new_y += cos(vars->player.p_angle) * PLAYER_SPEED;
	}
	if (double_counter(vars->map) < ((vars->player.posY + new_y) / TILE_SIZE) || vars->map[(int)((vars->player.posY + new_y)/TILE_SIZE)][(int)(vars->player.posX/TILE_SIZE)] == '1')
		new_y = 0;
	if (ft_strlen(vars->map[(int)(vars->player.posY/TILE_SIZE)]) < ((vars->player.posX + new_x) / TILE_SIZE) || vars->map[(int)(vars->player.posY/TILE_SIZE)][(int)((vars->player.posX + new_x)/TILE_SIZE)] == '1')
		new_x = 0;
	vars->player.posX += new_x;
	vars->player.posY += new_y;
	return (0);
}

int	key_release(int keycode, t_vars *vars)
{
	if (keycode == ARROW_R)
		vars->keys.keyRA = 0;
	if (keycode == ARROW_L)
		vars->keys.keyLA = 0;
	if (keycode == W)
		vars->keys.keyW = 0;
	if (keycode == S)
		vars->keys.keyS = 0;
	if (keycode == A)
		vars->keys.keyA = 0;
	if (keycode == D)
		vars->keys.keyD = 0;
	return (0);
}



int	key_capture(int keycode, t_vars *vars)
{
	double	new_x;
	double	new_y;

	if (keycode == ESC)
		close_windows(vars);
	if (keycode == ARROW_R)
		vars->keys.keyRA = 1;
	if (keycode == ARROW_L)
		vars->keys.keyLA = 1;
	if (keycode == W)
		vars->keys.keyW = 1;
	if (keycode == S)
		vars->keys.keyS = 1;
	if (keycode == A)
		vars->keys.keyA = 1;
	if (keycode == D)
		vars->keys.keyD = 1;
	return (0);
}
