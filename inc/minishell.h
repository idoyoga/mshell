/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:51:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 20:48:34 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define DEFAULT_ALLOC_CAPACITY 100
# define PROMPT_MESSAGE "minishell > "

# include "../libft/inc/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal;

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
}								t_t_typ;

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
}								t_b_typ;

typedef enum e_error
{
	INV_ARGS,
	NO_SHELL,
	NO_MEM,
	NO_ENV,
	GETCWD,
	NO_WD,
	NO_PROMPT,
	NO_USER,
	NO_HOME,
	NO_ALLOC,
	QUOTES,
	NO_RESIZE,
	NO_TRACK,
	START_PIPE,
	END_PIPE,
	CONSEC_PIPES,
	CONSEC_REDIR,
	NO_CMD,
	BACKSLASH,
	SEMICOLON,
	FILE_ERR,
	SYNTAX,
	INV_REDIR,
	NO_REMOVE,
	NO_EXPAND,
	NO_PIPE,
	NO_DUP,
	NO_DUP2,
	BAD_EXEC,
	BAD_WAIT,
	NO_FORK,
	EXIT_INVALID_ARGUMENT,
	EXIT_TOO_MANY_ARGUMENTS,
	PWD_NO_CWD,
	CD_TOO_MANY_ARGUMENTS,
	CD_NO_HOME,
	EXEC_NOT_FOUND,
	EXEC_NO_FILE,
	EXEC_IS_DIRECTORY,
	EXEC_PERMISSION_DENIED,
	TOTAL
}								t_error;

typedef enum e_access_status
{
	A_NOOP,
	A_NOT_FOUND,
	A_PERMISSION_DENIED,
	A_IS_DIRECTORY,
	A_CAN_EXECUTE
}								t_acc_t;

typedef enum e_quoted_status
{
	QUOTE_NONE = 0,
	QUOTE_SINGLE = '\'',
	QUOTE_DOUBLE = '"'
}								t_qstat;

typedef struct s_env
{
	char						*data;
	struct s_env				*next;
	struct s_env				*prev;
}								t_env;

typedef struct s_cmd
{
	char						*cmd;
	char						**args;
	int							argc;
	int							fd_in;
	int							fd_out;
	t_acc_t						access_status;
	pid_t						child_pid;
	bool						skip;
	struct s_cmd				*next;
	struct s_cmd				*prev;
}								t_cmd;

typedef struct s_tok
{
	char						*content;
	char						*file;
	t_t_typ						type;
	bool						is_quoted;
	bool						is_null;
	bool						first_cmd;
	struct s_tok				*next;
	struct s_tok				*prev;
}								t_tok;

typedef struct s_shell			t_shell;
/*
**	Meta-tracking allocations, used to keep track of all allocated memory
**	including the shell struct itself.
*/

typedef struct s_alloc_tracker
{
	void						**allocs;
	int							*is_array;
	int							count;
	int							capacity;
	bool						initialized;
	t_shell						*shell;
}								t_alloc;

typedef struct s_shell
{
	t_cmd						*cmd;
	t_env						*env;
	t_tok						*tokens;
	t_alloc						alloc_tracker;
	size_t						cmd_count;
	int							env_count;
	int							status;
	bool						abort;
	char						*prompt;
	char						*cmd_input;
	char						*home_dir;
	char						*work_dir;
	char						*old_work_dir;
	char						*user;
	char						**path_segments;
	char						**env_as_array;
	int							fd_copies[2];
}								t_shell;

typedef struct s_builtin
{
	t_b_typ						type;
	char						*name;
	void						(*fn)(t_shell *shell, t_cmd *cmd);
}								t_bltin;

typedef bool					(*t_expander)(t_shell *shell, char **output,
						char *input, int *index);

// --------------  alloc  ------------------------------------------------- //
void							*alloc_tracker_add(t_alloc *tracker, void *ptr,
									int is_array);
void							alloc_tracker_remove(t_alloc *tracker,
									void *ptr);
void							free_allocs(t_alloc *tracker);

// --------------  alloc_helper  ------------------------------------------ //
void							*safe_malloc(t_shell *shell, size_t size);
void							*safe_calloc(t_shell *shell, size_t count,
									size_t size);
char							*safe_strdup(t_shell *shell, const char *src);
char							*safe_strndup(t_shell *shell, const char *src,
									size_t n);
char							*safe_strjoin(t_shell *shell, const char *s1,
									const char *s2);

// --------------  alloc_helper  ------------------------------------------ //
void							*alloc_tracker_replace(t_alloc *tracker,
									void *old_ptr, void *new_ptr);

// --------------  clean  ------------------------------------------------- //
void							cleanup_fds(t_cmd *cmd);
void							clean_shell(t_shell *shell);

// --------------  cmd_parser  -------------------------------------------- //
bool							parse_commands(t_shell *shell);

// --------------  cmd_redir  --------------------------------------------- //
bool							handle_redirection(t_shell *shell, t_tok *token,
									t_cmd *cmd);

