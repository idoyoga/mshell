# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/02 16:05:14 by dplotzl           #+#    #+#              #
#    Updated: 2025/02/12 16:37:20 by xgossing         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= libft.a

CC 		= cc 
CFLAGS 	= -Werror -Wall -Wextra
AR 		= ar rcs

INC_DIR = ./inc/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

SRC 	= ft_atoi.c \
		  ft_putnbr_fd.c \
		  ft_putendl_fd.c \
		  ft_putstr_fd.c \
		  ft_putchar_fd.c \
		  ft_striteri.c \
		  ft_strmapi.c \
		  ft_itoa.c \
		  ft_strjoin.c \
		  ft_split.c \
		  ft_strtrim.c \
		  ft_substr.c \
		  ft_strdup.c \
		  ft_calloc.c \
		  ft_strnstr.c \
		  ft_strncmp.c \
		  ft_memset.c \
		  ft_memcmp.c \
		  ft_memchr.c \
		  ft_memcpy.c \
		  ft_memmove.c \
		  ft_bzero.c \
		  ft_isalnum.c \
		  ft_isalpha.c \
		  ft_isascii.c \
		  ft_isdigit.c \
		  ft_isprint.c \
		  ft_strchr.c \
		  ft_strrchr.c \
		  ft_strlcat.c \
		  ft_strlcpy.c \
		  ft_strlen.c \
		  ft_tolower.c \
		  ft_toupper.c \
		  ft_strcmp.c \
		  ft_free.c \
		  ft_isblank.c \
		  ft_strndup.c

SRCS	= $(addprefix $(SRC_DIR), $(SRC))
OBJS	= $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
INC		= -I $(INC_DIR)

GREEN	= \033[0;92m
DEF_C	= \033[0;39m

MAKEFLAGS	+= --jobs

all: $(NAME)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo "$(GREEN)Archiving $(NAME) ...$(DEF_C)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR) 

fclean: clean
	@rm -f $(NAME)

re: fclean
	$(MAKE) all

.PHONY:	all clean fclean re
