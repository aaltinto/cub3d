/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:50:09 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/12 13:51:07 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include "../libft/libft.h"

int north(t_vars *vars, char *tmp)
{
	if (vars->textures.north)
		return (err("Duplicating indicators: 'NO'"));
	vars->textures.north = ft_substr(tmp, 2, ft_strlen(tmp));
	if (!vars->textures.north)
		return (err("Substr error"));
	return(0);
}

int south(t_vars *vars, char *tmp)
{
	if (vars->textures.south)
		return (err("Duplicating indicators: 'SO'"));
	vars->textures.south = ft_substr(tmp, 2, ft_strlen(tmp));
	if (!vars->textures.south)
		return (err("Substr error"));
	return(0);
}

int west(t_vars *vars, char *tmp)
{
	if (vars->textures.west)
		return (err("Duplicating indicators: 'WE'"));
	vars->textures.west = ft_substr(tmp, 2, ft_strlen(tmp));
	if (!vars->textures.west)
		return (err("Substr error"));
	return(0);
}

int east(t_vars *vars, char *tmp)
{
	if (vars->textures.east)
		return (err("Duplicating indicators: 'EA'"));
	vars->textures.east = ft_substr(tmp, 2, ft_strlen(tmp));
	if (!vars->textures.east)
		return (err("Substr error"));
	return(0);
}

int	init_textures(t_vars *vars, char *tmp)
{
	if (ft_strnstr(tmp, "NO ", 3) != NULL && north(vars, tmp))
		return (1);
	else if (ft_strnstr(tmp, "SO ", 3) != NULL && south(vars, tmp))
		return (1);
	else if (ft_strnstr(tmp, "WE ", 3) != NULL && west(vars, tmp))
		return (1);
	else if (ft_strnstr(tmp, "EA ", 3) != NULL && east(vars, tmp))
		return (1);
	return (0);
}
