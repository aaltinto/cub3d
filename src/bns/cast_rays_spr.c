/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 23:05:03 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/12 23:05:04 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/bonus.h"
#include "../../libft/libft.h"
#include "../../minilibx/mlx.h"
#include <math.h>
#include <stdlib.h>

double	euclid_dist(double *cam, double *pos2)
{
	return (sqrt(pow((cam[X] / TILE_SIZE) - pos2[X], 2) + pow((cam[Y] / TILE_SIZE) - pos2[Y], 2)));
}

int	sort_sprites(t_vars *vars, t_sprite **sprites)
{
	t_sprite	tmp;
	int			i;
	int			j;

	i = 0;
	while (i < vars->spr_count - 1)
	{
		j = 0;
		while (j < vars->spr_count - i - 1)
		{
			if (euclid_dist(vars->player.pos, (*sprites)[j].spr_pos)
				< euclid_dist(vars->player.pos, (*sprites)[j + 1].spr_pos))
			{
				tmp = (*sprites)[j];
				(*sprites)[j] = (*sprites)[j + 1];
				(*sprites)[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	return (0);
}

char	*choose_texture(char c)
{
	char	*text;

	if (c == 'X')
		text = ft_strdup("./textures/idle1.xpm");
	if (c == 'B')
		text = ft_strdup("./textures/barrel.xpm");
	if (c == 'D')
		text = ft_strdup("./textures/collectible_img.xpm");
	return (text);
}

int	texture_fill(t_vars *vars, t_sprite *sprite, char c)
{
	int		x;
	int		y;
	char	*text;

	x = 64;
	y = 64;
	text = choose_texture(c);
	if (!text)
		return (err("Strdup error"));
	sprite->sprite.img = mlx_xpm_file_to_image(vars->mlx.mlx, text, &x, &y);
	if (!sprite->sprite.img)
		return (ft_putstr_fd(text, 2), err(" can't found"), null_free(text));
	sprite->sprite.addr = mlx_get_data_addr(sprite->sprite.img, &sprite->sprite.bits_per_pixel, &sprite->sprite.line_length, &sprite->sprite.endian);
	if (!sprite->sprite.addr)
		return (null_free(text), err("get_data_addr error"));
	return (null_free(text), 0);
}

t_sprite	*detect_barrels(t_vars *vars)
{
	int	count;
	int	x;
	int	y;
	t_sprite	*sprites;

	count = 0;
	y = -1;
	while (vars->map[++y] != NULL)
	{
		x = -1;
		while (vars->map[y][++x] != '\0')
			if (vars->map[y][x] == 'X' || vars->map[y][x] == 'B' || vars->map[y][x] == 'D')
				count++;
	}
	if (count == 0)
		return (NULL);
	vars->spr_count = count;
	sprites = malloc(sizeof(t_sprite) * (count));
	if (!sprites)
		return (err("Malloc error"), NULL);
	y = -1;
	while (vars->map[++y] != NULL)
	{
		x = -1;
		while (vars->map[y][++x] != '\0')
		{
			if (vars->map[y][x] == 'X')
			{
				count--;
				sprites[count].spr_pos[X] = (double)x;
				sprites[count].spr_pos[Y] = (double)y;
				sprites[count].is_enemy = 1;
				sprites[count].hit = 0;
				sprites[count].life = 500;
				sprites[count].emy_ani = 0;
				sprites[count].spr_ani = 0;
				sprites[count].time = 0;
				if (texture_fill(vars, &sprites[count], vars->map[y][x]))
					return (NULL);
			}
			if (vars->map[y][x] == 'B' || vars->map[y][x] == 'D')
			{
				count--;
				sprites[count].spr_pos[X] = (double)x;
				sprites[count].spr_pos[Y] = (double)y;
				sprites[count].hit = 0;
				sprites[count].is_enemy = 0;
				if (texture_fill(vars, &sprites[count], vars->map[y][x]))
					return (NULL);
			}
		}
		if (count == -1)
			break ;
	}
	sort_sprites(vars, &sprites);
	return (sprites);
}

typedef struct s_spr_vars
{
	double	sprite[2];
	double	transform[2];
	double	inv_det;
	int		screen_x;
	int		width;
	int		height;
	int		draw_s[2];
	int		draw_e[2];
	int		tex[2];

}	t_spr_vars;

void	calculate_draw_borders(t_render render, t_spr_vars *spr_vars)
{
	spr_vars->height = abs((int)(render.sc_height / spr_vars->transform[Y]));
    spr_vars->draw_s[Y] = (-1 * spr_vars->height) / 2 + render.sc_height / 2;
    if (spr_vars->draw_s[Y] < 0)
        spr_vars->draw_s[Y] = 0;
    spr_vars->draw_e[Y] = spr_vars->height / 2 + render.sc_height / 2;
    if (spr_vars->draw_e[Y] >= render.sc_height)
        spr_vars->draw_e[Y] = render.sc_height - 1;
	spr_vars->width = abs((int)(render.sc_height / spr_vars->transform[Y]));
    spr_vars->draw_s[X] = (-1 * spr_vars->width) / 2 + spr_vars->screen_x;
    if (spr_vars->draw_s[X] < 0)
        spr_vars->draw_s[X] = 0;
    spr_vars->draw_e[X] = spr_vars->width / 2 + spr_vars->screen_x;
    if (spr_vars->draw_e[X] >= render.sc_width)
        spr_vars->draw_e[X] = render.sc_width - 1;

}

t_spr_vars	fill_spr(t_vars *vars, t_sprite sprite)
{
	t_spr_vars	spr_vars;
	t_player	player;

	player = vars->player;
	spr_vars.sprite[X] = sprite.spr_pos[X] - (player.pos[X] / TILE_SIZE) + 0.5;
	spr_vars.sprite[Y] = sprite.spr_pos[Y] - (player.pos[Y] / TILE_SIZE) + 0.5;
	spr_vars.inv_det = 1.0 / (player.plane[X] * player.dir[Y] - player.dir[X] \
		* player.plane[Y]);
	spr_vars.transform[X] = spr_vars.inv_det * (player.dir[Y] \
		* spr_vars.sprite[X] - player.dir[X] * spr_vars.sprite[Y]);
	spr_vars.transform[Y] = spr_vars.inv_det * ((-player.plane[Y]) \
		* spr_vars.sprite[X] + player.plane[X] * spr_vars.sprite[Y]);
	spr_vars.screen_x = (int)((vars->render.sc_width / 2) \
		* (1 + (spr_vars.transform[X] / spr_vars.transform[Y])));
	calculate_draw_borders(vars->render, &spr_vars);
	return (spr_vars);
}

int	sprite_display(t_vars *vars, t_sprite *sprite, t_spr_vars spr_vars)
{
	if (sprite->is_enemy)
	{
		if (sprite->life <= 0)
		{
			vars->end_ani = 0;
			sprite->emy_ani = 4;
			if (sprite->spr_ani == 8)
				return (-16777216);
		}
		else if (!sprite->hit && sprite->dist > 7)
			sprite->emy_ani = 0;
		else if (!sprite->hit && sprite->dist <= 1.5f)
			sprite->emy_ani = 1;
		else if (sprite->hit)
		{
			vars->end_ani = 0;
			sprite->emy_ani = 3;
		}
		else
			sprite->emy_ani = 2;
		if ((sprite->emy_ani == 0 || sprite->emy_ani == 3) && sprite->spr_ani > 3)
			sprite->spr_ani = 0;
		return (texture_color(&vars->enemy.sprites[sprite->emy_ani][sprite->spr_ani], spr_vars.tex[X], spr_vars.tex[Y]));
	}
	return (texture_color(&sprite->sprite, spr_vars.tex[X], spr_vars.tex[Y]));
}

void	draw_sprite(t_vars *vars, t_spr_vars spr_vars, t_sprite *sprite, double *ddist)
{
	int	stripe;
	int	y;
	int	d;
	int	t;
	int color;

	stripe = spr_vars.draw_s[X] - 1;
    while (++stripe < spr_vars.draw_e[X])
    {
		t = 64;
		if (sprite->is_enemy)
			t = 128;
		spr_vars.tex[X] = (int)((stripe - ((-1 * spr_vars.width) / 2 + spr_vars.screen_x)) * t / spr_vars.width);
		if (spr_vars.transform[Y] > 0 && stripe > 0 && stripe < vars->render.sc_width && spr_vars.transform[Y] < ddist[stripe])
        {
			y = spr_vars.draw_s[Y] - 1;
			while (++y < spr_vars.draw_e[Y])
            {
                d = (y) * 256 - vars->render.sc_height * 128 + spr_vars.height * 128;
                spr_vars.tex[Y] = ((d * t) / spr_vars.height) / 256;
                color = sprite_display(vars, sprite, spr_vars);
                if (color != -16777216)
                    pixel_put(&vars->sprites_canvas, stripe, y, color);
            }
        }
    }
}

int cast_spr(t_vars *vars, double *ddist)
{
    int			i;
    t_sprite	*sprite;
	t_spr_vars	spr_vars;

    sprite = vars->sprites;
	if (!sprite)
		return (0);
	sort_sprites(vars, &sprite);
    if (!sprite)
        return (1);
    i = -1;
    while (++i < vars->spr_count)
    {
        spr_vars = fill_spr(vars, sprite[i]);
		sprite[i].dist = euclid_dist(vars->player.camera, sprite[i].spr_pos);
		if (1)
		{
			if (sprite[i].time == 0 || get_time() - sprite[i].time > 100)
			{
				sprite[i].time = get_time();
				if (sprite[i].emy_ani == 0 && sprite[i].spr_ani < 3)
					sprite[i].spr_ani++;
				else if (sprite[i].emy_ani == 1 && sprite[i].spr_ani < 5)
					sprite[i].spr_ani++;
				else if (sprite[i].emy_ani == 2 && sprite[i].spr_ani < 5)
					sprite[i].spr_ani++;
				else if (sprite[i].emy_ani == 3 && sprite[i].spr_ani < 3)
					sprite[i].spr_ani++;
				else if (sprite[i].emy_ani == 4 && sprite[i].spr_ani < 8)
					sprite[i].spr_ani++;
				else
				{
					if (sprite[i].emy_ani == 1 && sprite[i].dist <= 1)
						vars->player.life -= 25;
					if (sprite[i].hit)
					{
						sprite[i].hit = 0;
						if (vars->player.gun_type == 2)
							sprite[i].life -= 250;
						sprite[i].life -= 50;
					}
					if (vars->player.life <= 0)
					{
						mlx_mouse_show();
						vars->menu = 1;
					}
					if (sprite[i].life > 0)
						sprite[i].spr_ani = 0;
					vars->end_ani = 1;
				}
			}
		}
		draw_sprite(vars, spr_vars, &sprite[i], ddist);
    }
    return (0);
}
