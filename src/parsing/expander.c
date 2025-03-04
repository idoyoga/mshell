/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 20:37:00 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 20:29:04 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	should_expand_dollar(char *input, int i, bool s_quote)
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
	while (str[*index + i + 1] && is_valid_var_char(str[*index + i + 1], i))
		i++;
	variable = ft_substr(str, (*index) + 1, i);
	if (!variable)
		error_exit(shell, NO_MEM, "handy_expandy", EXIT_FAILURE);
	alloc_tracker_add(&shell->alloc_tracker, variable, 0);
	check_res = env_variable_exists(shell, variable);
	if (check_res == true)
		expand_env_variable(shell, dest, variable, index);
	else
		expand_noop(shell, dest, variable, index);
}

static void	xpand(t_shell *shell, t_tok *token, char *expanded, int i)
{
	t_qstat	quote;

	quote = QUOTE_NONE;
	while (token->content[i])
	{
		if (quote == QUOTE_NONE && is_quote(token->content[i]))
			quote = token->content[i++];
		else if (quote != QUOTE_NONE
			&& (unsigned char)token->content[i] == quote)
		{
			quote = QUOTE_NONE;
			i++;
		}
		else if (quote != QUOTE_SINGLE && token->content[i] == '$'
			&& token->content[i + 1] == '?')
			expand_exit_status(shell, &expanded, token->content + i, &i);
		else if (quote != QUOTE_SINGLE && token->content[i] == '$'
			&& token->content[i + 1] && is_valid_var_char(token->content[i + 1],
				0))
			handy_expandy(shell, token->content, &expanded, &i);
		else
			append_char_to_str(shell, &expanded, &i, &token->content[i]);
	}
	token->content = expanded;
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
			strip_redir_token_quotes(shell, current_token);
		else if (is_empty_variable(shell, current_token->content))
		{
			current_token->content = NULL;
			current_token->is_null = true;
		}
		else
			xpand(shell, current_token, safe_strdup(shell, ""), 0);
		current_token = current_token->next;
		if (current_token == shell->tokens)
			break ;
	}
	return (true);
}
