/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 16:42:23 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/22 12:25:43 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
/*
int	main()
{
	char	str[] = "teste";
	char	chr = 'e';
	char	*result = ft_strchr(str, chr);

	if (result != NULL)
		printf("Char found at: %s\n", result);
	else
		printf("Char not found.\n");
}*/
