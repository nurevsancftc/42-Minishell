/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:22:41 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 19:11:42 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_redir_filename(t_redir *redir, char *delimiter)
{
	if (redir->type == TOKEN_HEREDOC)
	{
		if (delimiter[0] == '\'' || delimiter[0] == '"')
			redir->expand_in_heredoc = 0;
		else
			redir->expand_in_heredoc = 1;
		redir->filename = ft_strunquote(delimiter);
	}
	else
	{
		if (delimiter[0] == '\'' || delimiter[0] == '"')
			redir->was_quoted = 1;
		redir->expand_in_heredoc = 0;
		redir->filename = ft_strdup(delimiter);
	}
	if (!redir->filename)
		return (-1);
	return (0);
}

static int	add_redir_to_cmd(t_redir *redir, t_simple_command *cmd)
{
	t_list	*new_node;

	new_node = ft_lstnew(redir);
	if (!new_node)
	{
		free(redir->filename);
		free(redir);
		return (-1);
	}
	ft_lstadd_back(&cmd->redirections, new_node);
	return (0);
}

int	handle_redirection(t_token **token_cursor, t_simple_command *cmd)
{
	t_redir	*redir;
	t_token	*redir_token;
	t_token	*delimiter_token;

	redir_token = *token_cursor;
	delimiter_token = redir_token->next;
	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (-1);
	redir->type = redir_token->type;
	redir->was_quoted = 0;
	if (set_redir_filename(redir, delimiter_token->value) == -1)
		return (free(redir), -1);
	if (add_redir_to_cmd(redir, cmd) == -1)
		return (-1);
	*token_cursor = delimiter_token->next;
	return (0);
}
