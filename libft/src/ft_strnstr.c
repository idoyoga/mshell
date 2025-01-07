/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:56:23 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/22 12:27:39 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	if (!big && !len)
		return (NULL);
	if (!*little)
		return ((char *)big);
	while (*big && len--)
	{
		i = 0;
		while (*(big + i) == *(little + i) && *(little + i) && i <= len)
		{
			if (!*(little + i + 1))
				return ((char *)big);
			i++;
		}
		big++;
	}
	return (NULL);
}
/*
int	main()
{
	char	str[] = "see FF your FF return FF now FF";
	char	sub[] = "FF";
	char	*result = ft_strnstr(str, sub, sizeof(str));
		
	if (result != NULL)
		printf("Substring found at index: %ld\n", result - str);
	else
		printf("Substring not found.\n");
}*/