// --------------  cmd_utils  --------------------------------------------- //
t_cmd							*add_cmd(t_shell *shell, t_cmd **lst);
void							skip_invalid_command(t_shell *shell,
									t_tok **current);
t_t_typ							determine_token_type(t_tok **lst);

// --------------  env_helper  -------------------------------------------- //
char							*get_env_value(t_shell *shell, char *key);
char							**create_default_env(t_shell *shell);
bool							upsert_env_variable(t_shell *shell, t_env **lst,
									char *key, char *new_value);

// --------------  env_utils  --------------------------------------------- //
t_env							*add_env_variable(t_shell *shell, t_env **lst,
									char *data);
bool							remove_env_variable(t_shell *shell, t_env **lst,
									char *var_name);

// --------------  error  ------------------------------------------------- //
bool							error(t_error err, int status);
bool							error_context(t_error err, char *context,
									int status);
void							error_exit(t_shell *shell, t_error err,
									char *context, int status);
void							error_exit_s(t_shell *shell, t_error err,
									char *context, int status);
void							strerror_cmd(const char *cmd_name);

// --------------  expander  ---------------------------------------------- //
bool							expand_dollar_variables(t_shell *shell,
									char **input);
bool							expand_dilla_variables(t_shell *shell);
bool							expand_env_variable(t_shell *shell,
									char **output, char *input, int *index);
bool							expand_exit_status(t_shell *shell,
									char **output, char *input, int *index);
bool							expand_noop(t_shell *shell, char **output,
									char *input, int *index);
void							strip_redir_token_quotes(t_shell *shell,
									t_tok *current_token);

bool							should_expand_dollar(char *input, int i,
									bool s_quote);

// --------------  expander_utils  ----------------------------------------
//
int								find_or_check_env(t_shell *shell, char *input,
									int *index, bool check);
bool							env_variable_exists(t_shell *shell,
									char *input);
int								match_env_variable(char *var_name,
									char *env_entry);
bool							append_char_to_str(t_shell *shell,
									char **output, int *index, char *c);
bool							is_quote(char c);
bool							is_valid_var_char(char c, size_t index);

// --------------  heredoc  ----------------------------------------------- //
int								handle_heredoc(t_shell *shell,
									const char *delimiter, bool is_quoted);

// --------------  init  -------------------------------------------------- //
bool							init_shell(t_shell *shell, char **env);

// --------------  parser_utils  ------------------------------------------ //
bool							invalid_redirection(t_tok *token);
int								validate_tokens(t_tok *tokens);

// --------------  signal  ------------------------------------------------ //
void							handle_sigint(int sig);
void							handle_sigquit(int sig);
void							handle_heredoc_sigint(int sig);
void							setup_signals(void (*sigint_handler)(int),
									void (*sigquit_handler)(int));

// --------------  token  ------------------------------------------------- //
bool							tokenize(t_shell *shell, t_tok **lst,
									char *input);

// --------------  tokenizer  --------------------------------------------- //
void							update_quote_state(bool *d_quote, bool *s_quote,
									char c);
bool							tokenize_input(t_shell *shell, char *input);

// --------------  token_utils  ------------------------------------------- //
t_t_typ							identify_special_token(char *str);
int								get_special_length(char *str);
int								get_token_length(char *input);
t_tok							*add_token(t_shell *shell, t_tok **lst,
									char *content, t_t_typ type);

// --------------  quote eliminator  -------------------------------------- //
bool							remove_quotes(t_shell *shell);

// --------------  command runner  ---------------------------------------- //
void							dispatch(t_shell *shell);
void							execute_single_builtin(t_shell *shell,
									t_b_typ type);
void							execute_with_pipeline(t_shell *shell,
									t_cmd *command, int *pipe_fd);
void							execute_builtin(t_shell *shell, t_cmd *cmd,
									t_b_typ type);
void							execute_command(t_shell *shell, t_cmd *command);
int								wait_for_children(t_shell *shell,
									t_cmd *command);

void							prepare_execution(t_shell *shell);
void							postpare_execution(t_shell *shell);
char							**get_env_array(t_shell *shell);
void							get_absolute_paths(t_shell *shell);

bool							is_path(char *str);
size_t							count_cmd_args(t_cmd *cmd);

// --------------  builtins  ---------------------------------------------- //
t_b_typ							identify_builtin(char *str);
void						(*get_builtin(t_b_typ type))(t_shell *, t_cmd *);
void							builtin_echo(t_shell *shell, t_cmd *cmd);
void							builtin_cd(t_shell *shell, t_cmd *cmd);
void							builtin_pwd(t_shell *shell, t_cmd *cmd);
void							builtin_export(t_shell *shell, t_cmd *cmd);
void							builtin_unset(t_shell *shell, t_cmd *cmd);
void							builtin_env(t_shell *shell, t_cmd *cmd);
void							builtin_exit(t_shell *shell, t_cmd *cmd);

// TODO: delet
void							print_parsed_data(t_shell *shell);

#endif
