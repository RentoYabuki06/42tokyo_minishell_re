/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:12:06 by myokono           #+#    #+#             */
/*   Updated: 2025/02/28 21:13:59 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Global variable for signal handling */
int		g_signal_status = 0;

/**
 * Function to initialize the shell
 * @param envp Array of environment variables
 * @return Initialized shell structure
 */
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = safe_malloc(sizeof(t_shell));
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env_list = init_env_list(envp);
	shell->env_array = env_list_to_array(shell->env_list);
	shell->exit_status = 0;
	shell->running = 1;
	return (shell);
}

/**
 * Function to free the shell's memory
 * @param shell Shell structure
 */
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

/**
 * Function to process the input line
 * @param input Input string
 * @param shell Shell structure
 * @return Processing result
 */
static int	process_input(char *input, t_shell *shell)
{
	if (!input || ft_strlen(input) == 0)
		return (SUCCESS);
	add_history(input);
	shell->tokens = tokenize(input, shell);
	if (!shell->tokens)
	{
		free(input);
		return (ERROR);
	}
	if (parse(shell) != SUCCESS)
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

/**
 * Main function
 * @param argc Number of arguments
 * @param argv Argument array
 * @param envp Environment variable array
 * @return Exit status
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*input;

	shell = init_shell(envp);
	setup_signals();
	if (argc >= 3 && ft_strcmp(argv[1], "-c") == 0)
	{
		process_input(ft_strdup(argv[2]), shell);
		free_shell(shell);
		clear_history();
		return (shell->exit_status);
	}
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
