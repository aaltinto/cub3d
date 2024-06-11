CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror 
SRC = src/main.c

LIB = libft/libft.a
GNL = gnl/gnl.a
OBJ = $(SRC:.c=.o)

NAME = cub3d

all: $(NAME)

$(NAME): $(OBJ)
	@make all -C minilibx
	@make all -C libft
	@make all -C gnl
	$(CC) $(CFLAGS) $(GNL) $(LIB) $(OBJ) -o $(NAME)

clean:
	@make clean -C minilibx
	@make clean -C libft
	@make clean -C gnl
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re