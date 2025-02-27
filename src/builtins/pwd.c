/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:55:44 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/26 20:32:22 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_pwd(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	(void)cmd;
	path = getcwd(NULL, 0);
	if (!path)
		error_exit(shell, PWD_NO_CWD, "getcwd", EXIT_FAILURE);
	printf("%s\n", path);
	free(path);
}
