/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 03:33:33 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/07/27 15:13:15 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// includes/parser.h

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h" // Ana başlık dosyasını dahil et (libft vb. için)
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


// --- 2. PARSER'IN ÜRETECEĞİ YAPI (AST - SOYUT SÖZDİZİMİ AĞACI) ---
// Bu, projenin en önemli veri yapısıdır. Executor ve built-in'ler bunu kullanacak.

// Tek bir basit komutu temsil eder (pipe'lar arası her bir parça).
// Örnek: "grep -i "hello" < input.txt > output.txt"
typedef struct s_simple_cmd
{
	char	**args;          // Komut ve argümanları: {"grep", "-i", "hello", NULL}
	char	*redir_in;       // Girdi yönlendirme dosyası: "input.txt"
	char	*redir_out;      // Çıktı yönlendirme dosyası: "output.txt"
	int		append_mode;     // Çıktı append modu (>> için 1, > için 0)
	char	*heredoc_eof;    // Here-document sonlandırıcısı
	// Birden fazla yönlendirme için bu yapıları liste haline getirebilirsin.
	// Ama başlangıç için bu kadarı yeterli.
}	t_simple_cmd;


// Komut tablosu. Pipe'larla birbirine bağlı basit komutların listesi.
// Örnek: cmd1 | cmd2 | cmd3
typedef struct s_command
{
	t_simple_cmd		*simple_cmd; // Bu düğümdeki basit komut
	struct s_command	*next;       // Pipe ile bağlı sonraki komut
}	t_command;


// --- 3. FONKSİYON PROTOTİPLERİ ---

// Dış dünyaya açılan ana fonksiyonlar
t_command	*parse(char *line); // Ana parser fonksiyonu: string alır, AST döndürür.

// İç fonksiyonlar (istersen ayrı bir private_parser.h'a da koyabilirsin)
t_token		*lexer(const char *line); // String'i token listesine çevirir.

// Temizlik fonksiyonları
void		free_tokens(t_token *tokens);
void		free_ast(t_command *ast);


#endif