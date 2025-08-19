/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:51:08 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 20:42:20 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_simple_command	*create_simple_cmd(void)
{
	t_simple_command	*cmd;

	cmd = malloc(sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

static int	validate_and_count_args(t_token *token, int *arg_count)
{
	*arg_count = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type == TOKEN_WORD)
			(*arg_count)++;
		else
		{
			if (!token->next || token->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token `");
				if (token->next)
					printf("%s'\n", token->next->value);
				else
					printf("newline'\n");
				return (-1);
			}
			token = token->next;
		}
		token = token->next;
	}
	return (0);
}

int	populate_simple_cmd(t_simple_command *cmd, t_token **token_cursor)
{
	int	arg_count;
	int	i;

	if (validate_and_count_args(*token_cursor, &arg_count) == -1)
		return (-1);
	cmd->args = ft_calloc(arg_count + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	i = 0;
	while (*token_cursor && (*token_cursor)->type != TOKEN_PIPE)
	{
		if ((*token_cursor)->type == TOKEN_WORD)
			cmd->args[i++] = ft_strdup((*token_cursor)->value);
		else
		{
			if (handle_redirection(token_cursor, cmd) == -1)
				return (-1);
			continue ;
		}
		*token_cursor = (*token_cursor)->next;
	}
	return (0);
}

static int	parser_process_command(
	t_command_chain	**cmd_head,
	t_token	**current_token)
{
	t_command_chain	*new_cmd_node;

	new_cmd_node = ft_calloc(1, sizeof(t_command_chain));
	if (!new_cmd_node)
		return (free_cmd_tree(*cmd_head), -1);
	new_cmd_node->simple_command = create_simple_cmd();
	if (!new_cmd_node->simple_command)
		return (free(new_cmd_node), free_cmd_tree(*cmd_head), -1);
	if (populate_simple_cmd(new_cmd_node->simple_command, current_token) == -1)
		return (free_cmd_tree(new_cmd_node), free_cmd_tree(*cmd_head), -1);
	add_cmd_to_chain(cmd_head, new_cmd_node);
	if (*current_token != NULL)
	{
		if ((*current_token)->type != TOKEN_PIPE)
			return (free_cmd_tree(*cmd_head), -1);
		*current_token = (*current_token)->next;
		if (*current_token == NULL || (*current_token)->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error ", 2);
			ft_putstr_fd("near unexpected token `|'\n", 2);
			return (free_cmd_tree(*cmd_head), -1);
		}
	}
	return (0);
}

t_command_chain	*parser(t_token *tokens)
{
	t_command_chain	*cmd_head;
	t_token			*current_token;

	if (!tokens)
		return (NULL);
	cmd_head = NULL;
	current_token = tokens;
	if (current_token->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (NULL);
	}
	while (current_token != NULL)
	{
		if (parser_process_command(&cmd_head, &current_token) == -1)
			return (NULL);
	}
	return (cmd_head);
}
