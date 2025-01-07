/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:50:27 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/25 22:28:19 by dp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	compute(int n)
{
	int		i;

	i = 0;
	if (n < 0)
	{
		i++;
		n = -n;
	}
	while (n > 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

static char	*malloc_and_result(int n, int len)
{
	char	*result;

	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	if (n < 0)
	{
		n = -n;
		result[0] = '-';
	}
	result[len] = '\0';
	while (n > 0)
	{
		result[len - 1] = (n % 10) + '0';
		n /= 10;
		len--;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	int	len;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	len = compute(n);
	return (malloc_and_result(n, len));
}
/*
int main()
{
	int	int1 = 0;
	int int2 = -2147483648;
	int	int3 = 2147483647;
	int	int4 = -1234;
	int	int5 = 156;

	printf("Should be 0: %s\n", ft_itoa(int1));
	printf("Should be -2147483648: %s\n", ft_itoa(int2));
	printf("Should be 2147483647: %s\n", ft_itoa(int3));
	printf("Should be -1234: %s\n", ft_itoa(int4));
	printf("Should be 156: %s\n", ft_itoa(int5));
}*/
