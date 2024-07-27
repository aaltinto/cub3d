/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_text_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 20:42:35 by aaltinto          #+#    #+#             */
/*   Updated: 2024/07/27 20:42:36 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/bonus.h"

char	*get_xpm_filename(char *filename, int i)
{
	char	*tmp;
	char	*tmp2;
	char	*num;

	num = ft_itoa(i);
	if (!num)
		return (err("Itoa error"), NULL);
	tmp = ft_strjoin(filename, num);
	if (null_free(num), !tmp)
		return (err("Strjoin err"), NULL);
	tmp2 = ft_strjoin(tmp, ".xpm");
	if (null_free(tmp), !tmp2)
		return (err("Strjoin err"), NULL);
	return (tmp2);
}
