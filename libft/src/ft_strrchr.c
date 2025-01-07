/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:58:48 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/22 12:27:54 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*tmp;

	tmp = NULL;
	while (*s)
	{
		if (*s == (char)c)
			tmp = s;
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	else
		return ((char *)tmp);
}
/*
int	main(int ac, char **av)
{
	char *ptr;

	ptr = ft_strrchr(av[1], av[2][0]);
	if (ac < 3)
	{
		printf("Usage: %s <string> <character>\n", av[0]);
		return (1);
	}
	if (ptr != NULL)
	{
		printf("Character '%c' found at position: %ld\n", av[2][0], ptr - av[1]);
	}
	else
		printf("Character '%c' not found in the string.\n", av[2][0]);
	return (0);
}*/
