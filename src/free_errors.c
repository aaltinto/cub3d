/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 04:41:58 by aaltinto          #+#    #+#             */
/*   Updated: 2024/06/12 17:22:30 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

int	err(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

int	free_doubles2(void **str, int len)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (++i < len)
		free(str[i]);
	free(str);
	return (0);
}
int	free_doubles(char **str)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	return (0);
}
