/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:31:02 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 19:39:58 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_dir(char *path)
{
	ft_fprintf1(STDOUT_FILENO, "%s\n", path);
	return (SUCCESS);
}

int	builtin_pwd(t_command *cmd, t_shell *shell)
{
	char	current_dir[PATH_MAX];
	char	*fallback_pwd;

	(void)cmd;
	(void)shell;
	fallback_pwd = get_env_value(shell->env_list, "PWD");
	if (fallback_pwd != NULL)
		return (print_dir(fallback_pwd));
	if (getcwd(current_dir, PATH_MAX) != NULL)
		return (print_dir(current_dir));
	system_error("pwd");
	return (ERROR);
}
