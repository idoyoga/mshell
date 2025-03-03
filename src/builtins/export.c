/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:12 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/03 19:10:35 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	has_valid_key(char *token)
{
	size_t	i;

	if (!token[0] || token[0] == '=')
		return (false);
	i = 0;
	while (token[i] && token[i] != '=')
	{
		if (!is_valid_var_char(token[i], i))
			return (false);
		i++;
	}
	return (true);
}

static void	export_goods(t_shell *shell, char *data)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(data, '=');
	if (equal_sign == NULL)
	{
		if (!env_variable_exists(shell, data))
			add_env_variable(shell, &shell->env, safe_strdup(shell, data));
	}
	else
	{
		*equal_sign = '\0';
		key = data;
		value = equal_sign + 1;
		upsert_env_variable(shell, &shell->env, key, value);
		*equal_sign = '=';
	}
}

void	builtin_export(t_shell *shell, t_cmd *cmd)
{
	size_t	argument_count;
	size_t	i;

	argument_count = count_cmd_args(cmd);
	if (argument_count == 1)
		return (export_print(shell->env));
	i = 1;
	while (cmd->args[i] != NULL)
	{
		if (!has_valid_key(cmd->args[i]))
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		}
		else
		{
			export_goods(shell, cmd->args[i]);
		}
		i++;
	}
}
