/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:33:33 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/28 23:19:06 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// includes/parser.h

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include <stdlib.h>

// --- 1. LEXER İÇİN YAPILAR ---

// Girdiyi ayırdığımız token'ların türlerini tanımlayan enum.
// Bu, parser'ın "kelime" mi "pipe" mı geldiğini anlamasını sağlar.
typedef enum e_token_type
{
	TOKEN_WORD,         // Kelime (komut, argüman, dosya adı vb.)
	TOKEN_PIPE,         // |
	TOKEN_REDIR_IN,     // <
	TOKEN_REDIR_OUT,    // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_EOF           // Girdinin sonu (End of File)
}	t_token_type;

// Her bir token'ı temsil eden bağlı liste düğümü.
// "ls -l | grep" -> [ls] -> [-l] -> [|] -> [grep]
typedef struct s_token
{
	char			*value;   // Token'ın içeriği (örn: "ls", "|")
	t_token_type	type;     // Token'ın türü
	struct s_token	*next;
}	t_token;


// --- Fonksiyon Protototipleri ---

// lexer.c
t_token	*lexer(const char *line);

// quotes.c
int		handle_quotes(const char *line, t_token **token_list);

// parser_utils.c
void	add_token_to_list(t_token **list_head, t_token *new_token);
t_token	*create_token(char *value, t_token_type type);
void	free_token_list(t_token *head);
int		is_metachar(char c);
int		is_whitespace(char c);


// Temizlik fonksiyonları
void		free_tokens(t_token *tokens);
void		free_ast(t_command_chain *ast);


#endif