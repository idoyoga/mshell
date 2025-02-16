/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:47:06 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/16 19:40:08 by dplotzl          ###   ########.fr       */
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
		g_signal = 1;
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
	if (sig == SIGINT)
	{
		g_signal = 1;
		write(1, "\n", 1);
	}
}

/*
**	Set up signal handlers for SIGINT and SIGQUIT (which is ignored)
*/

void	setup_signals(void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
