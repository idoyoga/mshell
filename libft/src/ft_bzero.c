/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:51:54 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/30 10:49:07 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = s;
	while (i < n)
	{
		*(tmp + i) = '\0';
		i++;
	}
}
/*
int	main(void)
{
	char	str[] = "Let's test this stuff!";

	printf("Before bzero: %s\n", str);
	ft_bzero(str, 20);
	printf("After bzero: %s\n", str);
	return (0);
}*/
