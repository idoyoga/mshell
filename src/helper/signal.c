/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:47:06 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/05 02:34:15 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

/*
**	Signal handler for regular signals (SIGINT)
*/

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = sig;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

/*
**	Signal handler for heredoc signals (SIGINT)
*/

void	handle_heredoc_sigint(int sig)
{
	char	cr;

	cr = '\n';
	if (sig == SIGINT)
	{
		g_signal = sig;
		ioctl(STDIN_FILENO, TIOCSTI, &cr);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

/*
**	Signal handler for SIGQUIT
*/

void	handle_sigquit(int sig)
{
	char	cr;

	cr = '\n';
	if (sig == SIGQUIT)
	{
		g_signal = sig;
		ioctl(STDIN_FILENO, TIOCSTI, &cr);
		write(2, "Quit (core dumped)", 18);
		exit (131);
	}
}

/*
**	Set up signal handlers for SIGINT and SIGQUIT (ignored by default)
**	-> SIGQUIT needs to be set in execute() with:
**		'setup_signals(handle_sigint, SIG_DFL);'
*/

void	setup_signals(void (*sigint_handler)(int), void (*sigquit_handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = sigquit_handler;
	sigaction(SIGQUIT, &sa, NULL);
}
