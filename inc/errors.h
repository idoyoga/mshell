/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dplotzl <dplotzl@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 11:20:52 by dplotzl           #+#    #+#             */
/*   Updated: 2025/01/23 14:35:38 by dplotzl          ###   ########.fr       */
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
# define OPEN_PIPE "minishell: Pipe not closed"
# define START_PIPE "minishell: Input cannot start with pipe"
# define END_PIPE "minishell: Input cannot end with pipe"
# define CONSEC_PIPES "minishell: Consecutive pipes not allowed"
# define CONSEC_REDIR "minishell: Consecutive redirections not allowed"
# define UNEXP_TOKEN "minishell: Syntax error near unexpected token"
# define NEWL "minishell: Newline not allowed"
# define NO_REPLACE "minishell: Failed to replace environment variables"
# define NO_CMD "minishell: No command to execute"
# define BACKSLASH "minishell: Backslash is not interpreted"
# define SEMICOLON "minishell: Semicolon is not interpreted"
# define FILE_ERR "minishell: Failed to open file"
# define PIPE_ERR "minishell: Failed to create pipe"
# define SYNT_ERR "minishell: Syntax error"

#endif
