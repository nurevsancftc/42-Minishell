/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:51:08 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 19:11:42 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_simple_command	*create_simple_command(void)
{
	t_simple_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

int	populate_simple_cmd(t_simple_command *cmd, t_token **token_cursor)
{
	t_list	*arg_list;
	t_list	*new_node;
	char	*arg_val;

	arg_list = NULL;
	while (*token_cursor && (*token_cursor)->type != TOKEN_PIPE)
	{
		if ((*token_cursor)->type >= TOKEN_REDIR_IN
			&& (*token_cursor)->type <= TOKEN_HEREDOC)
		{
			if (handle_redirection(token_cursor, cmd) == -1)
				return (ft_lstclear(&arg_list, free), -1);
		}
		else if ((*token_cursor)->type == TOKEN_WORD)
		{
			arg_val = ft_strdup((*token_cursor)->value);
			if (!arg_val)
				return (ft_lstclear(&arg_list, free), -1);
			new_node = ft_lstnew(arg_val);
			if (!new_node)
				return (free(arg_val), ft_lstclear(&arg_list, free), -1);
			ft_lstadd_back(&arg_list, new_node);
			*token_cursor = (*token_cursor)->next;
		}
	}
	cmd->args = list_to_array(arg_list);
	ft_lstclear(&arg_list, free);
	return (0);
}

static t_command_chain	*parse_pipeline(t_token **tokens)
{
	t_command_chain	*head;
	t_command_chain	*current_chain;
	t_token			*token_cursor;

	head = NULL;
	token_cursor = *tokens;
	while (token_cursor && token_cursor->type != TOKEN_EOF)
	{
		current_chain = ft_calloc(1, sizeof(t_command_chain));
		if (!current_chain)
			return (free_cmd_tree(head), NULL);
		current_chain->simple_command = create_simple_command();
		if (!current_chain->simple_command)
			return (free(current_chain), free_cmd_tree(head), NULL);
		if (populate_simple_cmd(current_chain->simple_command,
				&token_cursor) == -1)
			return (free_cmd_tree(current_chain), free_cmd_tree(head), NULL);
		add_cmd_to_chain(&head, current_chain);
		if (token_cursor && token_cursor->type == TOKEN_PIPE)
			token_cursor = token_cursor->next;
	}
	return (head);
}

t_command_chain	*parser(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	return (parse_pipeline(&tokens));
}