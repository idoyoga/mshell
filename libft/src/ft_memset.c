/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:42:41 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 14:33:07 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	uc;
	unsigned char	*us;

	i = 0;
	uc = (unsigned char)c;
	us = (unsigned char *)s;
	if (n == 0)
		return (s);
	while (i < n)
	{
		us[i] = uc;
		i++;
	}
	return (s);
}
/*
int	main()
{
	char str[30] = "Let's test also this stuff";
	int	c = 'X';
	size_t n = 10;

	printf("Original string: %s\n", str);
	ft_memset(str, c, n);
	printf("Modified string: %s\n", str);
}*/
