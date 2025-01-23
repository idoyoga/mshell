/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:43:16 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/22 18:32:17 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dupl;
	size_t	i;

	if (!s)
		return (ft_strdup(""));
	dupl = (char *)malloc((n + 1) * sizeof(char));
	if (!dupl)
		return (NULL);
	i = 0;
	while (*s && i < n)
		dupl[i++] = *s++;
	dupl[i] = '\0';
	return (dupl);
}
