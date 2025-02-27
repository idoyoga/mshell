/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:17 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/27 15:08:21 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_shell *shell, t_cmd *cmd)
{
	t_env	*current;

	(void)cmd;
	current = shell->env;
	while (current)
	{
		// TODO: shold only print variables with an assignment (with a '=')
		printf("%s\n", current->data);
		current = current->next;
		if (current == shell->env)
			break ;
	}
}
