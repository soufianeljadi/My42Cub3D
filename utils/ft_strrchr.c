#include "../includes/cub3d.h"

char *ft_strrchr(const char *s, int c) 
{
    const char *last_occurrence = NULL;

    while (*s) {
        if (*s == (char)c)
            last_occurrence = s;
        s++;
    }

    if ((char)c == '\0')
        return (char *)s;
    return (char *)last_occurrence;
}