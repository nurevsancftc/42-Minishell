/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:51:08 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 21:31:33 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (current && current->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	while (current && current->type != TOKEN_EOF)
	{
		if ((current->type >= TOKEN_PIPE && current->type <= TOKEN_HEREDOC))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
				return (syntax_error_unexpected_token(current->next));
		}
		current = current->next;
	}
	return (1);
}

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
			if (add_argument(&arg_list, token_cursor) == -1)
				return (-1);
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
	if (token_cursor && token_cursor->type == TOKEN_EOF)
		return (NULL);
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
	if (!validate_syntax(tokens))
		return (NULL);
	return (parse_pipeline(&tokens));
}
