/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:25:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 19:19:27 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_pwd_env(t_shell *shell, char *old_pwd)
{
	char	current_pwd[PATH_MAX];
	char	*logical_pwd;
	char	*fallback_pwd;
	
	if (getcwd(current_pwd, PATH_MAX) == NULL)
	{
		fallback_pwd = get_env_value(shell->env_list, "PWD");
		if (!fallback_pwd)
			return (system_error("getcwd and no PWD fallback"), ERROR);
		logical_pwd = ft_strjoin(fallback_pwd, "/..");
		if (!logical_pwd)
			return (system_error("ft_strjoin"), ERROR);
		add_env_node(&shell->env_list, "PWD", logical_pwd);
		free(logical_pwd);
	}
	else
		add_env_node(&shell->env_list, "PWD", current_pwd);
	if (old_pwd)
		add_env_node(&shell->env_list, "OLDPWD", old_pwd);
	update_env_array(shell);
	if (shell->env_array == NULL)
		return (system_error("update_env_array"), ERROR);
	return (SUCCESS);
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	old_pwd[PATH_MAX];

	if (cmd->args[1] && cmd->args[2])
		return (error_message("cd: too many arguments"), ERROR);
	if (getcwd(old_pwd, PATH_MAX) == NULL)
		old_pwd[0] = '\0';
	if (!cmd->args[1])
	{
		path = get_env_value(shell->env_list, "HOME");
		if (!path)
			return (error_message("cd: HOME not set"), ERROR);
	}
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
		return (system_error(path), ERROR);
	if (update_pwd_env(shell, old_pwd[0] ? old_pwd : NULL) != SUCCESS)
		return (system_error("update_pwd_env"), ERROR);
	return (SUCCESS);
}
