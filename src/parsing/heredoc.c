/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:06:25 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 18:57:05 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Print warning message when EOF is reached before delimiter,
**	imitating original bash behavior.
*/

static void	print_eof_warning(int line_count, const char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line_count, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd((char *)delimiter, 2);
	ft_putendl_fd("')", 2);
}

/*
**	Read user input into heredoc_filename until the delimiter is found,
**	Ctrl+D (EOF) or Ctrl+C (SIGINT) is pressed. Ctrl+C sets g_signal to 1.
*/

static bool	heredoc_read_input(t_shell *shell, const char *delimiter, int fd,
		bool is_quoted)
{
	char	*line;
	int		line_count;

	line = NULL;
	g_signal = 0;
	line_count = 0;
	setup_signals(handle_heredoc_sigint, SIG_IGN);
	while (!g_signal)
	{
		line = alloc_tracker_replace(&shell->alloc_tracker, line,
				readline("> "));
		line_count++;
		if (!line)
		{
			print_eof_warning(line_count, delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		if (!is_quoted && !expand_dollar_variables(shell, &line))
			error_exit(shell, NO_EXPAND, "heredoc_read_input", EXIT_FAILURE);
		ft_putendl_fd(line, fd);
	}
	setup_signals(handle_sigint, SIG_IGN);
	if (g_signal != 0)
	{
		shell->status = g_signal + 128;
		shell->abort = true;
		return (false);
	}
	return (true);
}

/*
**	Handle heredoc redirection:
**	1. Create unique filename per heredoc for cases like:
**		cat << EOF1 | grep hello << EOF2
**	(not sure if necessary to handle this, open to suggestions)
*/

int	handle_heredoc(t_shell *shell, const char *delimiter, bool is_quoted)
{
	int			fd;
	char		*count_str;
	char		*heredoc_filename;
	static int	heredoc_count = 0;

	count_str = ft_itoa(heredoc_count++);
	if (!count_str)
		return (-1);
	alloc_tracker_add(&shell->alloc_tracker, count_str, 0);
	heredoc_filename = safe_strjoin(shell, "/tmp/.heredoc_", count_str);
	fd = open(heredoc_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	if (!heredoc_read_input(shell, delimiter, fd, is_quoted))
	{
		if (fd >= 3)
			close(fd);
		(unlink(heredoc_filename), setup_signals(handle_sigint, SIG_IGN));
		return (-1);
	}
	close(fd);
	fd = open(heredoc_filename, O_RDONLY);
	if (fd > 0)
		unlink(heredoc_filename);
	return (fd);
}
