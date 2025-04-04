/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/04 18:24:48 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 新しいコマンド構造体を作成する関数
 * @return 作成されたコマンド構造体
 */
t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = safe_malloc(sizeof(t_command));
	cmd->args = NULL;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

/**
 * コマンドをリストに追加する関数
 * @param commands コマンドリストへのポインタ
 * @param new_command 追加する新しいコマンド
 */
void	add_command(t_command **commands, t_command *new_command)
{
	t_command	*current;

	if (!*commands)
	{
		*commands = new_command;
		return ;
	}
	
	current = *commands;
	while (current->next)
		current = current->next;
	
	current->next = new_command;
}

/**
 * 引数を追加する関数
 * @param cmd コマンド構造体
 * @param arg 追加する引数
 */
static void	add_arg(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;
	int		size;

	/* 現在の引数配列のサイズを計算 */
	size = 0;
	if (cmd->args)
	{
		while (cmd->args[size])
			size++;
	}
	
	/* 新しい引数配列を作成 */
	new_args = safe_malloc(sizeof(char *) * (size + 2));
	
	/* 既存の引数をコピー */
	i = 0;
	while (i < size)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	
	/* 新しい引数を追加 */
	new_args[size] = ft_strdup(arg);
	new_args[size + 1] = NULL;
	
	/* 古い配列を解放（内容はコピーしたので保持） */
	if (cmd->args)
		free(cmd->args);
	
	cmd->args = new_args;
}
/**
 * リダイレクトを処理する関数（実際のファイル操作は行わず情報のみ記録）
 * @param tokens 現在のトークンへのポインタ
 * @param cmd 処理中のコマンド
 * @return 処理に成功すれば1、失敗すれば0
 */
static int	handle_redirect(t_token **tokens, t_command *cmd)
{
	t_token_type	type;
	char			*filename;
	t_token			*redirect_token;
	t_token			*filename_token;

	/* リダイレクトトークンを記録 */
	type = (*tokens)->type;
	redirect_token = create_token(type, ft_strdup((*tokens)->value));
	*tokens = (*tokens)->next;
	/* リダイレクト対象のファイル名（または区切り文字）が必要 */
	if (!*tokens || (*tokens)->type != TOKEN_WORD)
	{
		free_tokens(redirect_token);
		error_message("Syntax error near unexpected token");
		return (0);
	}
	filename = (*tokens)->value;
	filename_token = create_token(TOKEN_WORD, ft_strdup(filename));
	*tokens = (*tokens)->next;
	/* リダイレクト情報を連結（ペアになっている） */
	redirect_token->next = filename_token;
	/* コマンドのリダイレクトリストに追加 */
	if (!cmd->redirects)
		cmd->redirects = redirect_token;
	else
	{
		t_token *last = cmd->redirects;
		while (last->next)
			last = last->next;
		last->next = redirect_token;
	}
	return (1);
}



/**
 * パイプでつながれたコマンドを処理する関数
 * @param tokens 現在のトークンへのポインタ
 * @param cmd 処理中のコマンド
 * @param commands コマンドリストへのポインタ
 * @return 処理に成功すれば1、失敗すれば0
 */
static int	handle_pipe(t_token **tokens, t_command *cmd, t_command **commands)
{
	/* 現在のコマンドに引数が1つもなければエラー */
	if (!cmd->args)
	{
		error_message("Syntax error near unexpected token `|'");
		return (0);
	}
	
	/* 現在のコマンドをリストに追加 */
	add_command(commands, cmd);
	
	/* パイプトークンをスキップ */
	*tokens = (*tokens)->next;
	
	return (1);
}

/**
 * トークンをコマンドに変換する関数
 * @param shell シェル構造体
 * @return 処理に成功すればSUCCESS、失敗すればERROR
 */
int	parse(t_shell *shell)
{
	t_token		*current_token;
	t_command	*current_cmd;

	if (!shell->tokens)
		return (SUCCESS);

	shell->commands = NULL;
	current_token = shell->tokens;
	current_cmd = create_command();

	while (current_token)
	{
		if (current_token->type == TOKEN_WORD)
		{
			add_arg(current_cmd, current_token->value);
			current_token = current_token->next;
		}
		else if (current_token->type == TOKEN_REDIRECT_IN
			|| current_token->type == TOKEN_REDIRECT_OUT
			|| current_token->type == TOKEN_HEREDOC
			|| current_token->type == TOKEN_APPEND)
		{
			if (!handle_redirect(&current_token, current_cmd))
			{
				free_commands(current_cmd);
				return (ERROR);
			}
		}
		else if (current_token->type == TOKEN_PIPE)
		{
			if (!handle_pipe(&current_token, current_cmd, &shell->commands))
			{
				free_commands(current_cmd);
				free_commands(shell->commands);
				shell->commands = NULL;
				return (ERROR);
			}
			current_cmd = create_command();
		}
		else
		{
			/* 未知のトークンタイプ */
			error_message("Parser error: unknown token type");
			free_commands(current_cmd);
			free_commands(shell->commands);
			shell->commands = NULL;
			return (ERROR);
		}
	}

	/* 最後のコマンドをリストに追加 */
	if (current_cmd->args || current_cmd->input_fd != STDIN_FILENO
		|| current_cmd->output_fd != STDOUT_FILENO)
	{
		add_command(&shell->commands, current_cmd);
	}
	else
	{
		free_commands(current_cmd);
	}

	return (SUCCESS);
}