/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:54:49 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 18:21:31 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

/**
 * リダイレクトの種類に応じたファイルディスクリプタの設定を行う共通ヘルパー関数
 * @param cmd 対象のコマンド構造体
 * @param type リダイレクトの種類
 * @param filename 対象のファイル名または区切り文字
 * @return 成功すれば1、失敗すれば0
 */
static int	setup_redirect_helper(t_command *cmd, t_token_type type, char *filename)
{
	int	fd;
	int	flags;

	if (type == TOKEN_REDIRECT_IN)
	{
		fd = open(filename, O_RDONLY);
		if (fd == -1)
		{
			system_error(filename);
			return (0);
		}
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = fd;
	}
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_APPEND)
	{
		flags = O_WRONLY | O_CREAT;
		if (type == TOKEN_REDIRECT_OUT)
			flags |= O_TRUNC;
		else // TOKEN_APPEND
			flags |= O_APPEND;
		fd = open(filename, flags, 0644);
		if (fd == -1)
		{
			system_error(filename);
			return (0);
		}
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		cmd->output_fd = fd;
	}
	else if (type == TOKEN_HEREDOC)
	{
		int pipe_fd[2];
		char *line;
		char *delimiter = ft_strdup(filename);
		
		if (pipe(pipe_fd) == -1)
		{
			free(delimiter);
			system_error("pipe");
			return (0);
		}
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			ft_putstr_fd(line, pipe_fd[1]);
			ft_putstr_fd("\n", pipe_fd[1]);
			free(line);
		}
		free(delimiter);
		close(pipe_fd[1]);
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		cmd->input_fd = pipe_fd[0];
	}
	return (1);
}

int	setup_redirects(t_command *cmd)
{
	t_token	*token;
	t_token	*next;

	token = cmd->redirects;
	while (token)
	{
		next = token->next;
		if (!next)
		{
			error_message("Missing filename for redirection");
			return (ERROR);
		}
		if (!setup_redirect_helper(cmd, token->type, next->value))
			return (ERROR);
		token = next->next;
	}
	return (SUCCESS);
}
