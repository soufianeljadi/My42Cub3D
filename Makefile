NAME = cub3D

SRCS =  src/parse_color.c  src/parse_textures.c src/parse_map.c src/parse_cub.c src/ft_error.c src/validate_map.c\
		src/textures_utils.c src/validate_map_utils.c \
		src/main.c src/cast_rays.c src/draw.c src/move.c src/distance.c \
		gnl/gnl.c gnl/gnl_utils.c \
		utils/ft_atoi.c utils/ft_strchr.c utils/ft_strcmp.c utils/ft_strncmp.c utils/ft_strjoin.c utils/ft_strdup.c \
		utils/ft_strtok.c utils/ft_substr.c utils/ft_strrchr.c utils/ft_strtrim.c utils/ft_isdigit.c utils/ft_split.c 
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS =  -Wall -Wextra -Werror 
MLX42= .MLX42/libmlx42.a
MLXFLAGS = -Iinclude -ldl -lglfw -pthread -lm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX42) $(MLXFLAGS) -o $(NAME)
%.o:%.c includes/cub3d.h
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all