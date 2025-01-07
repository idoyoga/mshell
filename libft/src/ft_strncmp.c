/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dp <marvin@42.fr>                          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:28:29 by dp                #+#    #+#             */
/*   Updated: 2024/04/22 12:27:20 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0)
	{
		if (*s1 != *s2)
			return ((unsigned char)(*s1) - (unsigned char)(*s2));
		if (*s1 == '\0' || *s2 == '\0')
			return (0);
		s1++;
		s2++;
		n--;
	}
	return (0);
}
/*
int	main(int ac, char **av)
{
	if (ac == 4)
		printf("%d\n", ft_strncmp(av[1], av[2], av[3][0]));
	else
		printf("Error\n");
}*/
