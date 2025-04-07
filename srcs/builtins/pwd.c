/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:31:02 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 17:04:36 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(t_command *cmd, t_shell *shell)
{
	char	current_dir[PATH_MAX];

	(void)cmd;
	(void)shell;
	if (getcwd(current_dir, PATH_MAX) == NULL)
	{
		system_error("pwd");
		return (ERROR);
	}
	ft_putstr_fd(current_dir, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}
