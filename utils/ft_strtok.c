#include "../includes/cub3d.h"

char *ft_strtok(char *str, const char *delim) {
    static char *save_ptr = NULL;
    char *token_start;
    // char *token_end;

    if (str)
        save_ptr = str;

    if (!save_ptr || *save_ptr == '\0')
        return NULL;

    while (*save_ptr && ft_strchr(delim, *save_ptr))
        save_ptr++;

    if (*save_ptr == '\0') {
        save_ptr = NULL;
        return NULL;
    }

    token_start = save_ptr;

    while (*save_ptr && !ft_strchr(delim, *save_ptr))
        save_ptr++;

    if (*save_ptr) {
        *save_ptr = '\0';
        save_ptr++;
    } else {
        save_ptr = NULL;
    }

    return token_start;
}