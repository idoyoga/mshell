/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:20:24 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/15 10:28:29 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
** Create the prompt string, which consists of the user name and the path.
*/

char	*create_prompt(t_shell *shell)
{
	char	*prompt;
	char	*work_dir;
	size_t	len;
	size_t	total_len;

	work_dir = shell->work_dir;
	shell->home_dir = getenv("HOME");
	if (!shell->home_dir)
		return (error(shell, NO_HOME, 1), NULL);
	len = ft_strlen(shell->home_dir);
	if (ft_strncmp(shell->home_dir, work_dir, len) == 0)
		work_dir += len;
	total_len = ft_strlen(shell->user) + ft_strlen(work_dir) + 6;
	prompt = ft_calloc(sizeof(char), total_len);
	alloc_tracker_add(&(shell->alloc_tracker), prompt);
	if (!prompt)
		return (error(shell, NO_MEM, 1), NULL);
	ft_strlcpy(prompt, shell->user, total_len);
	ft_strlcat(prompt, ":~", total_len);
	ft_strlcat(prompt, work_dir, total_len);
	ft_strlcat(prompt, "$ ", total_len);
	return (prompt);
}
