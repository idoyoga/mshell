/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:43:30 by dplotzl           #+#    #+#             */
/*   Updated: 2025/02/22 12:44:39 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*new;
	char	*temp;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	new = (char *)malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!new)
		return (NULL);
	temp = new;
	while (*s1)
		*temp++ = *s1++;
	while (*s2)
		*temp++ = *s2++;
	*temp = '\0';
	return (new);
}

char	*ft_strjoin_four(char const *s1, char const *s2, char const *s3,
			char const *s4)
{
	size_t	len;
	char	*new;
	char	*temp;

	if (!s1 || !s2 || !s3 || !s4)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + ft_strlen(s4);
	new = (char *)ft_calloc(len + 1, sizeof(char));
	if (!new)
		return (NULL);
	temp = new;
	while (*s1)
		*temp++ = *s1++;
	while (*s2)
		*temp++ = *s2++;
	while (*s3)
		*temp++ = *s3++;
	while (*s4)
		*temp++ = *s4++;
	*temp = '\0';
	return (new);
}
