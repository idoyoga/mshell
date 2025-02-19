/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:37:00 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/19 19:38:40 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Expands a given environment variable ($VAR) by replacing it with its value.
*/

static bool	expand_env_variable(t_shell *shell, char **output, char *input,
							int *index)
{
	t_env	*node;
	char	*var_value;
	char	*new_output;
	int		len;

	node = shell->env;
	while (node)
	{
		len = match_env_variable(input, node->data);
		if (len > 0)
		{
			var_value = node->data + len + 1;
			new_output = safe_strjoin(shell, *output, var_value);
			if (!new_output)
				return (error(NO_MEM, false));
			*output = new_output;
			*index += len + 1;
			return (true);
		}
		node = node->next;
	}
	return (true);
}

/*
**	Expands $?, replacing it with the exit status of the last command.
*/

static bool	expand_exit_status(t_shell *shell, char **output, char *input,
								int *index)
{
	char	*status_str;
	char	*new_str;

	(void)input;
	status_str = ft_itoa(shell->status);
	if (!status_str || !alloc_tracker_add(&shell->alloc_tracker, status_str, 0))
		return (error(NO_MEM, false));
	new_str = safe_strjoin(shell, *output, status_str);
	if (!new_str)
		return (error(NO_MEM, false));
	*output = new_str;
	(*index) += 2;
	return (true);
}

/*
**	Handles expanding a $-variable found in the input string:
**	- If result == 1: expands the environment variable using expand_env_variable.
**	- If result == 2: expands the exit status using expand_exit_status.
**	- If result == 0: skips expansion, as the variable does not exist.
*/

static bool	handle_expansion(t_shell *shell, char **output, char *input,
								int *index)
{
	t_expander	expander[2];
	int			result;

	if (!input)
		return (false);
	expander[0] = expand_env_variable;
	expander[1] = expand_exit_status;
	result = find_or_check_env(shell, input, index, false);
	if (result == 1 || result == 2)
		return (expander[result - 1](shell, output, &input[*index + 1],
				index));
	return (true);
}

/*
**	Helper to check if the dollar sign should trigger variable expansion:
**	- Skip expansion inside single quotes.
**	- Skip expansion if the character before '$' is alphanumeric.
**	- Expand $? as a special variable for exit status.
**	- Check if the next character is a valid start for an environment variable.
**	- Use find_or_check_env to verify if the environment variable exists.
*/

static bool	should_expand_dollar(t_shell *shell, char *input, int i,
									bool s_quote)
{
	if (!input[i] || input[i] != '$' || s_quote)
		return (false);
	if (i > 0 && ft_isalnum(input[i - 1]))
		return (false);
	if (input[i + 1] == '?')
		return (true);
	if (!input[i + 1] || (!ft_isalpha(input[i + 1]) && input[i + 1] != '_'))
		return (false);
	if (find_or_check_env(shell, &input[i + 1], NULL, true) == 0)
		return (false);
	return (true);
}

/*
**	Expands all $-variables in the user input. Allows dynamic substitition
**	of environment variables before command execution.
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
		if (should_expand_dollar(shell, *input, i, s_quote))
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
