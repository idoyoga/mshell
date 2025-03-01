/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:37:00 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/28 00:56:05 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Expand a given environment variable ($VAR) by replacing it with its value.
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
			*output = new_output;
			*index += len + 1;
			return (true);
		}
		node = node->next;
	}
	return (true);
}

/*
**	Expand $?, replacing it with the exit status of the last command.
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
	*output = new_str;
	(*index) += 2;
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
**	The function performs the following checks:
** 	- Returns false if '$' is preceded by an alphanumeric character (e.g., 'VAR$USER').
** 	- Returns false if '$' appears inside single quotes ('''), as they prevent expansion.
** 	- Detects if '$' is used within a heredoc ('<<') and determines if expansion should occur:
** 	  - If the heredoc delimiter is quoted ('<< "$VAR"'), expansion is prevented.
** 	  - If the heredoc delimiter is unquoted ('<< VAR'), expansion is allowed.
** 	- Returns true for '$?', as it should always be expanded.
** 	- Ensures only valid variable names are expanded ('$VAR' but not '$1VAR').
** 	- Returns false if the environment variable does not exist.
*/

static bool	should_expand_dollar(t_shell *shell, char *input, int i, bool s_quote)
{
	int	j;
	int	k;

	if (!input[i] || input[i] != '$' || s_quote)
		return (false);
	if (i > 0 && ft_isalnum(input[i - 1]))
		return (false);
	j = i - 1;
	while (j > 0 && (ft_isblank(input[j]) || input[j] == '"' || input[j] == '\''))
		j--;
	if ((j == 1 && input[j] == '<' && input[j - 1] == '<')
			|| (j > 1 && input[j] == '<' && input[j - 1] == '<'))
	{
		k = j + 2;
		while (input[k] && ft_isblank(input[k]))
			k++;
		if (input[k] != '\0')
			return (false);
	}
	if (input[i + 1] == '?')
		return (true);
	if (!input[i + 1] || (!ft_isalpha(input[i + 1]) && input[i + 1] != '_'))
		return (false);
	if (find_or_check_env(shell, &input[i + 1], NULL, true) == 0)
		return (false);
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
