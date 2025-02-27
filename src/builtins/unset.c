/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xgossing <xgossing@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:56:08 by xgossing          #+#    #+#             */
/*   Updated: 2025/02/26 21:08:19 by xgossing         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(t_shell *shell, t_cmd *cmd)
{
	(void)shell;
	(void)cmd;
	printf("running unset\n");
	// unlink_env_node for all passed parameters
	// ensure that `unset KEY=VALUE` or other silly stuff like that
	// behaves predictably
	// bash seems to interpret params as KEYS ONLY
	// meaning `unset KEY=foo` would actually look for
	// an env value whose key is 'KEY=foo' which shouldn't exist
	// because export considers anything after the first '=' the value
	// so a naive approach may well be the way to go
}
