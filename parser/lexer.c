/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:11:22 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/21 02:42:54 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_greater(const char *line, t_token **token_list)
{
	if ((*(line + 1) == '>' && *(line + 2) != '>')
		&& (*(line + 1) == '>' && *(line + 2) != '<'))
	{
		add_token_to_list(token_list, create_token(">>", TOKEN_REDIR_APPEND));
		return (2);
	}
	else if ((*(line + 1) != '>') && (*(line + 1) != '<'))
	{
		add_token_to_list(token_list, create_token(">", TOKEN_REDIR_OUT));
		return (1);
	}
	else
	{
		if (*(line + 1) == '<')
			print_syntax_error("`<'\n");
		else if (*(line + 2) == '>')
			print_syntax_error("`>'\n");
		else if (*(line + 2) == '<')
			print_syntax_error("`<'\n");
		return (-1);
	}
}

static int	handle_less(const char *line, t_token **token_list)
{
	if ((*(line + 1) == '<' && *(line + 2) != '<')
		&& (*(line + 1) == '<' && *(line + 2) != '>'))
	{
		add_token_to_list(token_list, create_token("<<", TOKEN_HEREDOC));
		return (2);
	}
	else if ((*(line + 1) != '<') && (*(line + 1) != '>'))
	{
		add_token_to_list(token_list, create_token("<", TOKEN_REDIR_IN));
		return (1);
	}
	else
	{
		if (*(line + 1) == '>')
			print_syntax_error("`newline'\n");
		else if (*(line + 2) == '>')
			print_syntax_error("`>'\n");
		else if (*(line + 2) == '<')
			print_syntax_error("`<'\n");
		return (-1);
	}
}

static int	handle_metachar(const char *line, t_token **token_list)
{
	if (*line == '>')
		return (handle_greater(line, token_list));
	if (*line == '<')
		return (handle_less(line, token_list));
	if (*line == '|')
	{
		add_token_to_list(token_list, create_token("|", TOKEN_PIPE));
		return (1);
	}
	return (0);
}

static int	handle_word(const char *line, t_token **token_list)
{
	int		i;
	char	*word;

	i = 0;
	while (line[i] && is_delimiter(line[i]) == DELIM_NONE)
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			i = skip_quote(line, i);
			if (i == -1)
				return (-1);
		}
		else
			i++;
	}
	word = ft_substr(line, 0, i);
	add_token_to_list(token_list, create_token(word, TOKEN_WORD));
	free(word);
	return (i);
}

t_token	*lexer(const char *line)
{
	t_token	*token_list;
	int		i;
	int		processed_len;
	int		delim;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		processed_len = 0;
		delim = is_delimiter(line[i]);
		if (delim == DELIM_WHITESPACE)
			processed_len = 1;
		else if (delim == DELIM_METACHAR)
			processed_len = handle_metachar(&line[i], &token_list);
		else
			processed_len = handle_word(&line[i], &token_list);
		if (processed_len < 0)
		{
			free_token_list(token_list);
			return (NULL);
		}
		i += processed_len;
	}
	return (token_list);
}
