/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 21:28:14 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/21 02:44:22 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error_unexpected_token(t_token *token)
{
	if (token)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near ", 2);
		ft_putstr_fd("unexpected token `newline'\n", 2);
	}
	return (0);
}

int	add_argument(t_list **arg_list, t_token **token_cursor)
{
	char	*arg_val;
	t_list	*new_node;

	arg_val = ft_strdup((*token_cursor)->value);
	if (!arg_val)
		return (ft_lstclear(arg_list, free), -1);
	new_node = ft_lstnew(arg_val);
	if (!new_node)
		return (free(arg_val), ft_lstclear(arg_list, free), -1);
	ft_lstadd_back(arg_list, new_node);
	*token_cursor = (*token_cursor)->next;
	return (0);
}
