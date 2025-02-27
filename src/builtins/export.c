/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:12 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/27 15:32:41 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(char *name)
{
	size_t	i;

	i = 0;
	if (!name) // should this ever happen?
		return (false);
	if (name[0] >= '0' && name[0] <= '9')
		return (false);
	while (name[i] != '\0' && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	export_print(t_env *env)
{
	t_env	*current;

	if (env == NULL)
		return ;
	current = env;
	while (current)
	{
		printf("declare -x %s\n", current->data);
		current = current->next;
		if (current == env)
			break ;
	}
}

void	builtin_export(t_shell *shell, t_cmd *cmd)
{
	size_t	argument_count;
	size_t	i;
	char	*tmp;

	argument_count = count_cmd_args(cmd);
	if (argument_count == 1)
		return (export_print(shell->env));
	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			printf("export: `%s': not a valid identifier\n", cmd->args[i]);
		}
		else
		{
			if (!find_or_check_env(shell, cmd->args[i], NULL, true))
			{
				tmp = safe_strdup(shell, cmd->args[i]);
				add_env_variable(shell, &shell->env, tmp);
			}
		}
		i++;
	}
}
