/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:37:00 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 14:26:34 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Expand a given environment variable ($VAR) by replacing it with its value.
*/

static bool	expand_env_variable(t_shell *shell, char **output, char *input,
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

static bool	expand_exit_status(t_shell *shell, char **output, char *input,
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

static bool	expand_noop(t_shell *shell, char **output, char *input, int *index)
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
**	Helper to check if the dollar sign should trigger variable expansion:
**	The function performs the following checks:
** 	- Returns false if '$' is preceded
		by an alphanumeric character (e.g., 'VAR$USER').
** 	- Returns false if '$' appears inside single quotes ('''),
		as they prevent expansion.
** 	- Detects if '$' is used within a heredoc ('<<') and determines
		if expansion should occur:
** 		- If the heredoc delimiter is quoted ('<< "$VAR"'),
			expansion is prevented.
** 		- If the heredoc delimiter is unquoted ('<< VAR'), expansion is allowed.
** 	- Returns true for '$?', as it should always be expanded.
** 	- Ensures only valid variable names are expanded ('$VAR' but not '$1VAR').
** 	- Returns false if the environment variable does not exist.
*/

static bool	should_expand_dollar(t_shell *shell, char *input, int i,
		bool s_quote)
{
	int	j;
	int	k;

	if (!input[i] || input[i] != '$' || s_quote)
		return (false);
	if (i > 0 && ft_isalnum(input[i - 1]))
		return (false);
	j = i - 1;
	while (j > 0 && (ft_isblank(input[j]) || input[j] == '"'
			|| input[j] == '\''))
		j--;
	if ((j == 1 && input[j] == '<' && input[j - 1] == '<') || (j > 1
			&& input[j] == '<' && input[j - 1] == '<'))
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

// checks whether a str contains only a variable that is unset
// returns true only if no whitespace or any other surrounding characters
// are found and the variable str expands to isn't set
static bool	is_empty_variable(t_shell *shell, char *str)
{
	size_t	i;

	if (!str[0] || !str[1])
		return (false);
	if (str[0] == '$' && is_valid_var_char(str[1], 0))
	{
		i = 1;
		while (str[i] && is_valid_var_char(str[i], i - 1))
			i++;
		if (str[i] == '\0' && !find_or_check_env(shell, str + 1, NULL, true))
			return (true);
	}
	return (false);
}

static void	handy_expandy(t_shell *shell, char *str, char **dest, int *index)
{
	char	*variable;
	int		check_res;
	int		i;

	i = 0;
	// +1 to skip the '$'
	while (str[*index + i + 1] && is_valid_var_char(str[*index + i + 1], i))
		i++;
	variable = ft_substr(str, (*index) + 1, i);
	// substr of only the valid chars
	if (!variable)
		error_exit(shell, NO_MEM, "handy_expandy", EXIT_FAILURE);
	// printf("looking for variable `%s`\n", variable);
	alloc_tracker_add(&shell->alloc_tracker, variable, 0);
	check_res = env_variable_exists(shell, variable);
	if (check_res == true)
	{
		// printf("found value for `%s`, expanding\n", variable);
		expand_env_variable(shell, dest, variable, index);
	}
	else
	{
		// printf("found nothing for `%s`, noop-ing\n", variable);
		expand_noop(shell, dest, variable, index);
	}
}

void	xpand(t_shell *shell, t_tok *token)
{
	char	*expanded_content;
	int		i;
	t_qstat	quote;

	if (is_empty_variable(shell, token->content))
	{
		token->content = NULL;
		token->is_null = true;
		return ;
	}
	i = 0;
	expanded_content = safe_strdup(shell, "");
	quote = QUOTE_NONE;
	while (token->content[i])
	{
		if (quote == QUOTE_NONE && is_quote(token->content[i]))
		{
			quote = token->content[i];
			append_char_to_str(shell, &expanded_content, &i,
				&token->content[i]);
		}
		else if (quote != QUOTE_NONE && token->content[i] == quote)
		{
			quote = QUOTE_NONE;
			append_char_to_str(shell, &expanded_content, &i,
				&token->content[i]);
		}
		else if (quote != QUOTE_SINGLE && token->content[i] == '$'
			&& token->content[i + 1] == '?')
		{
			expand_exit_status(shell, &expanded_content, token->content + i,
				&i);
		}
		else if (quote != QUOTE_SINGLE && token->content[i] == '$'
			&& token->content[i + 1] && is_valid_var_char(token->content[i + 1],
				0))
		{
			handy_expandy(shell, token->content, &expanded_content, &i);
		}
		else
			append_char_to_str(shell, &expanded_content, &i,
				&token->content[i]);
	}
	token->content = expanded_content;
}

bool	expand_dilla_variables(t_shell *shell)
{
	t_tok	*current_token;

	current_token = shell->tokens;
	while (current_token)
	{
		if ((current_token->type != CMD && current_token->type != ARG)
			|| (current_token->prev && current_token->prev->type != CMD
				&& current_token->prev->type != ARG
				&& current_token->prev->type != PIPE))
		{
		}
		else
		{
			xpand(shell, current_token);
		}
		current_token = current_token->next;
		if (current_token == shell->tokens)
			break ;
	}
	return (true);
}
