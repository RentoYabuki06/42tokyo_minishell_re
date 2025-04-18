/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 23:54:28 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/18 10:23:02 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../srcs/libft/libft.h"
# include "../srcs/gnl/includes/get_next_line.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <limits.h>

# define SUCCESS 0
# define ERROR 1
# define NO_TOKEN 123
# define MAXNUM_FD 1024

extern int		g_signal_status;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**args;
	int					input_fd;
	int					output_fd;
	t_token				*redirects;
	struct s_command	*next;
}	t_command;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_token		*tokens;
	t_command	*commands;
	t_env		*env_list;
	char		**env_array;
	int			exit_status;
	int			running;
}	t_shell;

/* main */
int			main(int argc, char **argv, char **envp);
int			process_input(char *input, t_shell *shell);

/* env_uyild*/
t_env		*init_env_list(char **envp);
char		**env_list_to_array(t_env *env_list);
t_env		*get_env_node(t_env *env_list, char *key);
void		add_env_node(t_env **env_list, char *key, char *value);
void		remove_env_node(t_env **env_list, char *key);
void		update_env_array(t_shell *shell);
char		*get_env_value(t_env *env_list, char *key);

/* lexer */
t_token		*tokenize(char *input, t_shell *shell);
void		free_tokens(t_token *tokens);
t_token		*create_token(t_token_type type, char *value);
int			add_token(t_token **tokens, t_token *new_token);
int			handle_dollar_single_quote(char *input, int *i, char **result);
int			handle_single_quote(char *input, int *i, char **result);
int			dollar_doublequote(char *input, int *i, \
			char **result, t_shell *shell);
int			handle_double_quote(char *input, int *i, \
			char **result, t_shell *shell);
int			expand_env_var(char *input, int *i, char **result, t_shell *shell);
int			is_space(char c);
int			handle_word_token(char *input, int *i, \
			t_token **tokens, t_shell *shell);

/* parser */
int			parse(t_shell *shell);
void		free_commands(t_command *commands);
t_command	*create_command(void);
void		add_command(t_command **commands, t_command *new_command);

/* parser/command */
int			add_arg(t_command *cmd, char *arg);
int			handle_pipe(t_token **tokens, t_command *cmd, t_command **commands);

/* parser/redirect */
t_token		*process_redirect_tokens(t_token **tokens, t_token_type type);
void		add_redirect_to_command(t_command *cmd, t_token *redirect_token);
int			handle_redirect(t_token **tokens, t_command *cmd);

/* env */
char		*expand_variables(char *str, t_shell *shell);

/* ececutor */
int			execute_commands(t_shell *shell);
int			execute_builtin(t_command *cmd, t_shell *shell);
int			execute_external_standalone(t_command *cmd, t_shell *shell);
int			is_builtin(char *cmd);
char		*find_executable(char *cmd, t_env *env_list);

/* pip redirects */
int			setup_pipes(t_command *commands);
int			execute_external_forked(t_command *cmd, t_shell *shell);
void		setup_child_io(t_command *cmd);
int			setup_redirects(t_command *cmd);
int			setup_redirect_heredoc(t_command *cmd, char *delimiter);
int			execute_pipeline(t_command *commands, t_shell *shell);
pid_t		static_execute_commands(t_command *current, \
			t_shell *shell, int stdin_backup);

/* builtins */
int			builtin_echo(t_command *cmd, t_shell *shell);
int			builtin_cd(t_command *cmd, t_shell *shell);
int			builtin_pwd(t_command *cmd, t_shell *shell);
int			builtin_export(t_command *cmd, t_shell *shell);
int			builtin_unset(t_command *cmd, t_shell *shell);
bool		is_valid_identifier(char *key);
int			print_sorted_env(t_env *env_list, int fd);
int			builtin_env(t_command *cmd, t_shell *shell);
int			builtin_exit(t_command *cmd, t_shell *shell);
char		*simplify_path(const char *path);

/* signal*/
void		setup_signals(void);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
void		ignore_signals(void);
void		default_signals(void);
void		setup_signal_heredoc(void);

/* error */
void		error_message(const char *msg);
void		system_error(const char *prefix);
void		print_error(const char *s1, const char *s2);

/* memory */
void		free_array(char **array);
void		free_env_list(t_env *env_list);
void		free_shell(t_shell *shell);

/* utils */
int			ft_strcmp(const char *s1, const char *s2);
bool		join_result(char **result, char *add);
bool		join_result_free(char **result, char *add);

int			setup_redir_return_fd(t_token_type type, char *delimiter);
int			handle_heredoc_fd(int *heredoc_fd, char *filename);
int			process_redirect(t_command *cmd, t_token *token, \
			t_token *next, int *heredoc_fd);
int			setup_redir(t_command *cmd, t_token_type type, char *filename);
bool		loop(int pipe_fd[2], char *delimiter, char **saved);
int			process_heredocs_in_parent(t_shell *shell);

#endif