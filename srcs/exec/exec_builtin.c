/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:06:48 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:27:16 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_node *node, t_shell *shell)
{
	int		status;
	char	**argv;

	do_redirect(node->command->redirects, shell);
	argv = token_list_to_argv(node->args, shell);
	if (ft_strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv, shell);
	else if (ft_strcmp(argv[0], "export") == 0)
		status = builtin_export(argv, shell);
	else if (ft_strcmp(argv[0], "unset") == 0)
		status = builtin_unset(argv, shell);
	else if (ft_strcmp(argv[0], "env") == 0)
		status = builtin_env(argv, shell);
	else if (ft_strcmp(argv[0], "cd") == 0)
		status = builtin_cd(argv, shell);
	else if (ft_strcmp(argv[0], "echo") == 0)
		status = builtin_echo(argv, shell);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		status = builtin_pwd(argv, shell);
	else
		assert_error("exec_builtin", shell);
	free_argv(argv);
	reset_redirect(node->command->redirects, shell);
	return (status);
}
