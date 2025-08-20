/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:31 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/21 02:43:51 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_delimiter_type	is_delimiter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (DELIM_WHITESPACE);
	if (c == '|' || c == '<' || c == '>')
		return (DELIM_METACHAR);
	return (DELIM_NONE);
}

t_token	*create_token(const char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token_to_list(t_token **list_head, t_token *new_token)
{
	t_token	*current;

	if (!list_head || !new_token)
		return ;
	if (!*list_head)
	{
		*list_head = new_token;
		return ;
	}
	current = *list_head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
