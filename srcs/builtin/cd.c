/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:25:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 19:01:21 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	update_pwd_env(t_shell *shell, char *old_pwd)
// {
// 	char	current_pwd[PATH_MAX];

// 	if (getcwd(current_pwd, PATH_MAX) == NULL)
// 	{
// 		system_error("getcwd");
// 		return ;
// 	}
// 	add_env_node(&shell->env_list, "PWD", current_pwd);
// 	if (old_pwd)
// 		add_env_node(&shell->env_list, "OLDPWD", old_pwd);
// 	update_env_array(shell);
// 	if (shell->env_array == NULL)
// 	{
// 		free_shell(shell);
// 		system_error("update_env_array");
// 		return ;
// 	}
// }

// int	builtin_cd(t_command *cmd, t_shell *shell)
// {
// 	char	*path;
// 	char	old_pwd[PATH_MAX];

// 	if (getcwd(old_pwd, PATH_MAX) == NULL)
// 	{
// 		system_error("getcwd");
// 		return (1);
// 	}
// 	if (!cmd->args[1])
// 	{
// 		path = get_env_value(shell->env_list, "HOME");
// 		if (!path)
// 			return (error_message("cd: HOME not set"), 1);
// 	}
// 	else
// 		path = cmd->args[1];
// 	if (chdir(path) != 0)
// 	{
// 		system_error(path);
// 		return (1);
// 	}
// 	update_pwd_env(shell, old_pwd);
// 	return (0);
// }
