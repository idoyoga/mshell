/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:12 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/02 19:47:03 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(char *name)
{
	size_t	i;

	i = 0;
	if (!name) // should this ever happen?
		//
		// I guess not, as function is only called after checking command arguments.
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
	char	*equal_sign;
	char	*key;
	char	*value;

	argument_count = count_cmd_args(cmd);
	if (argument_count == 1)
		return (export_print(shell->env));
	i = 1;
	while (cmd->args[i] != NULL)
	{
		equal_sign = ft_strchr(cmd->args[i], '='); // find the first '=' in the argument
		if (equal_sign) // Case: export KEY=VALUE
		{
			*equal_sign = '\0'; // replace '=' with '\0' to split the string
			key = cmd->args[i];
			value = equal_sign + 1;
			if (is_valid_identifier(cmd->args[i]))
				upsert_env_variable(shell, &shell->env, key, value);
			else
			{
				ft_putstr_fd("export: `", STDERR_FILENO);
				ft_putstr_fd(cmd->args[i], STDERR_FILENO);
				ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			}
			*equal_sign = '='; // restore the original string
		}
		else if (is_valid_identifier(cmd->args[i]))
		{
			upsert_env_variable(shell, &shell->env, cmd->args[i], "");
		}
		else
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		}
		i++;
	}
}
/* void	builtin_export(t_shell *shell, t_cmd *cmd) */
/* { */
/* 	size_t	argument_count; */
/* 	size_t	i; */
/* 	char	*tmp; */

/* 	argument_count = count_cmd_args(cmd); */
/* 	if (argument_count == 1) */
/* 		return (export_print(shell->env)); */
/* 	i = 1; */
/* 	while (cmd->args[i] != NULL) */
/* 	{ */
/* 		if (!is_valid_identifier(cmd->args[i])) */
/* 		{ */
/* 			ft_putstr_fd("export: `", STDERR_FILENO); */
/* 			ft_putstr_fd(cmd->args[i], STDERR_FILENO); */
/* 			ft_putendl_fd("': not a valid identifier\n", STDERR_FILENO); */
/* 		} */
/* 		else */
/* 		{ */
/* 			if (!find_or_check_env(shell, cmd->args[i], NULL, true)) */
/* 			{ */
/* 				tmp = safe_strdup(shell, cmd->args[i]); */
/* 				add_env_variable(shell, &shell->env, tmp); */
/* 			} */
/* 		} */
/* 		i++; */
/* 	} */
/* } */
