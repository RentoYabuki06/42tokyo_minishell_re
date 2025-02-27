/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:54:49 by myokono           #+#    #+#             */
/*   Updated: 2025/02/27 12:55:02 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

/**
 * 入力リダイレクトを設定する関数
 * @param filename 入力ファイル名
 * @param cmd コマンド構造体
 * @return 成功すれば1、失敗すれば0
 */
static int	setup_input_redirect(char *filename, t_command *cmd)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		system_error(filename);
		return (0);
	}

	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = fd;
	return (1);
}

/**
 * 出力リダイレクトを設定する関数
 * @param filename 出力ファイル名
 * @param append 追記モードか否か
 * @param cmd コマンド構造体
 * @return 成功すれば1、失敗すれば0
 */
static int	setup_output_redirect(char *filename, int append, t_command *cmd)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;

	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		system_error(filename);
		return (0);
	}

	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	
	cmd->output_fd = fd;
	return (1);
}

/**
 * ヒアドキュメントを設定する関数
 * @param delimiter ヒアドキュメントの区切り文字
 * @param cmd コマンド構造体
 * @return 成功すれば1、失敗すれば0
 */
static int	setup_heredoc(char *delimiter, t_command *cmd)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		system_error("pipe");
		return (0);
	}

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}

		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}

	close(pipe_fd[1]);

	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	
	cmd->input_fd = pipe_fd[0];
	return (1);
}

/**
 * コマンドのリダイレクトを設定する関数
 * @param cmd コマンド構造体
 * @return 成功すればSUCCESS、失敗すればERROR
 */
int	setup_redirects(t_command *cmd)
{
	t_token	*token;
	t_token	*next;

	token = cmd->redirects;
	while (token)
	{
		next = token->next;
		if (token->type == TOKEN_REDIRECT_IN)
		{
			if (!setup_input_redirect(next->value, cmd))
				return (ERROR);
		}
		else if (token->type == TOKEN_REDIRECT_OUT)
		{
			if (!setup_output_redirect(next->value, 0, cmd))
				return (ERROR);
		}
		else if (token->type == TOKEN_APPEND)
		{
			if (!setup_output_redirect(next->value, 1, cmd))
				return (ERROR);
		}
		else if (token->type == TOKEN_HEREDOC)
		{
			if (!setup_heredoc(next->value, cmd))
				return (ERROR);
		}
		if (next)
			token = next->next;
		else
			token = NULL;
	}

	return (SUCCESS);
}
