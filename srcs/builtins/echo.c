/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/03/09 22:30:18 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1); // "-" だけの場合はフラグではない
}

int	builtin_echo(t_command *cmd, t_shell *shell)
{
	int	i;
	int	n_flag;

	(void)shell;
	if (!cmd->args[1])
	{
		ft_putstr_fd("\n", cmd->output_fd);
		return (0);
	}
	i = 1;
	n_flag = 0;
	while (cmd->args[i] && is_n_flag(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], cmd->output_fd);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", cmd->output_fd);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", cmd->output_fd);
	return (0);
}
