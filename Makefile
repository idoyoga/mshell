# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/05 17:52:33 by dplotzl           #+#    #+#              #
#    Updated: 2025/01/23 14:57:12 by dplotzl          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC		= cc
CFLAGS	= -Werror -Wall -Wextra -g
LDFLAGS = -l readline

INC_DIR = ./inc/
SRC_DIR = ./src/
OBJ_DIR = ./obj/
LIB_DIR = ./libft/
LIBFT	= ./libft/libft.a

SRC		= main.c \
		  alloc.c \
		  alloc_utils.c \
		  init.c \
		  env_utils.c \
		  lexer.c \
		  parse_utils.c \
		  token.c \
		  token_utils.c \

SRCS	= $(addprefix $(SRC_DIR), $(SRC))
OBJS	= $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
INC		= -I $(INC_DIR) -I $(LIB_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make -C $(LIB_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIB_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re 
