/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:46:41 by dplotzl           #+#    #+#             */
/*   Updated: 2025/03/04 19:43:01 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Wrapper for strdup, automatically adding the allocation to the tracker
*/

char	*safe_strdup(t_shell *shell, const char *src)
{
	char	*dup;
	size_t	len;

	if (!shell || !shell->alloc_tracker.initialized || !src)
		return (NULL);
	len = ft_strlen(src);
	dup = safe_malloc(shell, len + 1);
	ft_memcpy(dup, src, len);
	dup[len] = '\0';
	return (dup);
}

/*
**	Wrapper for strndup, automatically adding the allocation to the tracker
*/

char	*safe_strndup(t_shell *shell, const char *src, size_t n)
{
	char	*dup;
	size_t	len;

	if (!shell || !shell->alloc_tracker.initialized || !src)
		return (NULL);
	len = ft_strlen(src);
	if (len < n)
		n = len;
	dup = safe_malloc(shell, n + 1);
	ft_memcpy(dup, src, n);
	dup[n] = '\0';
	return (dup);
}

/*
**	Wrapper for strjoin, automatically adding the allocation to the tracker
*/

char	*safe_strjoin(t_shell *shell, const char *s1, const char *s2)
{
	char	*new;
	size_t	len_s1;
	size_t	len_s2;

	if (!shell || !shell->alloc_tracker.initialized || !s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	new = safe_malloc(shell, len_s1 + len_s2 + 1);
	ft_memcpy(new, s1, len_s1);
	ft_memcpy(new + len_s1, s2, len_s2);
	new[len_s1 + len_s2] = '\0';
	return (new);
}

/*
**	Wrapper for malloc, automatically adding the allocation to the tracker
*/

void	*safe_malloc(t_shell *shell, size_t size)
{
	void	*ptr;

	if (!shell || !shell->alloc_tracker.initialized || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		error_exit(shell, NO_MEM, "safe_malloc", EXIT_FAILURE);
	if (!alloc_tracker_add(&shell->alloc_tracker, ptr, 0))
	{
		free(ptr);
		error_exit(shell, NO_MEM, "safe_malloc", EXIT_FAILURE);
	}
	return (ptr);
}

/*
**	Wrapper for calloc, automatically adding the allocation to the tracker
*/

void	*safe_calloc(t_shell *shell, size_t count, size_t size)
{
	void	*ptr;

	if (!shell || !shell->alloc_tracker.initialized || count == 0 || size == 0)
		return (NULL);
	ptr = ft_calloc(count, size);
	if (!ptr)
		error_exit(shell, NO_MEM, "safe_calloc", EXIT_FAILURE);
	if (!alloc_tracker_add(&shell->alloc_tracker, ptr, 0))
	{
		free(ptr);
		error_exit(shell, NO_MEM, "safe_calloc", EXIT_FAILURE);
	}
	return (ptr);
}
