/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:31 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 20:12:48 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (DELIM_WHITESPACE);
	if (c == '|' || c == '<' || c == '>')
		return (DELIM_METACHAR);
	return (DELIM_NONE);
}

t_token	*create_token(char *value, t_token_type type)
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

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

int	skip_quote(const char *line, int i)
{
	char	quote_char;

	quote_char = line[i];
	i++;
	while (line[i] && line[i] != quote_char)
		i++;
	if (line[i] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (-1);
	}
	return (i + 1);
}
