/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:47:06 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/13 18:13:47 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_signal = 0;

/*
**	Signal handler for regular signals (SIGINT)
*/

void	handle_signal(int sig)
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

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 1;
		write(1, "\n", 1);
		unlink_all_heredocs();
	}
}

