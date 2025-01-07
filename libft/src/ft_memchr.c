/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:41:48 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 12:52:31 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	uc;

	i = 0;
	p = (unsigned char *)s;
	uc = (unsigned char)c;
	while (i < n)
	{
		if (uc == p[i])
			return ((void *)(p + i));
		i++;
	}
	return (NULL);
}
/*
int	main(void)
{
	const char	*str = "Let's test this stuff";
	char		*result;

	result = ft_memchr(str, 'h', ft_strlen(str));
	if (result != NULL)
		printf("Found 'h' at pos: %ld\n", result - str);
	else
		printf("Not found\n");

	result = ft_memchr(str, ' ', ft_strlen(str));
	if (result != NULL)
		printf("Found ' ' at pos: %ld\n", result - str);
	else
		printf("Not found\n");
}*/
