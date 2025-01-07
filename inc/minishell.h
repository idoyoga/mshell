/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:51:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/07 14:28:16 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define CMD_NOT_FOUND 127
# define CMD_NOT_EXEC 126
# define MAX_ALLOCS 3000

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
# include "../libft/inc/libft.h"
# include "errors.h"

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
	int		argc;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_cmd	*cmd;
	t_env	*env;
	int		env_count;
	char	*work_dir;
	char	*old_work_dir;
	void	**allocs;
	int		alloc_count;

}	t_shell;

// --------------  clean  --------------------------------------- //
void	free_env_node(t_env **node);
void	free_env_list(t_env **env_list);
void	clean_shell(t_shell *shell);

// --------------  errors  -------------------------------------- //
bool	error(t_shell *shell, char *error, int status);

// --------------  init_env  ------------------------------------ //
t_env	*init_env(t_shell *shell, char **env);

// --------------  init_shell  ---------------------------------- //
bool	init_shell(t_shell *shell, char **env);

// --------------  init_wd  ------------------------------------- //
char	*get_env_value(t_env *env, char *key);
bool	init_wd(t_shell *shell);
#endif
