/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 04:42:18 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/12 04:42:21 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../cub3d.h"
#include "../libft/libft.h"

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

int	find_longest_line(char **str)
{
	int	i;
	int	len;

	i = -1;
	len = 0;
	while ((str)[++i])
		if (len < (int)ft_strlen((str)[i]))
			len = ft_strlen((str)[i]);
	return (len);
}

char	**reallocate_double(char	***str)
{
	char	**tmp;
	int		i;
	int		j;
	int		len;

	len = find_longest_line(*str);
	tmp = (char **)malloc(sizeof(char *) * (double_counter(*str) + 1));
	if (!tmp)
		return (err("Malloc error"), NULL);
	i = -1;
	while ((*str)[++i])
	{
		tmp[i] = ft_calloc(sizeof(char), len);
		if (!tmp[i])
			return (free(tmp), err("Malloc error"), NULL);
		j = -1;
		while ((*str)[i][++j])
			tmp[i][j] = (*str)[i][j];
		tmp[i][j] = '\0';
	}
	tmp[i] = NULL;
	return (free_doubles(*str), tmp);
}
