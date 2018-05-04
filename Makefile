NAME		=	malloc_test

CC			=	gcc
FLAGS		=	-Wall -Wextra -Werror

SRC_DIR		=	src/
INC_DIR		=	include/
OBJ_DIR		=	objs/

SRC_BASE	=	main.c \
				concatenate.c \
				convert_and_allocate.c \
				free.c \
				ft_memcpy.c \
				ft_putchar.c \
				ft_putnbr.c \
				ft_putstr.c \
				ft_strlen.c \
				init.c \
				malloc.c \
				realloc.c

SRCS = $(addprefix $(SRC_DIR), $(SRC_BASE))
OBJS = $(addprefix $(OBJ_DIR), $(SRC_BASE:.c=.o))

all : $(NAME)

$(NAME):		$(OBJ_DIR) $(OBJS)
	@$(CC) $(OBJS) -o $(NAME) \
		-I $(INC_DIR) \
		$(FLAGS)
	@echo "\033[1;34m"$(NAME)" compiled"

$(OBJ_DIR) :
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $(OBJS))

$(OBJ_DIR)%.o :	$(SRC_DIR)%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) -MMD -c $< -o $@\
		-I $(INC_DIR)\

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;33mall is clean";

fclean: clean
	@rm -f $(NAME)
	@echo "\033[3;0;31mall files have been scratched";

re:				fclean all

.PHONY :		fclean clean re

-include $(OBJS:.o=.d)
