CC = gcc
RM = rm -f
CFLAGS =  -g
OpenGL = -framework OpenGL -framework AppKit
SRC = src/mandatory/main.c\
		src/mandatory/map_reader.c\
		src/mandatory/free_errors.c\
		src/mandatory/init_textures.c\
		src/mandatory/init_colors.c\
		src/mandatory/render.c\
		src/mandatory/utils.c\
		src/mandatory/key_pres.c\
		src/mandatory/ray_utils.c\
		src/mandatory/render_utils.c\
		src/mandatory/map.c\
		src/mandatory/set_up.c\
		src/mandatory/cast_rays.c

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
	$(RM) $(OBJ_BNS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

mlx:
	@make clean -C minilibx
	@make all -C minilibx

norm:
	@norminette $(SRC) cub3d.h
.PHONY: all clean fclean re