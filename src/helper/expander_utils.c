/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:39:27 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/06 14:35:37 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Identify the end of an environment variable
*/

static int	end_env_var(char *var_name, char *env_entry)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (var_name[i] && (ft_isalnum(var_name[i]) || var_name[i] == '_'))
		++i;
	equal_sign = ft_strchr(env_entry, '=');
	if (!equal_sign)
		return (0);
	if (equal_sign - env_entry == i)
		return (i);
	return (0);
}

/*
**	Find an environment variable in the list:
**	- Return 0 if the variable does not exist
**	- Return 1 if the variable is found
**	- Return 2 if the variable is $?
*/

int	find_env_variable(t_shell *shell, char *input, int *index)
{
	t_env	*node;
	int		len;

	if (!input || !input[*index])
		return (0);
	if (input[*index + 1] == '?')
		return (2);
	node = shell->env;
	while (node)
	{
		len = end_env_var(&input[*index + 1], node->data);
		if (len > 0 && ft_strncmp(node->data, &input[*index + 1], len) == 0
			&& node->data[len] == '=')
		{
			*index += len + 1;
			return (1);
		}
		node = node->next;
		if (node == shell->env)
			break ;
	}
	return (0);
}

/*
**	Check if an environment variable exists
*/

bool	env_variable_exists(t_shell *shell, const char *var_name)
{
	t_env	*node;
	size_t	len;
	size_t	data_len;

	if (!var_name || !*var_name || !shell->env)
		return (false);
	len = 0;
	while (var_name[len] && (ft_isalnum(var_name[len]) || var_name[len] == '_'))
		len++;
	node = shell->env;
	while (node)
	{
		data_len = ft_strlen(node->data);
		if (data_len >= len && ft_strncmp(node->data, var_name, len) == 0)
		{
			if ((data_len == len) || (node->data[len] == '='))
				return (true);
		}
		node = node->next;
		if (node == shell->env)
			break ;
	}
	return (false);
}

/*
**	Append a character to a string
*/

bool	append_char_to_str(t_shell *shell, char **output, int *index, char *c)
{
	char	char_str[2];
	char	*new_str;

	char_str[0] = *c;
	char_str[1] = '\0';
	new_str = safe_strjoin(shell, *output, char_str);
	if (!new_str)
		return (error(NO_MEM, false));
	*output = new_str;
	(*index)++;
	return (true);
}
