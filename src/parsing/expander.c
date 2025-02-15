/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:37:00 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/07 13:56:18 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Expands the environment expand_dollar_variables
*/

static bool	expand_env_var(t_shell *shell, char **output, char *var, int len)
{
	t_env	*node;
	char	*var_value;
	char	*new_output;

	node = shell->env;
	while (node)
	{
		if (ft_strncmp(node->data, var, len) == 0 && node->data[len] == '=')
		{
			var_value = node->data + len + 1;
			break ;
		}
		node = node->next;
	}
	if (!node)
		return (true);
	new_output = safe_strjoin(shell, *output, var_value);
	if (!new_output)
		return (error(NO_MEM, false));
	*output = new_output;
	return (true);
}

/*
**	Expands the exit status
*/

static bool	expand_exit_status(t_shell *shell, char **output)
{
	char	*status_str;
	char	*new_str;

	status_str = ft_itoa(shell->status);
	if (!status_str || !alloc_tracker_add(&shell->alloc_tracker, status_str, 0))
		return (error(NO_MEM, false));
	new_str = safe_strjoin(shell, *output, status_str);
	if (!new_str)
		return (error(NO_MEM, false));
	*output = new_str;
	return (true);
}

/*
**	Helper to check if the dollar sign should be expanded
*/

static bool	should_expand_dollar(const char *input, int i, bool s_quote)
{
	if (!input[i] || input[i] != '$' || s_quote)
		return (false);
	if (input[i + 1] && (ft_isalpha(input[i + 1]) || input[i + 1] == '?'
			|| input[i + 1] == '_'))
		return (true);
	return (false);
}

/*
**	Handles the expansion of the dollar sign
*/

static bool	handle_expansion(t_shell *shell, char **output, char *input,
								int *index)
{
	int	start;
	int	result;

	start = *index;
	result = find_env_variable(shell, input, index);
	if (result == 1)
		return (expand_env_var(shell, output, &input[start + 1],
				*index - start - 1));
	else if (result == 2)
	{
		(*index) += 2;
		return (expand_exit_status(shell, output));
	}
	(*index)++;
	while ((input[*index]) && (ft_isalnum(input[*index])
			|| input[*index] == '_'))
		(*index)++;
	return (true);
}

/*
**	Expands the dollar sign variables
*/

bool	expand_dollar_variables(t_shell *shell, char **input)
{
	bool	d_quote;
	bool	s_quote;
	char	*output;
	int		i;

	output = safe_strdup(shell, "");
	if (!output)
		return (error(NO_MEM, false));
	d_quote = false;
	s_quote = false;
	i = 0;
	while ((*input)[i])
	{
		update_quote_state(&d_quote, &s_quote, (*input)[i]);
		if (should_expand_dollar(*input, i, s_quote))
		{
			if (!handle_expansion(shell, &output, *input, &i))
				return (false);
			continue ;
		}
		if (!append_char_to_str(shell, &output, &i, &(*input)[i]))
			return (false);
	}
	*input = output;
	return (true);
}
