/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:40:05 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 14:56:13 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Skipping blank characters
*/

char	*skip_blanks(char *input)
{
	while (ft_isblank(*input))
		input++;
	return (input);
}

/*
**	Handle quotes
*/

char	*handle_quotes(char *input)
{
	char	quote;

	quote = *input; // Store the opening quote character
	input++;		// Move to the next character
	while (*input) // Look for the closing quote
	{
		if (*input == quote) // Closing quote found
			return (input); // Return the position of the closing quote
		input++;
	}
	return (input); // Return pointer if no closing quote is found
}
