/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:25:47 by myokono           #+#    #+#             */
/*   Updated: 2025/04/18 10:56:49 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*path_join(const char *base, const char *rel)
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
		ft_fprintf1(STDOUT_FILENO, "%s\n", *target);
	}
	else
		*target = cmd->args[1];
	return (SUCCESS);
}

static int	new_logipath(t_shell *shell, char *tgt, char **rawpath, char **old)
{
	char	*pwd_env;

	pwd_env = get_env_value(shell->env_list, "PWD");
	if (!pwd_env || tgt[0] == '/')
		*rawpath = ft_strdup(tgt);
	else
		*rawpath = path_join(pwd_env, tgt);
	if (!*rawpath)
		return (system_error("path_join"), free(old), ERROR);
	return (SUCCESS);
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*target;
	char	*old_pwd;
	char	*rawpath;
	char	*simplified;

	if (cd_check_arg(cmd, shell, &target) == ERROR)
		return (ERROR);
	if (chdir(target) != 0)
		return (system_error(target), ERROR);
	old_pwd = getcwd(NULL, 0);
	if (new_logipath(shell, target, &rawpath, &old_pwd) == ERROR)
		return (ERROR);
	simplified = simplify_path(rawpath);
	free(rawpath);
	if (!simplified)
		return (system_error("simplify_path"), free(old_pwd), ERROR);
	add_env_node(&shell->env_list, "PWD", simplified);
	if (old_pwd)
	{
		add_env_node(&shell->env_list, "OLDPWD", old_pwd);
		free(old_pwd);
	}
	update_env_array(shell);
	free(simplified);
	return (SUCCESS);
}
