/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:14:39 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/12 20:43:17 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const t_bltin	g_builtins[] = {
{ECHO, "echo", &builtin_echo},
{CD, "cd", &builtin_cd},
{PWD, "pwd", &builtin_pwd},
{EXPORT, "export", &builtin_export},
{UNSET, "unset", &builtin_unset},
{ENV, "env", &builtin_env},
{EXIT, "exit", &builtin_exit}
};
static const size_t		g_builtin_count = sizeof(g_builtins) / sizeof(t_bltin);

t_b_typ	identify_builtin(char *str)
{
	size_t	i;
	char	*builtin_name;

	i = 0;
	while (i < g_builtin_count)
	{
		builtin_name = g_builtins[i].name;
		if (ft_strncmp(str, builtin_name, ft_strlen(builtin_name) + 1) == 0)
			return (g_builtins[i].type);
		i++;
	}
	return (_NOT_A_BUILTIN);
}

void	(*get_builtin(t_b_typ type))(t_shell *sh)
{
	size_t	i;

	i = 0;
	while (i < g_builtin_count)
	{
		if (g_builtins[i].type == type)
			return (g_builtins[i].fn);
		i++;
	}
	return (NULL);
}
