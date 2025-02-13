/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:06:25 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/13 18:27:37 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Delete all active heredocs when Ctrl+C is pressed.
**	Prevents leftover heredoc files in /tmp/ and ensures that
**	all heredocs closed at once.
*/

void	unlink_all_heredocs(void)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*filepath;

	dir = opendir("/tmp");
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strncmp(entry->d_name, ".heredoc_", 9) == 0)
		{
			filepath = safe_strjoin(NULL, "/tmp/", entry->d_name);
			unlink(filepath);
		}
		entry = readdir(dir);
	}
	closedir(dir);
}

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
		if (!line)
		{
			ft_putstr_fd("minishell: heredoc delimited by EOF (needs `", 2);
			ft_putstr_fd((char *)delimiter, 2);
			ft_putendl_fd("')", 2);
			break ;
		}
		alloc_tracker_add(&shell->alloc_tracker, line, 0);
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		ft_putendl_fd(line, fd);
	}
	close(fd);
	signal(SIGINT, handle_signal);
	return (!g_signal);
}

/*
**	Handle heredoc redirection:
**	1. Create unique filename per heredoc so each shell has its own heredoc.
**	2. Set custom signal handler for heredoc 
**	3. Read user input into the file until the delimiter is found or Ctrl+C.
**	4. Restore the default signal handler.
**	5. Open the temporary file for reading.
**	6. Unlink the temporary file.
**	7. Return the file descriptor for input redirection.
*/

int	handle_heredoc(t_shell *shell, const char *delimiter)
{
	int		fd;
	char	*pid_str;
	char	*heredoc_filename;

	pid_str = ft_itoa(getpid());
	alloc_tracker_add(&shell->alloc_tracker, pid_str, 0);
	heredoc_filename = safe_strjoin(shell, "/tmp/.heredoc_", pid_str);
	fd = open(heredoc_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("heredoc"), -1);
	if (!heredoc_read_input(shell, fd, delimiter))
	{
		unlink(heredoc_filename);
		signal(SIGINT, handle_signal);
		return (-1);
	}
	fd = open(heredoc_filename, O_RDONLY);
	if (fd > 0)
		unlink(heredoc_filename);
	return (fd);
}
