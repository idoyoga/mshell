/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:08 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 18:49:01 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(t_shell *shell, t_cmd *cmd)
{
	size_t	i;

	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (ft_strchr(cmd->args[i], '=') == NULL)
		{
			remove_env_variable(shell, &shell->env, cmd->args[i]);
		}
		i++;
	}
	shell->status = 0;
}
