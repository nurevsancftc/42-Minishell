/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:31 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 19:28:26 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/lexer_utils.c

#include "minishell.h"

// Boşluk karakteri mi diye kontrol eder.
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// Shell için özel anlamı olan bir metakarakter mi diye kontrol eder.
int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Yeni bir token oluşturur ve bellekte yer ayırır.
t_token	*create_token(char *value, t_token_type type)
{
	t_token *new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value); // Değeri kopyalamak önemli!
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

// Oluşturulan yeni bir token'ı listenin sonuna ekler.
void	add_token_to_list(t_token **list_head, t_token *new_token)
{
	t_token	*current;

	if (!list_head || !new_token)
		return;
	if (!*list_head)
	{
		*list_head = new_token;
		return;
	}
	current = *list_head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

// Bir token listesini tamamen serbest bırakır.
void free_token_list(t_token *head)
{
    t_token *tmp;

    while (head)
    {
        tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
    }
}