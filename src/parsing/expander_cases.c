/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_cases.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:13:55 by xgossing          #+#    #+#             */
/*   Updated: 2025/03/04 20:29:11 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Expand a given environment variable ($VAR) by replacing it with its value.
*/

bool	expand_env_variable(t_shell *shell, char **output, char *input,
		int *index)
{
	char	*var_value;
	char	*new_output;

	var_value = get_env_value(shell, input);
	new_output = safe_strjoin(shell, *output, var_value);
	*output = new_output;
	*index += ft_strlen(input) + 1;
	return (true);
}

/*
**	Expand $?, replacing it with the exit status of the last command.
*/

bool	expand_exit_status(t_shell *shell, char **output, char *input,
		int *index)
{
	char	*status_str;
	char	*new_str;

	(void)input;
	status_str = ft_itoa(shell->status);
	if (!status_str)
		error_exit(shell, NO_MEM, "expand_exit_status", EXIT_FAILURE);
	alloc_tracker_add(&shell->alloc_tracker, status_str, 0);
	new_str = safe_strjoin(shell, *output, status_str);
	*output = new_str;
	(*index) += 2;
	return (true);
}

bool	expand_noop(t_shell *shell, char **output, char *input, int *index)
{
	(void)shell;
	(void)output;
	(*index) += ft_strlen(input) + 1;
	return (true);
}

/*
**	Handle expanding a $-variable found in the input string:
**	- If result == 1: expand the environment variable using expand_env_variable.
**	- If result == 2: expand the exit status using expand_exit_status.
**	- If result == 0: skip expansion, as the variable does not exist.
*/

static bool	handle_expansion(t_shell *shell, char **output, char *input,
		int *index)
{
	t_expander	expander[3];
	int			result;

	if (!input)
		return (false);
	expander[0] = expand_noop;
	expander[1] = expand_env_variable;
	expander[2] = expand_exit_status;
	result = find_or_check_env(shell, input, index, false);
	if (result == 0 || result == 1 || result == 2)
		return (expander[result](shell, output, &input[*index + 1], index));
	return (true);
}

/*
**	Expand all $-variables in the user input. Allows dynamic substitition
**	of environment variables before command execution.
*/

bool	expand_dollar_variables(t_shell *shell, char **input)
{
	bool	d_quote;
	bool	s_quote;
	char	*output;
	int		i;

	output = safe_strdup(shell, "");
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
