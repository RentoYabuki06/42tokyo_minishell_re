/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryabuki <ryabuki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/04/06 16:41:28 by ryabuki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
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
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

/* グローバル変数 - シグナル処理用 */
extern int		g_signal_status;

/* トークンタイプの定義 */
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

/* トークン構造体 */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

/* 文字列操作ユーティリティ */
char	*ft_strjoin_free(char *s1, char *s2);
int		ft_strcmp(const char *s1, const char *s2);

/* コマンド構造体 */
typedef struct s_command
{
	char				**args;
	int					input_fd;
	int					output_fd;
	t_token				*redirects;
	struct s_command	*next;
}	t_command;

/* 環境変数管理構造体 */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* シェル全体の状態管理構造体 */
typedef struct s_shell
{
	t_token		*tokens;
	t_command	*commands;
	t_env		*env_list;
	char		**env_array;
	int			exit_status;
	int			running;
}	t_shell;

/* メイン関数 */
int		main(int argc, char **argv, char **envp);
int		process_input(char *input, t_shell *shell);

/* 初期化と終了処理 */
t_shell	*init_shell(char **envp);
void	free_shell(t_shell *shell);

/* 環境変数ユーティリティ */
t_env	*init_env_list(char **envp);
char	**env_list_to_array(t_env *env_list);
t_env	*get_env_node(t_env *env_list, char *key);
void	add_env_node(t_env **env_list, char *key, char *value);
void	remove_env_node(t_env **env_list, char *key);
void	update_env_array(t_shell *shell);
char	*get_env_value(t_env *env_list, char *key);

/* レキサー (字句解析) */
t_token	*tokenize(char *input, t_shell *shell);
void	free_tokens(t_token *tokens);
t_token	*create_token(t_token_type type, char *value);
void	add_token(t_token **tokens, t_token *new_token);

/* パーサー (構文解析) */
int		parse(t_shell *shell);
void	free_commands(t_command *commands);
t_command *create_command(void);
void	add_command(t_command **commands, t_command *new_command);

/* 環境変数展開 */
char	*expand_variables(char *str, t_shell *shell);

/* コマンド実行 */
int		execute_commands(t_shell *shell);
int		execute_builtin(t_command *cmd, t_shell *shell);
int		execute_external_standalone(t_command *cmd, t_shell *shell);
int		execute_external_forked(t_command *cmd, t_shell *shell);
int		is_builtin(char *cmd);
char	*find_executable(char *cmd, t_env *env_list);

/* パイプとリダイレクト */
int		setup_pipes(t_command *commands);
int		setup_redirects(t_command *cmd);
int		execute_pipeline(t_command *commands, t_shell *shell);
int		do_one_command(char *input, t_shell *shell);

/* ビルトインコマンド */
int		builtin_echo(t_command *cmd, t_shell *shell);
int		builtin_cd(t_command *cmd, t_shell *shell);
int		builtin_pwd(t_command *cmd, t_shell *shell);
int		builtin_export(t_command *cmd, t_shell *shell);
int		builtin_unset(t_command *cmd, t_shell *shell);
int		builtin_env(t_command *cmd, t_shell *shell);
int		builtin_exit(t_command *cmd, t_shell *shell);

/* シグナルハンドリング */
void	setup_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	ignore_signals(void);
void	setup_child_signals(void);
void	defalut_signals(void);

/* エラーハンドリング */
void	error_message(char *msg);
void	command_error(char *cmd, char *msg);
void	system_error(char *prefix);

/* メモリ管理 */
void	*safe_malloc(size_t size);
void	free_array(char **array);
void	free_env_list(t_env *env_list);

#endif