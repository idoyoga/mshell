/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:39:27 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/27 15:28:56 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Check if var_name matches the name of an environment variable inside
**	env_entry and returns its length if it does.
*/

int	match_env_variable(char *var_name, char *env_entry)
{
	int	i;

	i = -1;
	while (var_name[++i] && (ft_isalnum(var_name[i]) || var_name[i] == '_'))
	{
		if (var_name[i] != env_entry[i])
			return (0);
	}
	if (env_entry[i] == '=' || env_entry[i] == '\0')
		return (i);
	return (0);
}

/*
**	Find an environment variable in the list or check if it exists:
**	- If check == true:
**		- Return 0 if the variable does not exist
**		- Return 1 if the variable is found
**	- If check == false:
**		- Return 0 if the variable does not exist
**		- Return 1 if the variable is found
**		- Return 2 if the variable is $?
*/

int	find_or_check_env(t_shell *shell, char *input, int *index, bool check)
{
	t_env	*node;
	int		len;

	if (!shell->env || !input || (check && !*input))
		return (0);
	if (!check && input[*index] == '$' && input[*index + 1] == '?')
		return (2);
	node = shell->env;
	while (node)
	{
		if (check)
			len = match_env_variable(input, node->data);
		else
			len = match_env_variable(&input[*index + 1], node->data);
		if (len > 0)
			return (1);
		node = node->next;
		if (node == shell->env)
			break ;
	}
	return (0);
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
