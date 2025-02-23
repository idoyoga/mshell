# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/05 17:52:33 by dplotzl           #+#    #+#              #
#    Updated: 2025/02/22 14:10:50 by dplotzl          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC		= cc
CFLAGS	= -Werror -Wall -Wextra -g

SRC_DIR	=	src/
OBJ_DIR	=	obj/
LIB_DIR	=	./libft/
LIBFT	=	./libft/libft.a
INC		=	-I ./inc -I ./libft
HEADER 	=	inc/minishell.h

SRC		=	helper/alloc.c \
			helper/alloc_helper.c \
			helper/clean.c \
			helper/cmd_utils.c \
			helper/env_utils.c \
			helper/error.c \
			helper/expander_utils.c \
			helper/init.c \
			helper/signal.c \
			helper/token_utils.c \
			parsing/cmd_parser.c \
			parsing/cmd_redir.c \
			parsing/expander.c \
			parsing/heredoc.c \
			parsing/token.c \
			parsing/tokenizer.c \
			execution/execute.c \
			execution/builtin.c \
			execution/pipeline.c \
			builtins/dispatch.c \
			builtins/cd.c \
			builtins/echo.c \
			builtins/env.c \
			builtins/exit.c \
			builtins/export.c \
			builtins/pwd.c \
			builtins/unset.c \
			main.c \

SRCS	=	$(addprefix $(SRC_DIR), $(SRC))
OBJS	=	$(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(SRCS:.c=.o))

MAKEFLAGS	+= --jobs

all: $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline $(INC)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER) Makefile | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT): $(OBJS)
	@$(MAKE) --no-print-directory -C $(LIB_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -sC $(LIB_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -sC $(LIB_DIR) fclean

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re

