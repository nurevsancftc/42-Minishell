/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:58:42 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 19:11:42 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_word(const char *word, t_shell *shell)
{
	char	*new_word;
	char	quote_type;

	new_word = ft_strdup("");
	quote_type = 0;
	while (*word)
	{
		if (quote_type == 0 && (*word == '\'' || *word == '"'))
			quote_type = *word++;
		else if (quote_type != 0 && *word == quote_type)
		{
			quote_type = 0;
			word++;
		}
		else if (quote_type != '\'' && *word == '$')
			new_word = append_expanded_var(new_word, &word, shell);
		else
			new_word = append_char(new_word, *word++);
	}
	return (new_word);
}

static void	expand_single_redir(t_redir *redir, t_shell *shell)
{
	char	*temp_filename;

	if (!redir || redir->type == TOKEN_HEREDOC || !redir->filename)
		return ;
	temp_filename = redir->filename;
	redir->filename = expand_word(temp_filename, shell);
	free(temp_filename);
	if (!redir->was_quoted && (ft_strchr(redir->filename, ' ')
			|| redir->filename[0] == '\0'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->filename, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		shell->exit_code = 1;
	}
}

void	expand_redirections(t_simple_command *cmd, t_shell *shell)
{
	t_list	*redir_node;
	t_redir	*redir;

	if (!cmd || !cmd->redirections)
		return ;
	redir_node = cmd->redirections;
	while (redir_node)
	{
		redir = (t_redir *)redir_node->content;
		expand_single_redir(redir, shell);
		redir_node = redir_node->next;
	}
}

void	expand_simple_command(t_simple_command *cmd, t_shell *shell)
{
	t_list	*arg_list;

	if (!cmd || !cmd->args)
		return ;
	arg_list = array_to_list(cmd->args);
	if (!arg_list)
		return ;
	expand_args_list(&arg_list, shell);
	expand_redirections(cmd, shell);
	free_args(cmd->args);
	cmd->args = list_to_array(arg_list);
	ft_lstclear(&arg_list, free);
}

void	expander(t_command_chain *cmd_chain, t_shell *shell)
{
	while (cmd_chain)
	{
		expand_simple_command(cmd_chain->simple_command, shell);
		cmd_chain = cmd_chain->next;
	}
}
