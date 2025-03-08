/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:12:06 by myokono           #+#    #+#             */
/*   Updated: 2025/03/08 13:59:06 by myokono          ###   ########.fr       */
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

static int	process_input(char *input, t_shell *shell)
{
	if (!input || ft_strlen(input) == 0)
		return (SUCCESS);
	// add_history(input);
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
	return (SUCCESS);
}

static int	do_one_command(char *input, t_shell *shell)
{
	process_input(ft_strdup(input), shell);
	free_shell(shell);
	clear_history();
	exit (shell->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*input;

	shell = init_shell(envp);
	if (!shell)
		return (ERROR);
	setup_signals();
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
		do_one_command(argv[2], shell);
	while (shell->running)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		process_input(input, shell);
	}
	free_shell(shell);
	clear_history();
	return (shell->exit_status);
}
