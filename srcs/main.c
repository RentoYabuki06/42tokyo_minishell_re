/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:12:06 by myokono           #+#    #+#             */
/*   Updated: 2025/04/04 19:31:55 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Global variable for signal handling */
int		g_signal_status = 0;

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env_list = init_env_list(envp);
	shell->env_array = env_list_to_array(shell->env_list);
	shell->exit_status = 0;
	shell->running = 1;
	return (shell);
}

void	free_shell(t_shell *shell)
{
	t_env	*tmp;
	t_env	*current;

	if (!shell)
		return ;
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->env_list)
	{
		current = shell->env_list;
		while (current)
		{
			tmp = current->next;
			free(current->key);
			free(current->value);
			free(current);
			current = tmp;
		}
	}
	if (shell->env_array)
		free_array(shell->env_array);
	free(shell);
}

int	process_input(char *input, t_shell *shell)
{
	if (!input || ft_strlen(input) == 0)
		return (SUCCESS);
	add_history(input);
	shell->tokens = tokenize(input, shell);
	if (!shell->tokens || parse(shell) != SUCCESS)
	{
		free(input);
		return (ERROR);
	}
	shell->exit_status = execute_commands(shell);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_commands(shell->commands);
	shell->commands = NULL;
	free(input);
	return (shell->exit_status);
}

static int	shell_loop(t_shell *shell)
{
	char	*input;
	int		status;

	status = 0;
	while (shell->running)
	{
		if (g_signal_status)
		{
			shell->exit_status = 128 + g_signal_status;
			g_signal_status = 0;
			continue ;
		}
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		status = process_input(input, shell);
	}
	return (status);
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
		do_one_command(argv[2], shell);
	status = shell_loop(shell);
	free_shell(shell);
	rl_clear_history();
	return (status);
}
