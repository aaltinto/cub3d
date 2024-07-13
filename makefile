CC = gcc
RM = rm -f
CFLAGS =  -g
OpenGL = -framework OpenGL -framework AppKit
SRC = src/main.c\
		src/map_reader.c\
		src/free_errors.c\
		src/init_textures.c\
		src/init_colors.c\
		src/render.c\
		src/utils.c\
		src/key_pres.c\
		src/mini_map.c\
		src/ray_utils.c\
		src/render_utils.c\
		src/map.c\
		src/cast_rays.c

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
	$(CC) $(CFLAGS) $(OBJ) $(GNL) $(LIB) $(MLX) $(OpenGL) -o $(NAME)

clean:
	@make clean -C libft
	@make clean -C gnl
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

mlx:
	@make clean -C minilibx
	@make all -C minilibx

norm:
	@norminette $(SRC) cub3d.h
.PHONY: all clean fclean re