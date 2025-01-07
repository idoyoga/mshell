/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:02:34 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/06 15:54:59 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dupl;
	size_t	i;

	if (!s)
		return (NULL);
	dupl = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!dupl)
		return (NULL);
	i = 0;
	while (*s)
		dupl[i++] = *s++;
	dupl[i] = '\0';
	return (dupl);
}
