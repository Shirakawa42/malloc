ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		=	libft_malloc_$(HOSTTYPE).so

CC			=	gcc
FLAGS		=	-Wall -Wextra -Werror

SRC_DIR		=	src/
INC_DIR		=	include/
OBJ_DIR		=	objs/

SRC_BASE	=	show_alloc_mem.c \
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
				realloc.c \
				ft_putaddr.c \
				look_for_destruction.c \
				search_for_free_space.c \
				search_in_zone.c

SRCS = $(addprefix $(SRC_DIR), $(SRC_BASE))
OBJS = $(addprefix $(OBJ_DIR), $(SRC_BASE:.c=.o))

all : $(NAME)

$(NAME):		$(OBJ_DIR) $(OBJS)
	@$(CC) $(OBJS) -shared -o $(NAME) \
		-I $(INC_DIR) \
		$(FLAGS)
	@rm -f libft_malloc.so
	@ln -s $(NAME) libft_malloc.so
	@echo "\033[1;34m"$(NAME)" compiled"

$(OBJ_DIR) :
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $(OBJS))

$(OBJ_DIR)%.o :	$(SRC_DIR)%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) -fPIC -c $< -o $@\
		-I $(INC_DIR)\

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;33mall is clean";

fclean: clean
	@rm -f $(NAME)
	@rm -f libft_malloc.so
	@echo "\033[3;0;31mall files have been scratched";

re:				fclean all

.PHONY :		fclean clean re

-include $(OBJS:.o=.d)
