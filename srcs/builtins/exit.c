/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:20:33 by myokono           #+#    #+#             */
/*   Updated: 2025/04/16 17:47:06 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_numeric(char *str)
{
	int			i;
	bool		sign;

	i = 0;
	sign = false;
	if (str[i] == '+' || str[i] == '-')
	{
		sign = true;
		i++;
	}
	if (str[i] == '\0')
		return (false);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == false)
			return (false);
		i++;
	}
	if (sign == true)
		i--;
	if (i > 19)
		return (false);
	return (true);
}

static void	parse_sign_and_index(const char *str, long long *sign, int *i)
{
	*sign = 1;
	*i = 0;
	while (str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n'
		|| str[*i] == '\v' || str[*i] == '\f' || str[*i] == '\r')
		(*i)++;
	if (str[*i] == '-')
	{
		*sign = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
		(*i)++;
}

static long long	ft_atoll(const char *str, bool *flag)
{
	long long	result;
	long long	sign;
	int			i;

	result = 0;
	parse_sign_and_index(str, &sign, &i);
	if (ft_strcmp(&str[i], "9223372036854775808") == 0 && sign == -1)
		return (LLONG_MIN);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > LLONG_MAX / 10
			|| (result == LLONG_MAX / 10 && (str[i] - '0') > LLONG_MAX % 10))
			*flag = true;
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	numeric_error(t_command *cmd, t_shell *shell)
{
	char	*error_message;

	error_message = ft_strjoin(cmd->args[1], ": numeric argument required");
	print_error(cmd->args[0], error_message);
	free(error_message);
	shell->running = 0;
	shell->exit_status = 2;
	return (2);
}

int	builtin_exit(t_command *cmd, t_shell *shell)
{
	long long	exit;
	bool		flag;

	if (shell->commands->next == NULL)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (cmd->args[1] == NULL)
	{
		shell->running = 0;
		return (shell->exit_status);
	}
	if (cmd->args[2] != NULL)
		return (error_message("exit: too many arguments"), ERROR);
	if (!is_valid_numeric(cmd->args[1]))
		return (numeric_error(cmd, shell));
	flag = false;
	exit = ft_atoll(cmd->args[1], &flag);
	if (flag == true)
		return (numeric_error(cmd, shell));
	shell->running = 0;
	shell->exit_status = (unsigned char)exit;
	return (shell->exit_status);
}
