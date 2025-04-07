/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:25:59 by yabukirento       #+#    #+#             */
/*   Updated: 2025/04/07 19:34:52 by yabukirento      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
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

# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_ASSERT 255
# define ERROR_OPEN_REDIR 1
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'

/* グローバル変数 - シグナル処理用 */
extern int		g_signal_status;

typedef enum 	e_token_type	t_token_type;
typedef struct	s_token			t_token;
typedef struct	s_env			t_env;
typedef enum	e_node_type		t_node_type;
typedef struct	e_node			t_node;

/* トークンタイプの定義 */
enum e_token_type
{
	TOKEN_WORD,
	TOKEN_RESERVED,
	TOKEN_OPERATOR,
	TOKEN_EOF,
};

struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	bool			is_expanded;
};

/* 環境変数管理構造体 */
struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
};

enum	e_node_type
{
	ND_PIPELINE,
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};

struct	e_node
{
	t_token		*args;
	t_token		*delimiter;
	t_token		*filename;
	t_node_type	type;
	t_node		*command;
	t_node		*next;
	t_node		*redirects;
	bool		is_delim_unquoted;
	int			targetfd;
	int			filefd;
	int			stashed_targetfd;
	int			inpipe[2];
	int			outpipe[2];
};


/* シェル全体の状態管理構造体 */
typedef struct s_shell
{
	t_node		*node;
	t_token		*token;
	t_env		*env_list;
	char		**env_array;
	int			exit_status;
	int			running;
	bool		flag_error;
}	t_shell;

/* builtin*/
int		builtin_echo(char	**argv, t_shell *shell);
int		builtin_cd(char	**argv, t_shell *shell);
int		builtin_pwd(char	**argv, t_shell *shell);
int		builtin_export(char	**argv, t_shell *shell);
int		builtin_unset(char	**argv, t_shell *shell);
int		builtin_env(char	**argv, t_shell *shell);
int		builtin_exit(char	**argv, t_shell *shell);

/* environ */
t_env	*init_env_list(char **envp);
pid_t	exec_pipeline(t_node *node, t_shell *shell);
char	**env_list_to_array(t_env *env_list);
t_env	*get_env_node(t_env *env_list, char *key);
void	add_env_node(t_env **env_list, char *key, char *value);
void	remove_env_node(t_env **env_list, char *key);
void	update_env_array(t_shell *shell);
char	*get_env_value(t_env *env_list, char *key);

/* error */
void	fatal_error(char *msg, t_shell *shell);
void	assert_error(char *msg, t_shell *shell);
void	error_exit(char *location, char *msg, int status, t_shell *shell);

void	token_error(char *point, char **rest, char *input);
void	parse_error(char *point, t_token **rest, t_token *token);

void	ft_perror(char *s);
void	error_message(char *msg);
void	command_error(char *cmd, char *msg);
void	system_error(char *prefix);

/* exec */
void	exec(t_node *node, t_shell *shell);
bool	open_redir_file(t_node *node, t_shell *shell);
bool	is_builtin(t_node *node);
int	exec_builtin(t_node *node, t_shell *shell);
pid_t	exec_pipeline(t_node *node, t_shell *shell);
char	*search_path(const char *filename, t_shell *shell);
char	**token_list_to_argv(t_token *tok, t_shell *shell);


/* expand */
void	expand(t_shell *shell);
void	expand_parameter(t_node *node, t_shell *shell);
void	expand_word_splitting_token(t_token *token, t_shell *shell);
void	expand_word_splitting(t_node *node, t_shell *shell);
void	expand_quote_removal(t_node *node, t_shell *shell);
char	*expand_heredoc_line(char *line, t_shell *shell);
void	expand_variable_str(char **dst, char **rest, char *p, t_shell *shell);
void	expand_special_parameter_str(char **dst, char **rest, char *p, t_shell *shell);
void	append_char(char **src, char c, t_shell *shell);
void	append_single_quote(char **dst, char **rest, char *p, t_shell *shell);
void	append_double_quote(char **dst, char **rest, char *p, t_shell *shell);
void	trim_ifs(char **rest, char *p, t_shell *shell);
bool	is_special_parameter(char *s);
void	expand_parameter_token(t_token *token, t_shell *shell);
bool	consume_ifs(char **rest, char *line, t_shell *shell);

/* parse */
void	parse(t_shell *shell);
void	append_token(t_token **tok, t_token *elm);
t_node	*new_node(t_node_type type, t_shell *shell);
void	append_node(t_node **node, t_node *elm);

t_node	*redirect_out(t_token **rest, t_token *token, t_shell *shell);
t_node	*redirect_in(t_token **rest, t_token *token, t_shell *shell);
t_node	*redirect_append(t_token **rest, t_token *token, t_shell *shell);
t_node	*redirect_heredoc(t_token **rest, t_token *token, t_shell *shell);

/* pipe */
void	prepare_pipe(t_node *node, t_shell *shell);
void	prepare_pipe_child(t_node *node, t_shell *shell);
void	prepare_pipe_parent(t_node *node, t_shell *shell);
void	cpy_pipe(int dst[2], int src[2]);

/* redirect */
int	read_heredoc(const char *delimiter, bool is_delim_unquoted, t_shell *shell);
void	do_redirect(t_node *redir, t_shell *shell);
void	reset_redirect(t_node *redir, t_shell *shell);
bool	is_redirect(t_node *node);
int	stashfd(int fd, t_shell *shell);

/* signal */
void	setup_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	ignore_signals(void);
void	setup_child_signals(void);
void	defalut_signals(void);

/* tokenize */
void	tokenize(char	*line, t_shell *shell);
t_token	*new_token(char *value, t_token_type type, t_shell *shell);
t_token	*tokendup(t_token *token, t_shell *shell);
void	append_token(t_token **tok, t_token *elm);
t_token	*word(char **rest, char *line, t_shell *shell);
bool	is_eof(t_token *token);
int		is_space(char c);
bool	is_metacharacter(char c);
bool	is_word(const char *s);
bool	is_prefix(const char *s, const char *keyword);

/* util */
int	safe_close(int fd, t_shell *shell);
int	safe_dup(int fd, t_shell *shell);
int	safe_dup2(int from_fd, int to_fd, t_shell *shell);
int	safe_pipe(int fd[2], t_shell *shell);

void	*safe_malloc(size_t size, t_shell *shell);
void	*safe_calloc(size_t count, size_t size, t_shell *shell);
char	*safe_strdup(const char *s1, t_shell *shell);
char	*safe_strndup(const char *s1, size_t size, t_shell *shell);

void	free_node(t_node *node);
void	free_token(t_token *tok);
void	free_shell(t_shell *shell);
void	free_argv(char **argv);
void	free_env_list(t_env *env_list);

bool	is_alpha_under(char c);
bool	is_alpha_num_under(char c);
bool	is_variable(char *s);
bool	is_identifier(const char *s);
#endif