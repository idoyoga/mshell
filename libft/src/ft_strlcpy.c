/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:12:39 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 15:20:41 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
}
/*
int	main(void)
{
	char    src[] = "Let's test";
    char    src1[] = "this stuff!";
    char    dest[5];
    char    dest1[6];

    printf("%zu\n", ft_strlcpy(dest, src, 5));
    printf("%zu\n", ft_strlcpy(dest1, src1, 6));
}*/
