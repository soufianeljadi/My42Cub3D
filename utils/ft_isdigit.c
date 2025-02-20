#include "../includes/cub3d.h"

int ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int ft_all_digits(char **str)
{
    int i = 0;
    while (str[i])
    {
        int j = 0;
        while (str[i][j])
        {
            if (!ft_isdigit(str[i][j]) && str[i][j] != '\n')
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}