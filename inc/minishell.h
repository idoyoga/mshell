/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:51:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/15 17:37:17 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define CMD_NOT_FOUND 127
# define CMD_NOT_EXEC 126
# define DEFAULT_ALLOC_CAPACITY 100

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/inc/libft.h"
# include "errors.h"

typedef enum e_token_type
{
	WORD,
	PIPE,
	VAR,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
}	t_tok_type;

typedef struct s_lst
{
	void			*data;
	struct s_lst	*next;
	struct s_lst	*prev;
}	t_lst;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				argc;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_token
{
	char			*content;
	int				status;
	t_tok_type		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

/*
**	Meta-tracking allocations, used to keep track of all allocated memory
**	including the shell struct itself.
*/

typedef struct s_alloc_tracker
{
	void	**allocs;
	int		count;
	int		capacity;
	bool	initialized;
}	t_alloc;

typedef struct s_shell
{
	t_cmd	*cmd;
	t_lst	*env;
	t_alloc	alloc_tracker;
	t_token	*tokens;
	int		env_count;
	int		status;
	char	*prompt;
	char	*cmd_input;
	char	*home_dir;
	char	*work_dir;
	char	*old_work_dir;
	char	*user;
	pid_t	pid;
}	t_shell;

// --------------  alloc  --------------------------------------- //
bool	alloc_tracker_add(t_alloc *tracker, void *ptr);
void	*wrap_malloc(t_alloc *tracker, size_t size);
void	*wrap_calloc(t_alloc *tracker, size_t count, size_t size);
void	free_allocs(t_alloc *tracker);

// --------------  main  ---------------------------------------- //
bool	error(t_shell *shell, char *error, int status);

// --------------  init_shell  ---------------------------------- //
bool	init_shell(t_shell *shell, char **env);

// --------------  lst_utils  ----------------------------------- //
bool	append_node(t_shell *shell, t_lst **lst, void *data);
size_t	lst_len(t_lst *lst);

// --------------  utils  --------------------------------------- //
char	*create_prompt(t_shell *shell);

#endif
