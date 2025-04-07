/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:12:06 by myokono           #+#    #+#             */
/*   Updated: 2025/04/07 18:58:24 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status = 0;

static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->node = NULL;
	shell->token = NULL;
	shell->env_list = init_env_list(envp);
	shell->env_array = env_list_to_array(shell->env_list);
	if (!shell->env_list || !shell->env_array)
	{
		free_shell(shell);
		return (NULL);
	}
	shell->exit_status = 0;
	shell->running = 1;
	shell->flag_error = false;
	return (shell);
}

static char	*get_last_argument(t_node *node)
{
	t_token		*args;

	if (!node)
		return (NULL);
	while (node && node->next)
		node = node->next;
	if (!node->args)
		return (NULL);
	args = node->args;
	while (args && args->next)
		args = args->next;
	return (ft_strdup(args->value));
}

static int	process_line(char *input, t_shell *shell)
{
	char	*last_arg;

	tokenize(input, shell);
	if (is_eof(shell->token) == true)
		;
	else if (shell->flag_error == true)
		shell->exit_status = ERROR_TOKENIZE;
	else
	{
		parse(shell);
		if (shell->flag_error == true)
			shell->exit_status = ERROR_PARSE;
		else
		{
			expand(shell);
			exec(shell->node, shell);
		}
		last_arg = get_last_argument(shell->node);
		if (last_arg)
		{
			add_env_node(&shell->env_list, "_", last_arg);
			update_env_array(shell);
			free(last_arg);
		}
		free_node(shell->node);
	}
	free_token(shell->token);
	return (shell->exit_status);
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	line = NULL;
	while (shell->running)
	{
		if (g_signal_status != 0)
		{
			shell->exit_status = 128 + g_signal_status;
			g_signal_status = 0;
			continue ;
		}
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (*line)
			add_history(line);
		process_line(line, shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		status;

	setup_signals();
	shell = init_shell(envp);
	if (!shell)
		return (EXIT_FAILURE);
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
		process_line(ft_strdup(argv[2]), shell);
	else
		shell_loop(shell);
	status = shell->exit_status;
	free_shell(shell);
	rl_clear_history();
	return (status);
}
