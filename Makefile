# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/05 17:52:33 by dplotzl           #+#    #+#              #
#    Updated: 2025/03/05 12:38:16 by dplotzl          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CC		= cc
CFLAGS	= -Werror -Wall -Wextra

SRC_DIR	=	src/
OBJ_DIR	=	obj/
LIB_DIR	=	./libft/
LIBFT	=	./libft/libft.a
INC		=	-I ./inc -I ./libft
HEADER 	=	inc/minishell.h

SRC		=	helper/alloc.c \
			helper/alloc_helper.c \
			helper/alloc_utils.c \
			helper/clean.c \
			helper/cmd_utils.c \
			helper/env_helper.c \
			helper/env_utils.c \
			helper/error.c \
			helper/expander_utils.c \
			helper/init.c \
			helper/parser_utils.c \
			helper/signal.c \
			helper/token_utils.c \
			parsing/cmd_parser.c \
			parsing/cmd_redir.c \
			parsing/expander.c \
			parsing/expander_cases.c \
			parsing/expander_quotes.c \
			parsing/heredoc.c \
			parsing/quote_removal.c \
			parsing/token.c \
			parsing/tokenizer.c \
			execution/execute.c \
			execution/builtin.c \
			execution/pipeline.c \
			execution/setup.c \
			execution/setup_access.c \
			execution/teardown.c \
			execution/utils.c \
			builtins/dispatch.c \
			builtins/cd.c \
			builtins/echo.c \
			builtins/env.c \
			builtins/exit.c \
			builtins/export.c \
			builtins/pwd.c \
			builtins/unset.c \
			builtins/utils.c \
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

leak: 
	(ulimit -v 140000; valgrind --leak-check=full --show-leak-kinds=all --suppressions=suppression.txt --track-fds=yes --trace-children=yes ./$(NAME))

.PHONY: all clean fclean re leak
