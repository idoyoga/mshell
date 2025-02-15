/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:06:25 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/15 12:46:01 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Read user input into heredoc_filename until the delimiter is found
**	or Ctrl+C (SIGINT) is pressed. Ctrl+C sets g_signal to 1.
*/

static bool	heredoc_read_input(t_shell *shell, int fd, const char *delimiter)
{
	char	*line;

	line = NULL;
	g_signal = 0;
	signal(SIGINT, handle_heredoc_signal);
	while (!g_signal)
	{
		line = readline("> ");
		alloc_tracker_add(&shell->alloc_tracker, line, 0);
		if (!line)
		{
			ft_putstr_fd("minishell: heredoc delimited by EOF (needs `", 2);
			ft_putstr_fd((char *)delimiter, 2);
			ft_putendl_fd("')", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		ft_putendl_fd(line, fd);
	}
	signal(SIGINT, handle_signal);
	return (!g_signal);
}

/*
**	Handle heredoc redirection:
**	1. Create unique filename per heredoc for cases like:
**		cat << EOF1 | grep hello << EOF2
**	(not sure if necessary to handle this, open to suggestions)
*/

int	handle_heredoc(t_shell *shell, const char *delimiter)
{
	int			fd;
	char		*count_str;
	char		*heredoc_filename;
	static int	heredoc_count = 0;

	count_str = ft_itoa(heredoc_count++);
	alloc_tracker_add(&shell->alloc_tracker, count_str, 0);
	heredoc_filename = safe_strjoin(shell, "./.heredoc_", count_str);
	fd = open(heredoc_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	if (!heredoc_read_input(shell, fd, delimiter))
	{
		if (fd >= 3)
			close(fd);
		unlink(heredoc_filename);
		signal(SIGINT, handle_signal);
		return (-1);
	}
	close(fd);
	fd = open(heredoc_filename, O_RDONLY);
	if (fd > 0)
		unlink(heredoc_filename);
	signal(SIGINT, handle_signal);
	return (fd);
}
