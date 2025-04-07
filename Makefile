# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yabukirento <yabukirento@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/01 00:00:00 by user              #+#    #+#              #
#    Updated: 2025/04/07 19:47:33 by yabukirento      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = 
INCLUDES = -I./includes -I./libft
LIBS = -L./libft -lft -lreadline

# ソースファイルディレクトリ
SRCS_DIR = srcs
BUILTIN_DIR = $(SRCS_DIR)/builtin
ENV_DIR = $(SRCS_DIR)/environ
ERROR_DIR = $(SRCS_DIR)/error
EXEC_DIR = $(SRCS_DIR)/exec
EXPAND_DIR = $(SRCS_DIR)/expand
PARSE_DIR = $(SRCS_DIR)/parse
PIPE_DIR = $(SRCS_DIR)/pipe
REDIR_DIR = $(SRCS_DIR)/redirect
TOKENIZE_DIR = $(SRCS_DIR)/tokenize
SIGNAL_DIR = $(SRCS_DIR)/signal
UTIL_DIR = $(SRCS_DIR)/util

# ソースファイル
SRCS = $(SRCS_DIR)/main.c \
    $(BUILTIN_DIR)/cd.c \
    $(BUILTIN_DIR)/echo.c \
    $(BUILTIN_DIR)/env.c \
    $(BUILTIN_DIR)/exit.c \
    $(BUILTIN_DIR)/export.c \
    $(BUILTIN_DIR)/pwd.c \
    $(BUILTIN_DIR)/unset.c \
    $(ENV_DIR)/env_utils.c \
    $(ENV_DIR)/env_init.c \
    $(ERROR_DIR)/error.c \
    $(ERROR_DIR)/error_exit.c \
    $(ERROR_DIR)/syntax_error.c \
    $(EXEC_DIR)/exec.c \
    $(EXEC_DIR)/exec_builtin.c \
    $(EXEC_DIR)/is_builtin.c \
    $(EXEC_DIR)/open_redir.c \
    $(EXEC_DIR)/search_path.c \
    $(EXEC_DIR)/token_list_to_array.c \
    $(EXPAND_DIR)/append_quote.c \
    $(EXPAND_DIR)/expand_heredoc.c \
    $(EXPAND_DIR)/expand_parameter.c \
    $(EXPAND_DIR)/expand_special.c \
    $(EXPAND_DIR)/expand_variable.c \
    $(EXPAND_DIR)/expand_word_split.c \
    $(EXPAND_DIR)/expand.c \
    $(EXPAND_DIR)/ifs.c \
    $(EXPAND_DIR)/quote_removal.c \
    $(PARSE_DIR)/parse.c \
    $(PARSE_DIR)/parse_redirection.c \
    $(PARSE_DIR)/node.c \
    $(PIPE_DIR)/pipe.c \
    $(REDIR_DIR)/heredoc.c \
    $(REDIR_DIR)/redirect.c \
    $(REDIR_DIR)/stashfd.c \
    $(SIGNAL_DIR)/signals.c \
    $(TOKENIZE_DIR)/token.c \
    $(TOKENIZE_DIR)/tokenize.c \
    $(TOKENIZE_DIR)/tokenize_utils.c \
    $(TOKENIZE_DIR)/word.c \
    $(UTIL_DIR)/allocate_memory.c \
    $(UTIL_DIR)/check_character.c \
    $(UTIL_DIR)/free.c \
    $(UTIL_DIR)/operate_fd.c \

# オブジェクトファイル
OBJS = $(SRCS:.c=.o)

# libftのパス
LIBFT = ./libft/libft.a

# ディレクトリ作成コマンド
MKDIR = mkdir -p

# ルール定義
all: $(NAME)

# libftをコンパイル
$(LIBFT):
	$(MAKE) -C ./libft

# オブジェクトファイル生成
%.o: %.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 実行ファイル生成
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(INCLUDES) $(LIBS) -o $(NAME)
	@echo "$(NAME) has been created!"

test: $(NAME)
	@echo "Running tests..."
	@bash ./mytester.sh
	@echo "Tests completed!"

clean:
	$(MAKE) -C ./libft clean
	rm -f $(OBJS)
	@echo "Object files cleaned!"

fclean: clean
	$(MAKE) -C ./libft fclean
	rm -f $(NAME)
	@echo "$(NAME) and object files cleaned!"

re: fclean all

.PHONY: all clean fclean re