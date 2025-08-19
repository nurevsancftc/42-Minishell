/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:21:17 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 23:36:30 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*process_node(t_list *node, t_list **arg_list,
	t_list **prev, t_shell *shell)
{
	char	*original_word;
	char	*expanded_word;
	t_list	*next_node;

	original_word = (char *)node->content;
	expanded_word = expand_word(original_word, shell);
	node->content = expanded_word;
	if (expanded_word[0] == '\0' && !is_originally_quoted(original_word))
	{
		next_node = node->next;
		if (*prev)
			(*prev)->next = next_node;
		else
			*arg_list = next_node;
		ft_lstdelone(node, free);
		free(original_word);
		return (next_node);
	}
	free(original_word);
	*prev = node;
	return (node->next);
}

void	expand_args_list(t_list **arg_list, t_shell *shell)
{
	t_list	*current_node;
	t_list	*prev_node;

	current_node = *arg_list;
	prev_node = NULL;
	while (current_node)
		current_node = process_node(current_node, arg_list, &prev_node, shell);
}
