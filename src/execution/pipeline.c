/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:54:23 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/12 21:58:12 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_with_pipeline(t_shell *shell, size_t cmd_count)
{
	size_t	i;

	printf("executing with pipeline\n");
	(void)shell;
	i = 0;
	while (i < cmd_count)
	{
		i++;
	}
}
