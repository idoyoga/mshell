/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 14:14:50 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/27 14:14:51 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_cmd_args(t_cmd *cmd)
{
	size_t	count;

	count = 0;
	while (cmd->args[count] != NULL)
	{
		count++;
	}
	return (count);
}
