/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:06:57 by myokono           #+#    #+#             */
/*   Updated: 2025/04/17 19:02:07 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redirect_if_needed(int src_fd, int dest_fd)
{
	if (src_fd != dest_fd)
	{
		if (dup2(src_fd, dest_fd) == -1)
			system_error("dup2");
		close(src_fd);
	}
}

static int	execute_builtin_forked(t_command *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (system_error("fork"), ERROR);
	if (pid == 0)
	{
		redirect_if_needed(cmd->input_fd, STDIN_FILENO);
		redirect_if_needed(cmd->output_fd, STDOUT_FILENO);
		status = execute_builtin(cmd, shell);
		exit(status);
	}
	if (waitpid(pid, &status, 0) == -1)
		return (system_error("waitpid"), ERROR);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (ERROR);
}

static int	execute_command(t_command *cmd, t_shell *shell)
{
	int	status;

	if (setup_redirects(cmd) == ERROR)
		return (ERROR);
	if (!cmd->args || !cmd->args[0])
		return (SUCCESS);
	if (is_builtin(cmd->args[0]))
	{
		if (cmd->input_fd != STDIN_FILENO || cmd->output_fd != STDOUT_FILENO)
			return (execute_builtin_forked(cmd, shell));
		return (execute_builtin(cmd, shell));
	}
	status = execute_external_standalone(cmd, shell);
	return (status);
}



static void free_token(t_token *tok)
{
    if (!tok)
        return;
    if (tok->value)
        free(tok->value);
    free(tok);
}


/**
 * 親プロセスでヒアドキュメント（<<）だけを処理し、
 * TOKEN_HEREDOC トークンをリストから取り除き、
 * 得られた FD を cmd->input_fd にセットする。
 */
static int process_heredocs_in_parent(t_shell *shell)
{
    t_command *cmd = shell->commands;

    while (cmd)
    {
        t_token **p = &cmd->redirects;
        while (*p)
        {
            t_token *tok = *p;
            if (tok->type == TOKEN_HEREDOC)
            {
                t_token *next = tok->next;
                if (!next)
                    return (ERROR);
                /* ヒアドキュメントを展開 */
                int fd = setup_redir_return_fd(TOKEN_HEREDOC, next->value);
                if (fd < 0)
                    return (ERROR);
                /* 既存の input_fd があれば閉じる */
                if (cmd->input_fd != STDIN_FILENO)
                    close(cmd->input_fd);
                cmd->input_fd = fd;
                /* tok と next をリストから削除 */
                *p = next->next;
                free_token(next);
                free_token(tok);
                continue;  /* p は同じ場所を指したまま */
            }
            p = &tok->next;
        }
        cmd = cmd->next;
    }
    return (SUCCESS);
}

int execute_commands(t_shell *shell)
{
    if (!shell->commands)
        return (SUCCESS);

    /* パイプラインがある場合 */
    if (shell->commands->next)
    {
        /* ① 親でヒアドキュメントだけ処理 */
        if (process_heredocs_in_parent(shell) == ERROR)
            return (ERROR);

        /* ② パイプを張る */
        if (setup_pipes(shell->commands) != SUCCESS)
            return (ERROR);

        /* ③ fork & exec_pipeline */
        return (execute_pipeline(shell->commands, shell));
    }

    /* 単一コマンドの場合もヒアドキュメントは親で先に */
    if (process_heredocs_in_parent(shell) == ERROR)
        return (ERROR);

    /* リダイレクト／ビルトイン or 外部実行 */
    return (execute_command(shell->commands, shell));
}
