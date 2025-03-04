/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:09:55 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 20:48:24 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*g_error_msgs[TOTAL];

/*
**	Print an error message to stderr and return true if status is not 0.
**	- If error code is valid, print 'minishell: ' followed by the corresponding
**		error message from 'g_error_msgs' to stderr.
**	- Return 'true' if 'status != 0', otherwise return 'false'.
*/

bool	error(t_error err, int status)
{
	if (err < 0 || err >= TOTAL)
		return (false);
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd((char *)g_error_msgs[err], 2);
	return (status != 0);
}

bool	error_context(t_error err, char *context, int status)
{
	if (err < 0 || err >= TOTAL)
		return (false);
	ft_putstr_fd("minishell: ", 2);
	if (context && *context)
	{
		ft_putstr_fd(context, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd((char *)g_error_msgs[err], 2);
	return (status != 0);
}

/*
**	Print an error message and terminate the program.
**	1. Print 'minishell: '.
**	2. If 'context' is provided, print 'context: '.
**	3. Print the corresponding error message from `g_error_msgs`.
**	4. Append system error message ('strerror(errno)') for additional details.
**	5. Cleanup the shell before exit with status.
*/

void	error_exit(t_shell *shell, t_error err, char *context, int status)
{
	if (err < 0 || err >= TOTAL)
		return ;
	ft_putstr_fd("minishell: ", 2);
	if (context && *context)
	{
		ft_putstr_fd(context, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd((char *)g_error_msgs[err], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
	if (shell)
		clean_shell(shell);
	exit(status);
}

void	error_exit_s(t_shell *shell, t_error err, char *context, int status)
{
	if (err < 0 || err >= TOTAL)
		return ;
	ft_putstr_fd("minishell: ", 2);
	if (context && *context)
	{
		ft_putstr_fd(context, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd((char *)g_error_msgs[err], 2);
	if (shell)
		clean_shell(shell);
	exit(status);
}

/*
**	Print an error message for a command that failed to execute.
*/

void	strerror_cmd(const char *cmd_name)
{
	char	*err_msg;

	err_msg = ft_strjoin_four("minishell: ", cmd_name, ": ", strerror(errno));
	if (!err_msg)
		error(NO_MEM, false);
	ft_putendl_fd(err_msg, 2);
	free(err_msg);
}

/*
**	Static const char array of predefined error messages.
**	- Array stores error messages for various shell errors.
**	- Each message corresponds to an enum error code ('t_error').
*/

static const char	*g_error_msgs[TOTAL] = {"Too many arguments (max 1)",
	"Shell initialization failed",
	"Memory allocation failed",
	"Failed to initialize environment",
	"Failed to get current working directory",
	"Failed to get working directory",
	"Failed to create prompt",
	"Failed to get user",
	"Failed to get home directory",
	"Failed to add allocation",
	"Quotes not closed",
	"Failed to resize allocation tracker",
	"Failed to initialize allocation tracker",
	"Input cannot start with pipe",
	"Input cannot end with pipe",
	"Consecutive pipes not allowed",
	"Consecutive redirections not allowed",
	"No command to execute",
	"Backslash is not interpreted",
	"Semicolon is not interpreted",
	"Failed to open file",
	"Syntax error",
	"Invalid redirection type",
	"Pointer not found in allocation tracker",
	"Failed to expand variable",
	"Pipe could not be acquired",
	"File descriptor could not be duplicated (1)",
	"File descriptor could not be duplicated (2)",
	"Execution unsuccessful",
	"Waiting for subprocess unsuccessful",
	"Fork unsuccessful",
	"numeric argument required",
	"too many arguments",
	"Failed to run getcwd",
	"too many arguments",
	"HOME not set",
	"command not found",
	"No such file or directory",
	"Is a directory",
	"Permission denied"};
