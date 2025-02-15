#include "../includes/cub3d.h"


char *ft_strtrim(char const *s, char const *set) 
{
    size_t start;
    size_t end;
    char *trimmed;

    if (!s || !set)
        return NULL;

    start = 0;
    while (s[start] && ft_strchr(set, s[start]))
        start++;

    end = ft_strlen(s);
    while (end > start && ft_strchr(set, s[end - 1]))
        end--;

    trimmed = malloc((end - start + 1) * sizeof(char));
    if (!trimmed)
        return NULL;

    ft_strlcpy(trimmed, s + start, end - start + 1);

    return trimmed;
}