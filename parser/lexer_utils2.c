/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:42:47 by nuciftci          #+#    #+#             */
/*   Updated: 2025/08/21 02:44:07 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putstr_fd(str, 2);
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
	char	quote;

	quote = line[i];
	i++;
	while (line[i] && line[i] != quote)
	{
		i++;
	}
	if (line[i] == quote)
	{
		i++;
	}
	return (i);
}
