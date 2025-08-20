/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuciftci <nuciftci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:56:40 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/20 18:53:42 by nuciftci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef enum e_delimiter_type
{
	DELIM_NONE,
	DELIM_WHITESPACE,
	DELIM_METACHAR
}	t_delimiter_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	int				expand_in_heredoc;
	int				was_quoted;
}	t_redir;

t_token				*lexer(const char *input);
void				free_token_list(t_token *head);
t_token				*create_token(const char *value, t_token_type type);
void				add_token_to_list(t_token **list, t_token *new_token);
t_delimiter_type	is_delimiter(char c);
int					skip_quote(const char *line, int i);

#endif