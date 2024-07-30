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
			if (vars->map[y][x] == 'X')
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
			if (vars->map[y][x] == 'X')
			{
				if (count == -1)
					break ;
				count--;
				sprites[count].spr_pos[X] = (double)x;
				sprites[count].spr_pos[Y] = (double)y;
			}
	}
	sort_sprites(vars, &sprites);
	return (sprites);
}

int cast_spr(t_vars *vars, double *ddist)
{
    int i;
    t_ray ray_data;
    t_sprite *sprite;
    double inv_det;

    sprite = vars->sprites;
	if (!sprite)
		return (0);
	printf("hola\n");
	sort_sprites(vars, &sprite);
    if (!sprite)
        return (1);
    fill_variable(vars, &ray_data);
    i = -1;
    while (++i < vars->spr_count)
    {
        double spriteX = sprite[i].spr_pos[X] - (vars->player.pos[X] / TILE_SIZE) + 0.5;
        double spriteY = sprite[i].spr_pos[Y] - (vars->player.pos[Y] / TILE_SIZE) + 0.5;
        inv_det = 1.0 / (vars->player.plane[X] * vars->player.dir[Y] - vars->player.dir[X] * vars->player.plane[Y]);
        double transformX = inv_det * (vars->player.dir[Y] * spriteX - vars->player.dir[X] * spriteY);
        double transformY = inv_det * ((-vars->player.plane[Y]) * spriteX + vars->player.plane[X] * spriteY);
        int spriteScreenX = (int)((vars->render.sc_width / 2) * (1 + (transformX / transformY)));

        int spriteHeight = abs((int)(vars->render.sc_height / transformY));
        int drawStartY = (-1 * spriteHeight) / 2 + vars->render.sc_height / 2;
        if (drawStartY < 0)
            drawStartY = 0;
        int drawEndY = spriteHeight / 2 + vars->render.sc_height / 2;
        if (drawEndY >= vars->render.sc_height)
            drawEndY = vars->render.sc_height - 1;

        int spriteWidth = abs((int)(vars->render.sc_height / transformY));
        int drawStartX = (-1 * spriteWidth) / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= vars->render.sc_width)
            drawEndX = vars->render.sc_width - 1;

        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = (int)((stripe - ((-1 * spriteWidth) / 2 + spriteScreenX)) * 64 / spriteWidth);
            if (transformY > 0 && stripe > 0 && stripe < vars->render.sc_width && transformY < ddist[stripe])
            {
                for (int y = drawStartY; y < drawEndY; y++)
                {
                    int d = (y) * 256 - vars->render.sc_height * 128 + spriteHeight * 128;
                    int texY = ((d * 64) / spriteHeight) / 256;
                    int color = texture_color(&vars->sprite, texX, texY);
                    if (color != -16777216)
                        pixel_put(&vars->sprites_canvas, stripe, y, color);
                }
            }
        }
    }
    return (0);
}
