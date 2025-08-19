/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:56:40 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/19 20:15:21 by aldurmaz         ###   ########.fr       */
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
	TOKEN_EOF,
}	t_token_type;

enum e_delim_type
{
	DELIM_NONE = 0,
	DELIM_WHITESPACE = 1,
	DELIM_METACHAR = 2
};

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	int				expand_in_heredoc;
}	t_redir;

t_token	*lexer(const char *line);
t_token	*create_token(char *value, t_token_type type);
void	add_token_to_list(t_token **list_head, t_token *new_token);
void	free_token_list(t_token *head);
void    free_args(char **args);
int		is_delimiter(char c);
int		handle_quotes(const char *line, t_token **token_list);
int		skip_quote(const char *line, int i);


#endif