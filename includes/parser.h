/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:33:33 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/04 18:46:12 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// includes/parser.h

#ifndef PARSER_H
# define PARSER_H

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

typedef struct s_env
{
	char			*key;
	char			*value;
	// struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_list	*env_list;
	int		exit_code;
}	t_shell;

typedef struct s_redir
{
	t_token_type	type;       // Yönlendirme tipi (T_REDIR_OUT, vb.)
	char			*filename;  // Yönlendirilecek dosyanın adı
}	t_redir;

typedef struct s_simple_command
{
	char	**args;      // Komut ve argümanları (execve'e uygun)
	t_list	*redirections; // t_redir'lerden oluşan bir liste   //LİBFT DEKİ LİST T_LİST
}	t_simple_command;

typedef struct s_command_chain
{
	t_simple_command		*simple_command; // Bu düğümdeki basit komut
	struct s_command_chain	*next;       // Pipe ile sonraki komuta işaretçi
}	t_command_chain;


t_list	*create_env_list(char **envp);
void	main_loop(t_shell *shell);

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


// # include "minishell.h"
// Temizlik fonksiyonları
// void		free_tokens(t_token *tokens);
// void		free_ast(t_command_chain *ast);
void free_token_list(t_token *head);
void free_cmd_tree(t_command_chain *head);

t_command_chain	*parser(t_token *tokens);
int	populate_simple_cmd(t_simple_command *cmd, t_token **token_cursor);


void	print_tokens(t_token *tokens);



#endif