#include <stdio.h>
#include <fcntl.h>

int main()
{
    int fd = open("textures/tes.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file\n");
        return 1;
    }

}