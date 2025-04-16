/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:25:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 21:00:21 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *path_join(const char *base, const char *rel)
{
	char	*joined;
	size_t	len;
	
	len = ft_strlen(base) + ft_strlen(rel) + 2;
	joined = malloc(len);
	if (!joined)
		return (NULL);
	joined[0] = '\0';
	ft_strlcpy(joined, base, len);
	if (base[ft_strlen(base) - 1] != '/')
		ft_strlcat(joined, "/", len);
	ft_strlcat(joined, rel, len);
	return (joined);
}

static int	cd_check_arg(t_command *cmd, t_shell *shell, char **target)
{
	if (cmd->args[1] && cmd->args[2])
		return (error_message("cd: too many arguments"), ERROR);
	if (!cmd->args[1])
	{
		*target = get_env_value(shell->env_list, "HOME");
		if (!*target)
			return (error_message("cd: HOME not set"), ERROR);
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		*target = get_env_value(shell->env_list, "OLDPWD");
		if (!*target)
			return (error_message("cd: OLDPWD not set"), ERROR);
		ft_fprintf1(STDOUT_FILENO, "%s\n", *target); // echo $OLDPWD
	}
	else
		*target = cmd->args[1];
	return (SUCCESS);
}

static int new_logical_path(t_shell *shell, char *target, char **raw_path, char **old_pwd)
{
	char *pwd_env;
	
	pwd_env = get_env_value(shell->env_list, "PWD");
	if (!pwd_env || target[0] == '/')
		*raw_path = ft_strdup(target);
	else
		*raw_path = path_join(pwd_env, target);
	if (!*raw_path)
		return (system_error("path_join"), free(old_pwd), ERROR);
	return (SUCCESS);
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*target;
	char	*old_pwd;
	char	*raw_path;
	char	*simplified;

	if (cd_check_arg(cmd, shell, &target) == ERROR)
		return (ERROR);
	if (chdir(target) != 0)
		return (system_error(target), ERROR);
	old_pwd = getcwd(NULL, 0);
	if (new_logical_path(shell, target, &raw_path, &old_pwd) == ERROR)
		return (ERROR);
	simplified = simplify_path(raw_path);
	if (!simplified)
		return (system_error("simplify_path"), free(raw_path), free(old_pwd), ERROR);
	add_env_node(&shell->env_list, "PWD", simplified);
	if (old_pwd)
	{
		add_env_node(&shell->env_list, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	update_env_array(shell);
	free(raw_path);
	free(simplified);
	return (SUCCESS);
}
