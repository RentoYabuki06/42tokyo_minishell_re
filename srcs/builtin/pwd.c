/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:31:02 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 19:01:41 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	builtin_pwd(t_command *cmd, t_shell *shell)
// {
// 	char	current_dir[PATH_MAX];

// 	(void)shell;
// 	if (getcwd(current_dir, PATH_MAX) == NULL)
// 	{
// 		system_error("pwd");
// 		return (1);
// 	}
// 	ft_putstr_fd(current_dir, cmd->output_fd);
// 	ft_putstr_fd("\n", cmd->output_fd);
// 	return (0);
// }
