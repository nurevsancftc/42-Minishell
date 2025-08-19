/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 19:09:45 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 20:46:09 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/parser_utils.c

#include "minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_redir_node(void *content)
{
	t_redir	*redir;

	if (!content)
		return ;
	redir = (t_redir *)content;
	if (redir->filename)
		free(redir->filename);
	free(redir);
}

void	free_cmd_tree(t_command_chain *head)
{
	t_command_chain		*tmp_chain;
	t_simple_command	*cmd;

	while (head)
	{
		tmp_chain = head->next;
		cmd = head->simple_command;
		if (cmd)
		{
			ft_free_array(cmd->args);
			if (cmd->redirections)
				ft_lstclear(&cmd->redirections, free_redir_node);
			free(cmd);
		}
		free(head);
		head = tmp_chain;
	}
}

char	*ft_strunquote(const char *str)
{
	int		len;
	char	quote;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	quote = str[0];
	if (len >= 2 && (quote == '\'' || quote == '"') && str[len - 1] == quote)
		return (ft_substr(str, 1, len - 2));
	return (ft_strdup(str));
}

void	add_cmd_to_chain(t_command_chain **head, t_command_chain *new_node)
{
	t_command_chain	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}
