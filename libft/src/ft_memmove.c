/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 14:10:28 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 14:27:01 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (dest == src || n == 0)
		return (dest);
	if (d < s)
	{
		while (n--)
			*d++ = *s++;
	}
	else
	{
		d += n;
		s += n;
		while (n--)
			*--d = *--s;
	}
	return (dest);
}
/*
int	main()
{
	char	str1[] = "Let's test this stuff";
	char	str2[] = "..and let's also test this stuff";

	printf("Ex-ante ft_memmove:\n");
	printf("str1: %s\n", str1);
	printf("str2: %s\n", str2);

	ft_memmove(str1 + 11, str1, 10);
	ft_memmove(str2, str2 + 8, 10);

	printf("\n");
	printf("Ex-post ft_memmove:\n");
	printf("str1: %s\n", str1);
	printf("str2: %s\n", str2);
}*/
