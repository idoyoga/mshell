/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 12:31:59 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 15:57:34 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*result;

	result = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s))
	{
		result[i] = (*f)(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
/*
char	test_function(unsigned int i, char c)
{
	if (ft_isalpha(c))
		if (i % 2 == 0)
			return (ft_toupper(c));
		else
			return (c);
	else
		return (c);
}

int	main()
{
	const char	*test_string = "Let us test this stuff!";
	char		*result = ft_strmapi(test_string, test_function);

	if (result)
	{
		printf("Original string: %s\n", test_string);
		printf("Modified string: %s\n", result);
		free(result);
	}
	else
		printf("Malloc failed!\n");
}*/
