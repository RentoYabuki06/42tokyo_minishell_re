/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokono <myokono@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/02/27 12:10:19 by myokono          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * 新しいトークンを作成する関数
 * @param type トークンタイプ
 * @param value トークンの値（所有権が移転する）
 * @return 作成されたトークン
 */
t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = safe_malloc(sizeof(t_token));
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

/**
 * トークンをリストに追加する関数
 * @param tokens トークンリストへのポインタ
 * @param new_token 追加する新しいトークン
 */
void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	
	current = *tokens;
	while (current->next)
		current = current->next;
	
	current->next = new_token;
}