/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 11:20:52 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/20 20:00:31 by dplotzl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# define INV_ARGS "minishell: Too many arguments (max 1)"
# define NO_SHELL "minishell: Shell initialization failed"
# define INIT_FAIL "minishell: Initialization failed"
# define NO_MEM "minishell: Memory allocation failed"
# define NO_ENV "minishell: Failed to initialize environment"
# define GETCWD "minishell: Failed to get current working directory"
# define NO_WD "minishell: Failed to get working directory"
# define NO_PROMPT "minishell: Failed to create prompt"
# define NO_USER "minishell: Failed to get user"
# define NO_HOME "minishell: Failed to get home directory"
# define NO_ALLOC "minishell: Failed to add allocation"
# define QUOTES "minishell: Quotes not closed"
# define NO_RESIZE "minishell: Failed to resize allocation tracker"
# define NO_TRACK "minishell: Failed to initialize allocation tracker"

#endif
