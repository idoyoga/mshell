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

static int	end_env_var(char *var_name, char *env_entry)
{
	int	i;

	i = 0;
	while (var_name[i] && (ft_isalnum(var_name[i]) || var_name[i] == '_'))
		++i;
	if (ft_strchr(env_entry, '=') - env_entry == i)
		return (i);
	return (0);
}

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
	}
	return (0);
}

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
