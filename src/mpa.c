static int	check_borders(char	***(*map))
{
	int	i;
	int	j;
	int	len;
  
	(*map) = reallocate_double((*map));
	if (!(*map))
		return (err("Allocate error"), 1);
	len = find_longest_line((*map));
	i = -1;
	while ((*map)[++i])
	{
		j = -1;
		while ((*map)[i][++j])
		{
			if ((*map)[i][j] == '0' && (i == 0 || j == len -1 || (j < len
				&& ((*map)[i][j + 1] == '\0' || (*map)[i][j + 1] == ' '
				|| (*map)[i][j + 1] == '\n')) || j == 0 || (j > 0
				&& (*map)[i][j - 1] == ' ') || (*map)[i + 1] == NULL
				|| ((*map)[i + 1] != NULL && ((*map)[i + 1][j] == '\0'
				|| (*map)[i + 1][j] == ' ')) || (i != 0
				&& ((*map)[i -1][j] == ' ' || (*map)[i -1][j] == '\0'))))
				return (err("Error!\nMap should be closed by 1's"));
		}
	}
	return (0);
}