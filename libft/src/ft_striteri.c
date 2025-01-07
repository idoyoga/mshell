/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:08:37 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 15:09:43 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (i < ft_strlen(s))
	{
		f(i, &s[i]);
		i++;
	}
}
/*
void	test_function(unsigned int i, char *chr)
{
	printf("Index: %u, Character: %c\n", i, *chr);
}

int	main()
{
	char	test_str[] = "Let's test this stuff!";

	ft_striteri(test_str, &test_function);
}*/
