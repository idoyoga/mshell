/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 10:19:38 by dplotzl           #+#    #+#             */
/*   Updated: 2024/04/26 14:54:37 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (s[start + i] && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
/**
int main(void)
{
    char *s = "Let's test this stuff!";
    char *substr;

    // Test with a valid substring
    substr = ft_substr(s, 7, 5);
    printf("Substring: %s\n", substr);
    free(substr);

    // Test with start index exceeding string length
    substr = ft_substr(s, 20, 5);
    printf("Substring: %s\n", substr);  // Expected: ""
    free(substr);

    // Test with empty string
    substr = ft_substr("", 0, 0);
    printf("Substring: %s\n", substr);  // Expected: ""
    free(substr);

    // Test with NULL string
    substr = ft_substr(NULL, 0, 5);
    printf("Substring: %s\n", substr);  // Expected: NULL

    return 0;
}*/
