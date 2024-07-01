CC = gcc
RM = rm -f
CFLAGS =  -g
OpenGL = -framework OpenGL -framework AppKit
SRC = src/main.c\
		src/map_reader.c\
		src/free_errors.c\
		src/init_textures.c\
		src/init_colors.c\
		src/utils.c

LIB = libft/libft.a
GNL = gnl/gnl.a
MLX = minilibx/libmlx.a
OBJ = $(SRC:.c=.o)

NAME = cub3d

all: $(NAME)

$(NAME): $(OBJ)
	@make all -C minilibx
	@make all -C libft
	@make all -C gnl
	$(CC) $(GNL) $(LIB) $(MLX) $(OpenGL) $(OBJ) -o $(NAME) -g

clean:
	@make clean -C libft
	@make clean -C gnl
	$(RM) $(OBJ)

fclean: clean
	@make clean -C minilibx
	$(RM) $(NAME)

re: fclean $(NAME)

norm:
	@norminette $(SRC) cub3d.h
.PHONY: all clean fclean re