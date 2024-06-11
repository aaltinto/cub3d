#include <stdio.h>
#include "../cub3d.h"
#include "../gnl/get_next_line.h"
#include "../libft/libft.h"

int	err(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

int	read_map(char **argv, t_vars *vars)
{
	size_t	len;
	int		fd;
	char	*map;

	(void)vars;
	len = ft_strlen(argv[1]) - 1;
	if (argv[1][len] != 'b' || argv[1][len -1] != 'u' || argv[1][len -2] != 'c'
		|| argv[1][len -3] != '.')
		return (err(ARG));
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("Can't open the file"), 1);
	map = get_next_line(fd);
	printf("%s\n", map);
	return (0);
}

int main(int ac, char **argv)
{
	t_vars	vars;

	if (ac != 2)
		return (err(ARG));
	if (read_map(argv, &vars))
		return (1);
	return (0);
}