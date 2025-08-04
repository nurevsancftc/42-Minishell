/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldurmaz <aldurmaz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:11:22 by aldurmaz          #+#    #+#             */
/*   Updated: 2025/08/04 19:54:00 by aldurmaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parser/lexer.c

#include "minishell.h"

// Metakarakterleri işler ve işlenen karakter sayısını döndürür.
static int	handle_metachar(const char *line, t_token **token_list)
{
	if (*line == '>')
	{
		if ((*(line + 1) == '>' && *(line + 2) != '>') && (*(line + 1) == '>' && *(line + 2) != '<'))
		{
			add_token_to_list(token_list, create_token(">>", TOKEN_REDIR_APPEND));
			return 2;
		}
		else if ((*(line + 1) != '>') && (*(line + 1) != '<'))
		{
			add_token_to_list(token_list, create_token(">", TOKEN_REDIR_OUT));
			return 1;
		}
		else
		{
			if(*(line + 2) == '>')
				printf("minishell: syntax error near unexpected token `>'\n");
			if(*(line + 2) == '<')
				printf("minishell: syntax error near unexpected token `<'\n");
			return -1; // Hatalı: >>> gibi
		}
	}
	if (*line == '<')
	{
		if ((*(line + 1) == '<' && *(line + 2) != '<') && (*(line + 1) == '<' && *(line + 2) != '>'))
		{
			add_token_to_list(token_list, create_token("<<", TOKEN_HEREDOC));
			return 2;
		}
		else if ((*(line + 1) != '<') && (*(line + 1) != '>'))
		{
			add_token_to_list(token_list, create_token("<", TOKEN_REDIR_IN));
			return 1;
		}
		else
		{
			if(*(line + 2) == '>')
				printf("minishell: syntax error near unexpected token `>'\n");
			if(*(line + 2) == '<')
				printf("minishell: syntax error near unexpected token `<'\n");
			return -1; // Hatalı: >>> gibi
		}
	}
	if (*line == '|')
	{
		add_token_to_list(token_list, create_token("|", TOKEN_PIPE));
		return 1;
	}
	return (0);
}

// Normal kelimeleri (tırnaklar hariç) işler ve kelimenin uzunluğunu döndürür.
static int	handle_word(const char *line, t_token **token_list)
{
	int		i;
	char	*word;

	i = 0;
	// Boşluk, metakarakter veya tırnak görene kadar ilerle.
	while (line[i] && !is_whitespace(line[i]) && \
		   !is_metachar(line[i]) && line[i] != '\'' && line[i] != '"')
	{
		i++;
	}
	word = ft_substr(line, 0, i);
	add_token_to_list(token_list, create_token(word, TOKEN_WORD));
	free(word);
	return (i);
}

/*
 * LEXER ANA FONKSİYONU
 * Görevi: Ham komut satırını alır ve bir token listesi döndürür.
 * 1. Boş bir token listesi başlatır.
 * 2. Satırın başından sonuna kadar bir döngü ile ilerler.
 * 3. Her adımda:
 *    a. Boşlukları atlar.
 *    b. Metakarakterleri, alıntıları veya kelimeleri işlemek için
 *       ilgili yardımcı fonksiyonları çağırır.
 * 4. Helper fonksiyonlar işledikleri kısmın uzunluğunu döndürür.
 *    Döngü bu uzunluk kadar ilerletilir.
 * 5. Satırın sonuna gelindiğinde, oluşturulan token listesini döndürür.
*/
t_token	*lexer(const char *line)
{
	t_token	*token_list;
	int		i;
	int		processed_len;

	token_list = NULL;
	i = 0;
	while (line[i])
	{
		processed_len = 0;
		if (is_whitespace(line[i]))
			processed_len = 1;
		else if (is_metachar(line[i]))
			processed_len = handle_metachar(&line[i], &token_list);
		else if (line[i] == '\'' || line[i] == '"')
			processed_len = handle_quotes(&line[i], &token_list); // Bu quotes.c'de olacak
		else
			processed_len = handle_word(&line[i], &token_list);

		if (processed_len < 0) // Hata durumu (örn: kapanmamış tırnak)
		{
			free_token_list(token_list); // O ana kadar olanı temizle
			return (NULL);
		}
		i += processed_len;
	}
	return (token_list);
}