/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:12:06 by myokono           #+#    #+#             */
/*   Updated: 2025/04/21 13:23:08 by ryabuki          ###   ########.fr       */
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
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env_list = init_env_list(envp);
	shell->env_array = env_list_to_array(shell->env_list);
	if (!shell->env_list || !shell->env_array)
	{
		free_shell(shell);
		return (NULL);
	}
	shell->exit_status = 0;
	shell->running = 1;
	return (shell);
}

static int	shell_loop(t_shell *shell)
{
	char	*input;

	while (shell->running)
	{
		setup_signals();
		input = readline("minishell$ ");
		if (g_signal_status != 0)
		{
			shell->exit_status = g_signal_status + 128;
			g_signal_status = 0;
		}
		if (input == NULL)
			return (printf("exit\n"), shell->exit_status);
		process_input(input, shell);
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_commands(shell->commands);
		shell->commands = NULL;
		free(input);
	}
	return (shell->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		status;

	setup_signals();
	shell = init_shell(envp);
	if (!shell)
		return (ERROR);
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
		status = process_input(ft_strdup(argv[2]), shell);
	else
		status = shell_loop(shell);
	free_shell(shell);
	rl_clear_history();
	return (status);
}
