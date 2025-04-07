/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:20:33 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 19:28:03 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		builtin_cd(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

int		builtin_echo(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

int		builtin_env(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

int		builtin_exit(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

int		builtin_export(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

int		builtin_pwd(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

int		builtin_unset(char	**argv, t_shell *shell)
{
	(void)argv;
	(void)shell;

	return (EXIT_SUCCESS);
}

// static bool	is_valid_numeric(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (str[i] == '+' || str[i] == '-')
// 		i++;
// 	if (str[i] == '\0')
// 		return (false);
// 	while (str[i] != '\0')
// 	{
// 		if (ft_isdigit(str[i]) == false)
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

// static void	parse_sign_and_index(const char *str, int *sign, int *i)
// {
// 	*sign = 1;
// 	*i = 0;
// 	while (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n' \
// 		|| str[*i] == '\v' || str[*i] == '\f' || str[*i] == '\r')
// 		(*i)++;
// 	if (str[*i] == '-')
// 	{
// 		*sign = -1;
// 		(*i)++;
// 	}
// 	else if (str[*i] == '+')
// 		(*i)++;
// }

// static long long	ft_atoll(const char *str)
// {
// 	long long	result;
// 	int			sign;
// 	int			i;

// 	result = 0;
// 	parse_sign_and_index(str, &sign, &i);
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		if (result > LLONG_MAX / 10
// 			|| (result == LLONG_MAX / 10 && (str[i] - '0') > LLONG_MAX % 10))
// 		{
// 			if (sign == 1)
// 				return (LLONG_MAX);
// 			else
// 				return (LLONG_MIN);
// 		}
// 		result = result * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	return (result * sign);
// }

// int	builtin_exit(t_command *cmd, t_shell *shell)
// {
// 	ft_putstr_fd("exit\n", STDERR_FILENO);
// 	if (cmd->args[1] == NULL)
// 	{
// 		shell->running = 0;
// 		return (shell->exit_status);
// 	}
// 	if (!is_valid_numeric(cmd->args[1]))
// 	{
// 		command_error("exit", ft_strjoin(cmd->args[1], \
// 			": numeric argument required"));
// 		shell->running = 0;
// 		shell->exit_status = 255;
// 		return (255);
// 	}
// 	if (cmd->args[2] != NULL)
// 	{
// 		error_message("exit: too many arguments");
// 		return (1);
// 	}
// 	shell->running = 0;
// 	shell->exit_status = (unsigned char)ft_atoll(cmd->args[1]);
// 	return (shell->exit_status);
// }
