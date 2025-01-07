/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:50:36 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/25 14:37:36 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	whitespaces(int n)
{
	return ((n >= 9 && n <= 13) || (n == 32));
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	n;
	int	i;

	sign = 1;
	n = 0;
	i = 0;
	while (whitespaces(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		sign = sign - (2 * (nptr[i++] == '-'));
	while (nptr[i] >= '0' && nptr[i] <= '9')
		n = n * 10 + sign * (nptr[i++] - '0');
	return (n);
}
/*
int	main(int ac, char **av)
{
	(void)ac;
	printf("ft_atoi result: %d\n", ft_atoi(av[1]));
	printf("atoi result   : %d\n", atoi(av[1]));
	return (0);
}*/
