/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:51:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/12 22:19:20 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define DEFAULT_ALLOC_CAPACITY 100

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
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
	END,
	REDIR_IN,
	HEREDOC,
	REDIR_OUT,
	REDIR_APPEND,
	PIPE,
	CMD,
	ARG
}	t_t_typ;

typedef enum e_builtin_type
{
	_NOT_A_BUILTIN,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_b_typ;

typedef struct s_env
{
	char			*data;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_tok
{
	char			*content;
	char			*file;
	t_t_typ			type;
	struct s_tok	*next;
	struct s_tok	*prev;
}	t_tok;

/*
**	Meta-tracking allocations, used to keep track of all allocated memory
**	including the shell struct itself.
*/

typedef struct s_alloc_tracker
{
	void	**allocs;
	int		*flags;
	int		count;
	int		capacity;
	bool	initialized;
}	t_alloc;

typedef struct s_shell
{
	t_cmd	*cmd;			// Command list
	t_env	*env;			// Environment variables
	t_tok	*tokens;		// Token list
	t_alloc	alloc_tracker;	// Memory tracker
	int		env_count;		// Environment variable count
	int		status;			// Exit status
	char	*prompt;		// Prompt string
	char	*cmd_input;		// Command input
	char	*home_dir;		// Home directory
	char	*work_dir;		// Current working directory
	char	*old_work_dir;	// Previous working directory
	char	*user;			// User name
}	t_shell;

typedef struct s_builtin
{
	t_b_typ	type;
	char	*name;
	void	(*fn)(t_shell *shell, t_cmd *cmd);
}	t_bltin;

// --------------  alloc  ------------------------------------------------- //
void	*alloc_tracker_add(t_alloc *tracker, void *ptr, int is_array);
void	free_allocs(t_alloc *tracker);

// --------------  alloc_helper  ------------------------------------------ //
void	*safe_malloc(t_shell *shell, size_t size);
void	*safe_calloc(t_shell *shell, size_t count, size_t size);
char	*safe_strdup(t_shell *shell, const char *src);
char	*safe_strjoin(t_shell *shell, const char *s1, const char *s2);

// --------------  cmd_parser  -------------------------------------------- //
bool	parse_commands(t_shell *shell);

// --------------  cmd_redir  --------------------------------------------- //
bool	handle_redirection(t_shell *shell, t_tok *token, t_cmd *cmd);

// --------------  cmd_utils  --------------------------------------------- //
t_cmd	*add_cmd(t_shell *shell, t_cmd **lst);

// --------------  env_utils  --------------------------------------------- //
t_env	*add_env_var(t_shell *shell, t_env **lst, char *data);
char	*create_prompt(t_shell *shell);
int		env_var_count(char **env);

// --------------  error  ------------------------------------------------- //
bool	error(const char *error_msg, int status);
void	error_exit(t_shell *shell, const char *error_msg, int exit_status);
void	clean_shell(t_shell *shell);

// --------------  expander  ---------------------------------------------- //
bool	expand_dollar_variables(t_shell *shell, char **input);

// --------------  expander_helper  --------------------------------------- //
int		find_env_variable(t_shell *shell, char *input, int *index);
bool	append_char_to_str(t_shell *shell, char **output, int *index, char *c);

// --------------  init  -------------------------------------------------- //
bool	init_shell(t_shell *shell, char **env);

// --------------  token  ------------------------------------------------- //
bool	tokenize(t_shell *shell, t_tok **lst, char *input);

// --------------  tokenizer  --------------------------------------------- //
char	*trim_quotes(t_shell *shell, char *src, int len);
void	update_quote_state(bool *d_quote, bool *s_quote, char c);
bool	tokenize_input(t_shell *shell, char *input);

// --------------  token_utils  ------------------------------------------- //
t_t_typ	identify_special_token(char *str);
int		get_special_length(char *str);
int		get_token_length(char *input, int *quote);
t_tok	*add_token(t_shell *shell, t_tok **lst, char *content, t_t_typ type);

// --------------  command runner  ---------------------------------------- //
void	execute(t_shell *shell);
void	execute_single_builtin(t_shell *shell, t_b_typ type);
void	execute_with_pipeline(t_shell *shell, size_t cmd_count);

// --------------  builtins  ---------------------------------------------- //
t_b_typ	identify_builtin(char *str);
void	(*get_builtin(t_b_typ type))(t_shell *, t_cmd *);
void	builtin_echo(t_shell *shell, t_cmd *cmd);
void	builtin_cd(t_shell *shell, t_cmd *cmd);
void	builtin_pwd(t_shell *shell, t_cmd *cmd);
void	builtin_export(t_shell *shell, t_cmd *cmd);
void	builtin_unset(t_shell *shell, t_cmd *cmd);
void	builtin_env(t_shell *shell, t_cmd *cmd);
void	builtin_exit(t_shell *shell, t_cmd *cmd);

#endif
