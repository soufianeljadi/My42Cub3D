NAME = cub3D

SRCS = src/main.c src/parser.c src/utils.c \
		get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
		utils/ft_atoi.c utils/ft_strchr.c utils/ft_strcmp.c utils/ft_strncmp.c utils/ft_strjoin.c utils/ft_strdup.c \
		utils/ft_strtok.c utils/ft_substr.c utils/ft_strrchr.c utils/ft_strtrim.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o : %.c /includes/cub3d.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all