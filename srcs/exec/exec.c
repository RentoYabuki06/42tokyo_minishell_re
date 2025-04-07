/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:50:52 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:29:50 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void	validate_access(char *path, char *filename, t_shell *shell)
{
	struct stat	st;
	
	if (path == NULL)
		error_exit(filename, "command not found", 127, shell);
	if (ft_strcmp(filename, "") == 0)
		error_exit(filename, "command not found", 127, shell);
	if (ft_strcmp(filename, "..") == 0)
		error_exit(filename, "command not found", 127, shell);
	if (access(path, F_OK) < 0)
		error_exit(filename, "command not found", 127, shell);
	if (stat(path, &st) < 0)
		fatal_error("fstat", shell);
	if (S_ISDIR(st.st_mode))
		error_exit(filename, "is a directory", 126, shell);
	if (access(path, X_OK) < 0)
		error_exit(path, "Permission denied", 126, shell);
}

static int	exec_nonbuiltin(t_node *node, t_shell *shell)
{
	char		*path;
	char		**argv;
	
	do_redirect(node->command->redirects, shell);
	argv = token_list_to_argv(node->command->args, shell);
	path = argv[0];
	if (ft_strchr(path, '/') == NULL)
		path = search_path(path, shell);
	validate_access(path, argv[0], shell);
	execve(path, argv, shell->env_array);
	free_argv(argv);
	reset_redirect(node->command->redirects, shell);
	fatal_error("execve", shell);
}

pid_t	exec_pipeline(t_node *node, t_shell *shell)
{
	pid_t		pid;
	
	if (node == NULL)
		return (-1);
	prepare_pipe(node, shell);
	pid = fork();
	ignore_signals();
	if (pid < 0)
		fatal_error("fork", shell);
	else if (pid == 0)
	{
		prepare_pipe_child(node, shell);
		defalut_signals();
		setup_child_signals();
		if (is_builtin(node))
			exit(exec_builtin(node, shell));
		else
			exec_nonbuiltin(node, shell);
	}
	prepare_pipe_parent(node, shell);
	if (node->next)
	return (exec_pipeline(node->next, shell));
	return (pid);
}

static int	wait_pipeline(pid_t last_pid, t_shell *shell)
{
	pid_t	wait_result;
	int		status;
	int		wstatus;
	
	while (true)
	{
		wait_result = wait(&wstatus);
		if (wait_result == last_pid)
		{
			if (WIFSIGNALED(wstatus))
				status = 128 + WTERMSIG(wstatus);
			else
				status = WEXITSTATUS(wstatus);
		}
		else if (wait_result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				fatal_error("wait", shell);
		}
	}
	return (status);
}

void	exec(t_node *node, t_shell *shell)
{
	pid_t	last_pid;

	if (open_redir_file(node, shell) == EXIT_FAILURE)
		shell->exit_status = ERROR_OPEN_REDIR;
	else if (node->next == NULL && is_builtin(node))
		shell->exit_status = exec_builtin(node, shell);
	else
	{
		last_pid = exec_pipeline(node, shell);
		shell->exit_status = wait_pipeline(last_pid, shell);
	}
}