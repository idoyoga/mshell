/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:55:39 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 17:13:45 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	libft_safe_malloc(char **word_arr, size_t pos)
{
	size_t	i;

	if (!word_arr)
		return ;
	i = 0;
	while (i < pos)
	{
		free(word_arr[i]);
		word_arr[i] = NULL;
		i++;
	}
	free(word_arr);
	word_arr = NULL;
}

char	**fill(char **word_arr, char const *s, char c)
{
	size_t	len;
	size_t	i;

	i = 0;
	while (*s)
	{
		len = 0;
		while (*s == c && *s)
			++s;
		while (*s != c && *s)
		{
			++len;
			++s;
		}
		if (len)
		{
			word_arr[i] = ft_substr(s - len, 0, len);
			if (!(word_arr[i]))
				return (libft_safe_malloc(word_arr, i), NULL);
			i++;
		}
	}
	return (word_arr);
}

size_t	count_words(char const *s, char c)
{
	size_t				words;
	size_t				i;
	unsigned char		uc;
	const unsigned char	*us;

	if (!ft_strlen(s))
		return (0);
	uc = (unsigned char)c;
	us = (const unsigned char *)s;
	i = 0;
	words = 0;
	while (us[i])
	{
		if (us[i] != uc && (us[i + 1] == uc || us[i + 1] == '\0'))
			words++;
		i++;
	}
	return (words);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**word_arr;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	words++;
	word_arr = (char **)malloc((words) * sizeof(char *));
	if (!word_arr)
		return (NULL);
	if (!fill(word_arr, s, c))
	{
		libft_safe_malloc(word_arr, words - 1);
		return (NULL);
	}
	word_arr[words - 1] = NULL;
	return (word_arr);
}

/*int	main()
{
	char	*s = "hello! hey hey hey hey";
	char	**ar;
	ar = ft_split(s, ' ');
	if (**ar)
	{
		for (int i = 0; ar[i] != NULL; i++)
		{
			printf("%s\n", ar[i]);
			free(ar[i]);
		}
		free(ar);
		return (0);
	}*
	else
		return (1);
}*/
